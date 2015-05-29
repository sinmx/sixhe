#include "StdAfx.h"
#include "OrderManager.h"
#include "ConfManager.h"
#include "CommunicationDefine.h"

//数据库文件名
#define DB_NAME		_T("bmp.db")

//单子表名称
#define ORDER_TABLE_NAME		_T("ordertable")

//订单表各列名称
#define ORDER_COLUMN_NAME		_T("name")
#define ORDER_COLUMN_CODE_PREFIX	_T("code")

//吃码表名称
#define EAT_TABLE_NAME		_T("eat")

//吃码表各个列名称 name code1 code2
#define EAT_COLUMN_NAME		_T("name")   //吃码表固定只有一行，name只是作为primary key
#define EAT_COLUMN_CODE_PREFIX		_T("code")

//吃码表固定只有一行，name固定为此值
#define EAT_TABLE_PRIMARY_KEY_VALUE	_T("key")

CCS COrderManager::m_synCCS;
COrderManager::COrderManager(void)
{
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		m_eatCode[i].SetCode(i);
	}
}

COrderManager::~COrderManager(void)
{
}

COrderManager* COrderManager::Instance()
{
	CSynObject synObj(&m_synCCS);

	static COrderManager thisObj;
	return &thisObj;
}

void COrderManager::InitOrder()
{
	//创建订单表并读取数据
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = ORDER_COLUMN_NAME;
	nameColumn.dataType = DB_DATA_TYPE_STR;
	nameColumn.IsPrimaryKey = true;
	columnVec.push_back(nameColumn);

	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		COLUMN_ITEM codeColumn;
		codeColumn.strName.Format(_T("%s%d"), ORDER_COLUMN_CODE_PREFIX, i);
		codeColumn.dataType = DB_DATA_TYPE_INT;
		columnVec.push_back(codeColumn);
	}	

	nResult = dbUtil.CreateTable(ORDER_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to create order table, error=%d."), nResult);
		return;
	}

	m_orderVec.clear();
	vector<COLUMN_ITEM> whereVec;
	nResult = dbUtil.Query(ORDER_TABLE_NAME, whereVec, EnumOrderRecord, (LPARAM)this);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to query orders, error=%d."), nResult);
		return;
	}
}

void COrderManager::InitEatTable()
{
	//默认吃码值为单子各码的总额
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		m_eatCode[i].Buy(GetTotalBuy(i));
	}	

	//数据库有吃码值，就从数据库中读取数据
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = EAT_COLUMN_NAME;
	nameColumn.dataType = DB_DATA_TYPE_STR;
	nameColumn.IsPrimaryKey = true;
	columnVec.push_back(nameColumn);

	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		COLUMN_ITEM codeColumn;
		codeColumn.strName.Format(_T("%s%d"), EAT_COLUMN_CODE_PREFIX, i);
		codeColumn.dataType = DB_DATA_TYPE_INT;
		columnVec.push_back(codeColumn);
	}

	nResult = dbUtil.CreateTable(EAT_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to create eat table, error=%d."), nResult);
		return;
	}

	vector<COLUMN_ITEM> whereVec;
	nResult = dbUtil.Query(EAT_TABLE_NAME, whereVec, EnumEatRecord, (LPARAM)this);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to query eat table, error=%d."), nResult);
		return;
	}
}

void COrderManager::Init()
{
	CSynObject synObj(&m_synCCS);

	//初始化单子
	InitOrder();

	//初始化吃码表
	InitEatTable();
}

bool COrderManager::DeleteEatTable()
{
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> whereVec;
	nResult = dbUtil.Delete(EAT_TABLE_NAME, whereVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to delete the eat table, error=%d."), nResult);
		return false;
	}

	//重新初始化吃码数据
	InitEatTable();

	return true;
}

void COrderManager::CalcAbsoluteProfit(__int64 nMinEat, __int64 nMaxEat, __int64& nAbsProfit, __int64& nAvgEat)
{
	//用于计算平均吃码一个数时，保存最大最小利润及对应的开奖号码
	__int64 nMin1Profit, nMax1Profit, nMin2Profit, nMax2Profit, nMin3Profit, nMax3Profit;
	int nMin1Code, nMax1Code, nMin2Code, nMax2Code, nMin3Code, nMax3Code;
	
	//计算如果平均吃码nMinEat时对应的最小利润
	AvgEat(nMinEat, nMin1Code, nMin1Profit, nMax1Code, nMax1Profit);

	//计算如果平均吃码nMaxEat时对应的最小利润
	AvgEat(nMaxEat, nMin2Code, nMin2Profit, nMax2Code, nMax2Profit);

	__int64 nAvgValue = (nMinEat+nMaxEat)/2;
	if (nAvgValue == nMinEat || nAvgValue == nMaxEat)  //已经是中间了，不用再查找取其最大值即可
	{
		if (nMin1Profit < nMin2Profit)
		{
			nAbsProfit = nMin2Profit;
			nAvgEat = nMaxEat;
		}
		else
		{
			nAbsProfit = nMin1Profit;
			nAvgEat = nMinEat;
		}

		return;
	}

	//计算如果平均吃码nAvgValue时对应的最小利润
	AvgEat(nAvgValue, nMin3Code, nMin3Profit, nMax3Code, nMax3Profit);

	__int64 nAbsProfit1, nAvgEat1;
	__int64 nAbsProfit2, nAvgEat2;
	if (nMin3Profit >= nMin2Profit && nMin3Profit >= nMin1Profit)
	{
		//最大值可能位于[min, avg]或[avg,max]
		CalcAbsoluteProfit(nMinEat, nAvgValue, nAbsProfit1, nAvgEat1);
		CalcAbsoluteProfit(nAvgValue, nMaxEat, nAbsProfit2, nAvgEat2);
		if (nAbsProfit1 <= nAbsProfit2)
		{
			nAbsProfit = nAbsProfit2;
			nAvgEat = nAvgEat2;
		}
		else
		{
			nAbsProfit = nAbsProfit1;
			nAvgEat = nAvgEat1;
		}		
	}
	else if (nMin3Profit >= nMin2Profit && nMin3Profit < nMin1Profit)
	{
		//最大值位于[min, avg]之间
		CalcAbsoluteProfit(nMinEat, nAvgValue, nAbsProfit, nAvgEat);
	}
	else if (nMin3Profit < nMin2Profit && nMin3Profit >= nMin1Profit)
	{
		//最大值位于[avg, max]之间
		CalcAbsoluteProfit(nAvgValue, nMaxEat, nAbsProfit2, nAvgEat2);
	}
	else if (nMin3Profit < nMin2Profit && nMin3Profit < nMin1Profit)
	{
		ASSERT(false);  //不可能会有此情况
	}
}

void COrderManager::CalcAbsoluteProfit(__int64& nAbsProfit, __int64& nAvgEat)
{
	/*
	假设平均吃码x，开每个号码都会有一个利润值，这些利润值中的最小值记为y，
	假设每个号码购买的总数为a1,a2,a3,...,an，因为下注总数可能小于平均吃码值，于是真正吃码的数目
	应为：a1,a2,a3,..ak, x, x, x，其中a1-ak是小于x，n>=k>=1，x的个数为n-k
	假设折扣为d,赔率为b，那么利润 y=(sum(a1-ak) + x(n-k)) * d - x*b ; 
	假设ak<x1<x2<a[k+1]，那么记deltaX = x2-x1, deltaY = y2-y1，
	deltaY = (n-k)d*deltaX-b*deltaX = (nd-kd-b) * deltaX；nd-kd-b=0时就是所有y中取得最大值，也就是绝对利润值，
	由于k随着x的增大而逐渐增大，所以nd-kd-b逐渐减小，在减小到0之前，由于还是整数，所以y是逐渐增大的，
	在减小到0后，由于是小于0的，所以y是逐渐减小的，整个y的单调过程是先逐渐增大再逐渐减小，如果一开始就小于0，那就是一直逐渐减小。

	根据y的单调规则，可以通过二分查找法找到y的最大值，假设下注总数最小值为min，最大值为max，
	min和max的平均值为avg，对应的y为Ymin,Ymax,Yavg，如果Yavg>Ymin && Yavg>Ymax那么y最大值可能位于
	[min, avg]和[avg,max]之间，对这两个区间继续计算求出最大值； 如果Ymax>Yavg>Ymin，那么最大值位于[avg, max]之间；
	如果Ymin>Yavg>Ymax，最大值位于[min, avg]之间。
	*/

	CSynObject synObj(&m_synCCS);

	//默认为没有绝对利润
	nAbsProfit = 0;
	nAvgEat = 0;
	
	__int64 nMinEat = GetTotalBuy(1);
	__int64 nMaxEat = nMinEat;
	for (int i=2; i<= COrder::CODE_NUMBER; i++)
	{
		__int64 nTotal = GetTotalBuy(i);
		if (nTotal < nMinEat)
		{
			nMinEat = nTotal;
		}
		else if (nTotal > nMaxEat)
		{
			nMaxEat = nTotal;
		}
	}

	if (nMinEat == 0 && nMaxEat == 0)  //根本就没有单子
	{
		return;
	}

	CalcAbsoluteProfit(nMinEat, nMaxEat, nAbsProfit, nAvgEat);
}

void COrderManager::CalcMinProfit(__int64 nProfit, __int64& nAvgEat)
{
	CSynObject synObj(&m_synCCS);

	__int64 nMinEat = GetTotalBuy(1);
	__int64 nMaxEat = nMinEat;
	for (int i=2; i<= COrder::CODE_NUMBER; i++)
	{
		__int64 nTotal = GetTotalBuy(i);
		if (nTotal < nMinEat)
		{
			nMinEat = nTotal;
		}
		else if (nTotal > nMaxEat)
		{
			nMaxEat = nTotal;
		}
	}

	int nMinCode, nMaxCode;
	__int64 nMinProfit, nMaxProfit;
	AvgEat(nMaxEat, nMinCode, nMinProfit, nMaxCode, nMaxProfit);
	if (nMinProfit >= nProfit)  //吃最大码可满足条件
	{
		nAvgEat = nMaxEat;
		return;
	}

	__int64 nAbsProfit, nTempAvgEat;
	CalcAbsoluteProfit(nAbsProfit, nTempAvgEat);
	if (nAbsProfit < nProfit)  //不可能大于绝对利润，否则绝对利润就是你了
	{
		nAvgEat = 0;
		return;
	}

	//[nTempAvgEat, nMaxEat]间的绝对利润是单调递增的，所以采用二分发查找
	CalcMinProfit(nTempAvgEat, nMaxEat, nProfit, nAvgEat);
}

void COrderManager::CalcAvgEatProfit(__int64 nAvgEat, __int64& nMinProfit, __int64& nMaxProfit)
{
	CSynObject synObj(&m_synCCS);

	int nMinCode, nMaxCode;
	AvgEat(nAvgEat, nMinCode, nMinProfit, nMaxCode, nMaxProfit);
}

void COrderManager::PrintSelfBuyTable()
{
	CSynObject synObj(&m_synCCS);

	CString strItem;
	CString strSellTable;

	//标题居中显示
	for (int i=1; i<=30; i++)  //前面30个空格
	{
		strSellTable += _T(" ");
	}
	strSellTable += CLanguageManager::Instance()->LoadString(10)+_T("\r\n\r\n");  //10=上报表

	//前面48个号码，12行4列
	for (int i=1; i<=12; i++)
	{
		for (int j=0; j<4; j++)
		{
			CCode code;
			code.SetCode(j*12+i);			

			__int64 nSelfBuy = COrderManager::Instance()->GetSelfBuy(code.GetCode());
			if (nSelfBuy > 0)
			{
				strItem.Format(_T("%02d%s %-10d  "), code.GetCode(), code.GetShengXiao(), nSelfBuy);
			}
			else
			{
				strItem.Format(_T("%02d%s %10s  "), code.GetCode(), code.GetShengXiao(), _T(" "));
			}
			
			strSellTable += strItem;
		}

		strSellTable += _T("\r\n");
	}

	//第49个号码与最后一列对齐
	for (int i=1; i<=51; i++)  //先51个空格
	{
		strSellTable += _T(" ");
	}

	CCode code;
	code.SetCode(49);
	__int64 nSelfBuy = COrderManager::Instance()->GetSelfBuy(code.GetCode());
	if (nSelfBuy > 0)
	{
		strItem.Format(_T("%02d%s %-10d  "), code.GetCode(), code.GetShengXiao(), nSelfBuy);
	}
	else
	{
		strItem.Format(_T("%02d%s %10s  "), code.GetCode(), code.GetShengXiao(), _T(" "));
	}
	strSellTable += strItem;

	//写入到文件中
	CString strFile = CPathUtilEx::Instance()->GetImagePath()+CLanguageManager::Instance()->LoadString(10)+_T(".txt");
	CUnicodeFile file;
	if (file.Open(strFile, CFile::modeWrite|CFile::modeCreate)) //10=上报表
	{
		file.WriteString(strSellTable);
		file.Close();

		::ShellExecute(NULL, _T("open"), strFile, NULL, NULL, SW_SHOWNORMAL);
	}
}

__int64 COrderManager::CalcCodeProfit(int nCode)
{
	CSynObject synObj(&m_synCCS);

	return GetEatValue()*(1000-CConfManager::Instance()->AvgDiscount())/1000-m_eatCode[nCode].Money()*CConfManager::Instance()->AvgBettingOdds();
}

void COrderManager::GetMinMaxProfit(__int64& nMinProfit, int& nMinCode, __int64& nMaxProfit, int& nMaxCode)
{
	CSynObject synObj(&m_synCCS);

	nMinCode = 1;
	nMaxCode = 1;
	nMinProfit = CalcCodeProfit(1);
	nMaxProfit = nMinProfit;
	for (int i=2; i<=COrder::CODE_NUMBER; i++)
	{
		__int64 nProfit = CalcCodeProfit(i);
		if (nProfit < nMinProfit)
		{
			nMinProfit = nProfit;
			nMinCode = i;
		}
		else if (nProfit > nMaxProfit)
		{
			nMaxProfit = nProfit;
			nMaxCode = i;
		}
	}
}

void COrderManager::CalcMinProfit(__int64 nMinEat, __int64 nMaxEat, __int64 nProfit, __int64& nAvgEat)
{
	int nMinCode, nMaxCode;
	__int64 nMinProfit, nMaxProfit;
	AvgEat(nMaxEat, nMinCode, nMinProfit, nMaxCode, nMaxProfit);
	if (nMinProfit >= nProfit)  //吃最大码可满足条件
	{
		nAvgEat = nMaxEat;
		return;
	}

	__int64 nMidValue = (nMinEat+nMaxEat)/2;
	if (nMidValue == nMinEat || nMidValue == nMaxEat)  //没有中间值了
	{
		AvgEat(nMinEat, nMinCode, nMinProfit, nMaxCode, nMaxProfit);
		if (nMinProfit >= nProfit)  //吃小码可以满足条件
		{
			nAvgEat = nMinEat;
		}
		else  //都无法满足，没找到，不建议吃值
		{
			nAvgEat = 0;
		}

		return;
	}

	AvgEat(nMidValue, nMinCode, nMinProfit, nMaxCode, nMaxProfit);
	if (nMinProfit >= nProfit)  //可满足条件，继续在[mid, max]间查找更大符合条件的吃码
	{
		CalcMinProfit(nMidValue, nMaxEat, nProfit, nAvgEat);
	}
	else  //[min, mid]间查找更大符合条件的吃码
	{
		CalcMinProfit(nMinEat, nMidValue, nProfit, nAvgEat);
	}
}

void COrderManager::AvgEat(__int64 nEat, int& nMinCode, __int64& nMinProfit, int& nMaxCode, __int64& nMaxProfit)
{
	//计算各号码吃码值，如果下注总数比平均吃码值来得小，以下注总数为准
	__int64 nTotalEat = 0;
	CCode eatCode[COrder::CODE_NUMBER+1];
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		eatCode[i].SetCode(i);

		__int64 nTotalBuy = GetTotalBuy(i);
		if (nEat > nTotalBuy)
		{
			eatCode[i].Buy(nTotalBuy);
		}
		else
		{
			eatCode[i].Buy(nEat);
		}

		nTotalEat += eatCode[i].Money();
	}

	//总收入为吃码总和乘以平均回水
	nTotalEat = nTotalEat*(1000-CConfManager::Instance()->AvgDiscount()) /1000;

	//计算最大值和最小值
	nMaxCode = 1;
	nMinCode = 1;
	nMaxProfit = nTotalEat - eatCode[1].Money()*CConfManager::Instance()->AvgBettingOdds();
	nMinProfit = nMaxProfit;
	for (int i=2; i<=COrder::CODE_NUMBER; i++)
	{
		__int64 nTemp = nTotalEat - eatCode[i].Money()*CConfManager::Instance()->AvgBettingOdds();
		if (nMinProfit > nTemp)
		{
			nMinProfit = nTemp;
			nMinCode = i;
		}

		if (nMaxProfit < nTemp)
		{
			nMaxProfit = nTemp;
			nMaxCode = i;
		}
	}
}

bool COrderManager::DeleteOrder(const CString& strUserName)
{
	CSynObject synObj(&m_synCCS);

	//如果本来就没有订单，无需删除
	if (!strUserName.IsEmpty())
	{
		bool bHasOrder = false;
		for (vector<COrder>::iterator it=m_orderVec.begin(); it!=m_orderVec.end(); it++)
		{
			if (it->Name() == strUserName)
			{
				bHasOrder = true;
				break;
			}
		}

		if (!bHasOrder)
		{
			return true;
		}
	}
	
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> whereVec;
	if (!strUserName.IsEmpty())
	{
		COLUMN_ITEM nameColumn;
		nameColumn.strName = ORDER_COLUMN_NAME;
		nameColumn.strValue = strUserName;
		whereVec.push_back(nameColumn);
	}	

	nResult = dbUtil.Delete(ORDER_TABLE_NAME, whereVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to delete the order(%s), error=%d."), strUserName, nResult);
		return false;
	}

	//从列表中删除
	if (!strUserName.IsEmpty())
	{
		for (vector<COrder>::iterator it=m_orderVec.begin(); it!=m_orderVec.end(); it++)
		{
			if (it->Name() == strUserName)
			{
				m_orderVec.erase(it);
				break;
			}
		}
	}
	else
	{
		m_orderVec.clear();
	}

	//单子发生变化，吃码表要设置为失效
	if (!DeleteEatTable())
	{
		theLog.LogError(_T("Failed to delete eat table."));
	}

	//通知监听对象数据发生变化了
	for (vector<ICommunication*>::const_iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
	{
		(*it)->AcceptEvent(COMEVENT_OrderChange, (WPARAM)(ORDER_CHANGE_MASK_ORDER|ORDER_CHANGE_MASK_EAT_TABLE));
	}	

	return true;
}

bool COrderManager::GetOrder(const CString& strUserName, COrder& order) const
{
	CSynObject synObj(&m_synCCS);

	for (vector<COrder>::const_iterator it=m_orderVec.begin(); it!=m_orderVec.end(); it++)
	{
		if (it->Name() == strUserName)
		{
			order = (*it);
			return true;
		}
	}

	return false;
}

bool COrderManager::UpdateOrder(const CString& strUserName, const COrder& order)
{
	CSynObject synObj(&m_synCCS);

	bool bSuccess = false;
	if (order.GetTotalBuyMoney() == 0)  //没有购买
	{
		bSuccess = DeleteOrder(strUserName);
	}
	else
	{
		COrder tempOrder = order;
		tempOrder.Name(strUserName);
		bSuccess =  InsertOrder(tempOrder);
	}

	//单子吃码表数据发生变化，吃码表要设置为失效
	if (bSuccess)
	{
		if (!DeleteEatTable())
		{
			theLog.LogError(_T("Failed to delete eat table."));
		}

		for (vector<ICommunication*>::const_iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
		{
			(*it)->AcceptEvent(COMEVENT_OrderChange, (WPARAM)(ORDER_CHANGE_MASK_ORDER|ORDER_CHANGE_MASK_EAT_TABLE));
		}
	}

	return bSuccess;
}

bool COrderManager::UpdateOrder(const CString& strUserName, const vector<CCode>& buyVec)
{
	if (buyVec.empty())
	{
		return true;
	}

	CSynObject synObj(&m_synCCS);

	COrder order;
	GetOrder(strUserName, order);
	for (vector<CCode>::const_iterator it=buyVec.begin(); it!=buyVec.end(); it++)
	{
		order.Buy(it->GetCode(), it->Money());
	}

	return UpdateOrder(strUserName, order);
}

bool COrderManager::OrderAppend(const CString& strUserName, const vector<CCode>& buyVec)
{
	if (buyVec.empty())
	{
		return true;
	}

	CSynObject synObj(&m_synCCS);

	COrder order;
	GetOrder(strUserName, order);
	for (vector<CCode>::const_iterator it=buyVec.begin(); it!=buyVec.end(); it++)
	{
		__int64 nMoney = order.Money(it->GetCode()) + it->Money();
		if (nMoney < 0)
		{
			nMoney = 0;
		}
		order.Buy(it->GetCode(), nMoney);
	}

	return UpdateOrder(strUserName, order);
}

bool COrderManager::IsHasOrder()
{
	CSynObject synObj(&m_synCCS);

	return m_orderVec.size()>0;
}

bool __stdcall  COrderManager::EnumOrderRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec)
{
	COrderManager* thisObj = (COrderManager*)context;
	COrder order;
	for (vector<COLUMN_ITEM>::const_iterator it=recordVec.begin(); it!=recordVec.end(); it++)
	{
		if (it->strName == ORDER_COLUMN_NAME)
		{
			order.Name(it->strValue);
		}
		else if (it->strName.Find(ORDER_COLUMN_CODE_PREFIX) == 0)
		{
			int nIndex = _ttoi(it->strName.Mid((int)_tcslen(ORDER_COLUMN_CODE_PREFIX)));
			if (nIndex>=1 && nIndex<=COrder::CODE_NUMBER)
			{
				order.Buy(nIndex, _ttoi64(it->strValue));
			}
		}
	}

	thisObj->m_orderVec.push_back(order);

	return true;
}

bool __stdcall  COrderManager::EnumEatRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec)
{
	COrderManager* thisObj = (COrderManager*)context;
	for (vector<COLUMN_ITEM>::const_iterator it=recordVec.begin(); it!=recordVec.end(); it++)
	{
		if (it->strName.Find(EAT_COLUMN_CODE_PREFIX) == 0)
		{
			int nIndex = _ttoi(it->strName.Mid((int)_tcslen(EAT_COLUMN_CODE_PREFIX)));
			if (nIndex>=1 && nIndex<=COrder::CODE_NUMBER)
			{
				thisObj->m_eatCode[nIndex].Buy(_ttoi64(it->strValue));				
			}
		}		
	}

	return true;
}

bool COrderManager::InsertOrder(const COrder& order)
{
	//将订单插入到数据库中
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = ORDER_COLUMN_NAME;
	nameColumn.strValue = order.Name();
	columnVec.push_back(nameColumn);

	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		COLUMN_ITEM codeColumn;
		codeColumn.strName.Format(_T("%s%d"), ORDER_COLUMN_CODE_PREFIX, i);
		codeColumn.strValue.Format(_T("%I64d"), order.Money(i));
		columnVec.push_back(codeColumn);
	}

	nResult = dbUtil.Insert(ORDER_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to insert order(%s) to database, error=%d."), order.Name(), nResult);
		return false;
	}

	//加入到列表中
	for (vector<COrder>::iterator it=m_orderVec.begin(); it!=m_orderVec.end(); it++)
	{
		if (it->Name() == order.Name())  //找到，删除它
		{
			m_orderVec.erase(it);
			break;
		}
	}
	m_orderVec.push_back(order);

	return true;
}

bool COrderManager::RestEatTable()
{
	CSynObject synObj(&m_synCCS);

	//默认吃码值为单子各码的总额
	CCode tmpEatCode[COrder::CODE_NUMBER+1];
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		tmpEatCode[i].SetCode(i);
		tmpEatCode[i].Buy(GetTotalBuy(i));
	}

	return InsertEatTable(tmpEatCode);
}

bool COrderManager::InsertEatTable(CCode tmpEatCode[COrder::CODE_NUMBER+1])
{
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = EAT_COLUMN_NAME;
	nameColumn.strValue = EAT_TABLE_PRIMARY_KEY_VALUE;
	columnVec.push_back(nameColumn);

	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		COLUMN_ITEM codeColumn;
		codeColumn.strName.Format(_T("%s%d"), EAT_COLUMN_CODE_PREFIX, i);
		codeColumn.strValue = CStringUtil::Int64ToStr(tmpEatCode[i].Money());		
		columnVec.push_back(codeColumn);
	}	

	nResult = dbUtil.Insert(EAT_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to insert eat to database, error=%d."),  nResult);
		return false;
	}

	//更新下内存数据
	for (int nCode=1; nCode<=COrder::CODE_NUMBER; nCode++)
	{
		m_eatCode[nCode] = tmpEatCode[nCode];
	}

	//通知监听对象，吃码表数据变化了
	for (vector<ICommunication*>::const_iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
	{
		(*it)->AcceptEvent(COMEVENT_OrderChange, (WPARAM)(ORDER_CHANGE_MASK_EAT_TABLE));
	}

	return true;
}

bool COrderManager::UpdateEatTable(const vector<CCode>& eatVec)
{
	CSynObject synObj(&m_synCCS);

	if (eatVec.size() <= 0)
	{
		return true;
	}

	//要保存成功才能更新内存数据，所以先生成一份临时吃码表
	CCode tmpEatCode[COrder::CODE_NUMBER+1];
	for (int nCode=1; nCode<=COrder::CODE_NUMBER; nCode++)
	{
		tmpEatCode[nCode] = m_eatCode[nCode];
	}

	for (vector<CCode>::const_iterator it=eatVec.begin(); it!= eatVec.end(); it++)
	{
		//吃码不能超过下注总数
		__int64 nTotalBuy = GetTotalBuy(it->GetCode());
		if (it->Money() <= nTotalBuy)
		{
			tmpEatCode[it->GetCode()].Buy(it->Money());
		}
		else
		{
			tmpEatCode[it->GetCode()].Buy(nTotalBuy);
		}
	}

	return InsertEatTable(tmpEatCode);
}

__int64 COrderManager::GetEatValue(int nCode) const
{
	CSynObject synObj(&m_synCCS);

	if (nCode>=1 && nCode<=COrder::CODE_NUMBER)
	{
		return m_eatCode[nCode].Money();
	}
	else
	{
		return 0;
	}
}

__int64 COrderManager::GetEatValue() const
{
	CSynObject synObj(&m_synCCS);

	__int64 nSum = 0;
	for (int nCode=1; nCode<=COrder::CODE_NUMBER; nCode++)
	{
		nSum += m_eatCode[nCode].Money();
	}

	return nSum;
}

__int64 COrderManager::GetSelfBuy(int nCode) const
{
	CSynObject synObj(&m_synCCS);

	return GetTotalBuy(nCode) - GetEatValue(nCode);
}

bool COrderManager::IsAllEat()
{
	CSynObject synObj(&m_synCCS);

	return GetTotalBuy()==GetEatValue();
}

__int64 COrderManager::GetTotalBuy(int nCode) const
{
	CSynObject synObj(&m_synCCS);

	if (nCode <1 ||  nCode>COrder::CODE_NUMBER)
	{
		return 0;
	}

	__int64 nSum = 0;
	for (vector<COrder>::const_iterator it=m_orderVec.begin(); it!=m_orderVec.end(); it++)
	{
		nSum += it->Money(nCode);
	}

	return nSum;
}

__int64 COrderManager::GetTotalBuy(const CString& strCustomer) const
{
	CSynObject synObj(&m_synCCS);

	COrder order;
	if (GetOrder(strCustomer, order))
	{
		return order.GetTotalBuyMoney();
	}

	return 0;
}

__int64 COrderManager::GetBuyValue(const CString& strCustomer, int nCode) const
{
	CSynObject synObj(&m_synCCS);

	COrder order;
	if (GetOrder(strCustomer, order))
	{
		return order.Money(nCode);
	}

	return 0;
}

__int64 COrderManager::GetTotalBuy() const
{
	CSynObject synObj(&m_synCCS);	

	__int64 nSum = 0;
	for (vector<COrder>::const_iterator it=m_orderVec.begin(); it!=m_orderVec.end(); it++)
	{
		nSum += it->GetTotalBuyMoney();
	}

	return nSum;
}

void COrderManager::Attach(ICommunication* pObj)
{
	CSynObject synObj(&m_synCCS);

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

void COrderManager::Detach(ICommunication* pObj)
{
	CSynObject synObj(&m_synCCS);

	for (vector<ICommunication*>::iterator it=m_listenerVec.begin(); it!=m_listenerVec.end(); it++)
	{
		if ((*it) == pObj)
		{
			m_listenerVec.erase(it);
			break;
		}
	}
}