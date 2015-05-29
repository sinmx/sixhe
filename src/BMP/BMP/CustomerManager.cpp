#include "StdAfx.h"
#include "CustomerManager.h"
#include "CommunicationDefine.h"
#include "LicenseManager.h"

//数据库文件名
#define DB_NAME		_T("bmp.db")

//用户表名称
#define CUSTOMER_TABLE_NAME		_T("customer")

//用户表各列名称
#define CUSTOMER_COLUMN_NAME		_T("name")
#define CUSTOMER_COLUMN_DISCOUNT			_T("discount")
#define CUSTOMER_COLUMN_BETTINGODDS		_T("bettingodds")

CCS CCustomerManager::m_csSyn;
CCustomerManager::CCustomerManager(void)
{
}

CCustomerManager::~CCustomerManager(void)
{
}

CCustomerManager* CCustomerManager::Instance()
{
	CSynObject synObj(&m_csSyn);

	static CCustomerManager thisObj;
	return &thisObj;
}

void CCustomerManager::Init()
{
	CSynObject synObj(&m_csSyn);

	//创建用户表并读取用户数据
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = CUSTOMER_COLUMN_NAME;
	nameColumn.dataType = DB_DATA_TYPE_STR;
	nameColumn.IsPrimaryKey = true;
	columnVec.push_back(nameColumn);

	COLUMN_ITEM discountColumn;
	discountColumn.strName = CUSTOMER_COLUMN_DISCOUNT;
	discountColumn.dataType = DB_DATA_TYPE_INT;	
	columnVec.push_back(discountColumn);

	COLUMN_ITEM bettingOddsColumn;
	bettingOddsColumn.strName = CUSTOMER_COLUMN_BETTINGODDS;
	bettingOddsColumn.dataType = DB_DATA_TYPE_INT;
	columnVec.push_back(bettingOddsColumn);

	nResult = dbUtil.CreateTable(CUSTOMER_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to create customer table, error=%d."), nResult);
		return;
	}

	m_customerVec.clear();
	vector<COLUMN_ITEM> whereVec;
	nResult = dbUtil.Query(CUSTOMER_TABLE_NAME, whereVec, EnumRecord, (LPARAM)this);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to query customers, error=%d."), nResult);
		return;
	}
}

bool CCustomerManager::InsertCustomer(const CUSTOMER& customer)
{
	//将该用户保存到数据库中
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = CUSTOMER_COLUMN_NAME;
	nameColumn.strValue = customer.strName;
	columnVec.push_back(nameColumn);

	COLUMN_ITEM discountColumn;
	discountColumn.strName = CUSTOMER_COLUMN_DISCOUNT;
	discountColumn.strValue.Format(_T("%d"), customer.nDiscount);
	columnVec.push_back(discountColumn);

	COLUMN_ITEM bettingOddsColumn;
	bettingOddsColumn.strName = CUSTOMER_COLUMN_BETTINGODDS;
	bettingOddsColumn.strValue.Format(_T("%d"), customer.nBettingOdds);
	columnVec.push_back(bettingOddsColumn);

	nResult = dbUtil.Insert(CUSTOMER_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to insert customer(%s) to database, error=%d."), customer.strName, nResult);
		return false;
	}

	//保存或更新到内存用户表中
	bool bAlreadyInVec = false;
	for (vector<CUSTOMER>::iterator it=m_customerVec.begin(); it!=m_customerVec.end(); it++)
	{
		if (it->strName == customer.strName)  //找到更新为它
		{
			bAlreadyInVec = true;
			(*it) = customer;
			break;
		}
	}

	if (!bAlreadyInVec) //新用户
	{
		m_customerVec.push_back(customer);
	}

	return true;
}

bool CCustomerManager::AddCustomer(CUSTOMER& customer, bool bNeedAutoGenerate)
{
	CSynObject synObj(&m_csSyn);

	if (bNeedAutoGenerate)
	{		
		customer.strName = GenerateOneCustomerName();
	}
	
	if (InsertCustomer(customer))
	{
		for (vector<ICommunication*>::iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
		{
			(*it)->AcceptEvent(COMEVENT_CustomerChange, (WPARAM)CUSTOMER_CHANGE_NEW_CUSTOMER, (LPARAM)(LPCTSTR)customer.strName);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool CCustomerManager::DeleteCustomer(const CString& strName)
{
	CSynObject synObj(&m_csSyn);

	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> whereVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = CUSTOMER_COLUMN_NAME;
	nameColumn.strValue = strName;
	whereVec.push_back(nameColumn);

	nResult = dbUtil.Delete(CUSTOMER_TABLE_NAME, whereVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to delete the customer(%s), error=%d."), strName, nResult);
		return false;
	}

	//从列表中删除	
	for (vector<CUSTOMER>::iterator it=m_customerVec.begin(); it!=m_customerVec.end(); it++)
	{
		if (it->strName == strName)
		{			
			m_customerVec.erase(it);
			break;
		}
	}

	//通知监听对象	
	for (vector<ICommunication*>::iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
	{
		(*it)->AcceptEvent(COMEVENT_CustomerChange, (WPARAM)CUSTOMER_CHANGE_DELETE_CUSTOMER, (LPARAM)(LPCTSTR)strName);
	}	

	return true;
}

bool CCustomerManager::UpdateCustomer(const CString& strOldName, const CUSTOMER& customer)
{
	CSynObject synObj(&m_csSyn);

	if (!customer.IsValid())  //无效用户数据
	{
		return false;
	}

	if (strOldName != customer.strName)
	{
		if (!IsValidName(customer.strName))  //名字冲突
		{
			return false;
		}
	}

	if (strOldName != customer.strName)  //更改名字，那么原来的用户会先被删除，然后增加新用户
	{
		CUSTOMER tempCustomer = customer;
		if (DeleteCustomer(strOldName) && AddCustomer(tempCustomer, false))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else  //插入新用户信息，数据库会直接替换
	{
		if (InsertCustomer(customer))
		{
			//通知监听对象	
			for (vector<ICommunication*>::iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
			{
				(*it)->AcceptEvent(COMEVENT_CustomerChange, (WPARAM)CUSTOMER_CHANGE_UPDATE_CUSOMTER, (LPARAM)(LPCTSTR)customer.strName);
			}

			return true;
		}
		else
		{
			return false;
		}
	}	
}

vector<CUSTOMER> CCustomerManager::GetCustomerList()
{
	CSynObject synObj(&m_csSyn);

	return m_customerVec;
}

int CCustomerManager::GetCustomerCount() const
{
	CSynObject synObj(&m_csSyn);

	return (int)m_customerVec.size();
}

bool CCustomerManager::IsHasCustomer()
{
	CSynObject synObj(&m_csSyn);

	return m_customerVec.size() > 0;
}

void CCustomerManager::Attach(ICommunication* pObj)
{
	CSynObject synObj(&m_csSyn);

	bool bHasAttach = false;
	for (vector<ICommunication*>::iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
	{
		if ((*it) == pObj)
		{
			bHasAttach = true;
			break;
		}
	}

	if (!bHasAttach)
	{
		m_listenerVec.push_back(pObj);
	}
}

void CCustomerManager::Detach(ICommunication* pObj)
{
	CSynObject synObj(&m_csSyn);

	for (vector<ICommunication*>::iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
	{
		if ((*it) == pObj)
		{
			m_listenerVec.erase(it);
			break;
		}
	}
}

bool CCustomerManager::EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec)
{
	CCustomerManager* thisObj = (CCustomerManager*)context;
	CUSTOMER customer;
	for (vector<COLUMN_ITEM>::const_iterator it=recordVec.begin(); it!=recordVec.end(); it++)
	{
		if (it->strName == CUSTOMER_COLUMN_NAME)
		{
			customer.strName = it->strValue;
		}
		else if (it->strName == CUSTOMER_COLUMN_DISCOUNT)
		{
			customer.nDiscount = _ttoi(it->strValue);
		}
		else if (it->strName == CUSTOMER_COLUMN_BETTINGODDS)
		{
			customer.nBettingOdds = _ttoi(it->strValue);
		}
	}

	//可能直接修改数据库，绕过下庄用户数检查，在此判断如果超出最大用户数，不添加
	if ((int)thisObj->m_customerVec.size() < CLicenseManager::Instance()->GetUserMaxCount())
	{
		thisObj->m_customerVec.push_back(customer);
	}	

	return true;
}

CString CCustomerManager::GenerateOneCustomerName()
{
	CString strName;
	int i=0;
	while (true)
	{		
		strName.Format(_T("%s%d"), CLanguageManager::Instance()->LoadString(2), ++i);  //2=下庄
		bool bFound = false;
		for (vector<CUSTOMER>::const_iterator it=m_customerVec.begin(); it!=m_customerVec.end(); it++)
		{
			if (it->strName == strName)
			{
				bFound = true;
				break;
			}
		}	

		if (!bFound) //没有冲突
		{
			break;
		}
	}

	return strName;
}

bool CCustomerManager::IsValidName(const CString& strName)
{
	for (vector<CUSTOMER>::const_iterator it=m_customerVec.begin(); it!=m_customerVec.end(); it++)
	{
		if (it->strName == strName)  //名字已经存在
		{
			return false;
		}
	}

	return true;
}