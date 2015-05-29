#include "stdafx.h"
#include "RewardManager.h"
#include "CustomerManager.h"
#include "OrderManager.h"
#include "ConfManager.h"

//���ݿ��ļ���
#define DB_NAME		_T("bmp.db")

//�ҽ�������
#define REWARD_TABLE_NAME		_T("reward")

//�ҽ����������
#define REWARD_COLUMN_NAME				_T("name")
#define REWARD_COLUMN_ALREADY_REWARD	_T("alreadyReward")

CCS CRewardManager::m_synCCS;

CRewardManager::CRewardManager()
{
	//
}

CRewardManager::~CRewardManager()
{
	//
}

CRewardManager* CRewardManager::Instance()
{
	CSynObject synObj(&m_synCCS);

	static CRewardManager thisObj;
	return &thisObj;
}

void CRewardManager::Init()
{
	CSynObject synObj(&m_synCCS);

	//�����û�����ȡ�û�����
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = REWARD_COLUMN_NAME;
	nameColumn.dataType = DB_DATA_TYPE_STR;
	nameColumn.IsPrimaryKey = true;
	columnVec.push_back(nameColumn);

	COLUMN_ITEM alreadyColumn;
	alreadyColumn.strName = REWARD_COLUMN_ALREADY_REWARD;
	alreadyColumn.dataType = DB_DATA_TYPE_INT;	
	columnVec.push_back(alreadyColumn);	

	nResult = dbUtil.CreateTable(REWARD_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to create reward table, error=%d."), nResult);
		return;
	}

	m_alreadyRewardVec.clear();
	vector<COLUMN_ITEM> whereVec;
	nResult = dbUtil.Query(REWARD_TABLE_NAME, whereVec, EnumRecord, (LPARAM)this);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to query reward information, error=%d."), nResult);
		return;
	}
}

vector<REWARD_ITEM> CRewardManager::GetRewardInfo()
{
	CSynObject synObj(&m_synCCS);

	vector<REWARD_ITEM> rewardInfo;
	int nWinCode = CConfManager::Instance()->WinCode();
	vector<CUSTOMER> customerList = CCustomerManager::Instance()->GetCustomerList();
	for (vector<CUSTOMER>::const_iterator it=customerList.begin(); it!=customerList.end(); it++)
	{
		REWARD_ITEM item;
		item.strCustomer = it->strName;
		item.nBettingOdds = it->nBettingOdds;
		item.nDiscount = it->nDiscount;
		item.nBuy = COrderManager::Instance()->GetTotalBuy(it->strName);
		if (item.nBuy == 0)  //���û�û�й���
		{
			continue;
		}

		item.nWin = COrderManager::Instance()->GetBuyValue(it->strName, nWinCode);
		item.nProfit = item.nWin*item.nBettingOdds - item.nBuy*(1000-item.nDiscount)/1000;
		item.bAlreadyReward = IsAlreadyReward(it->strName);

		rewardInfo.push_back(item);
	}

	return rewardInfo;
}

bool CRewardManager::SaveAlreadyReward(const CString& strCustomer, bool bAlready)
{
	CSynObject synObj(&m_synCCS);

	//�����ñ��浽���ݿ���
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM nameColumn;
	nameColumn.strName = REWARD_COLUMN_NAME;
	nameColumn.strValue = strCustomer;
	columnVec.push_back(nameColumn);

	COLUMN_ITEM alreadyColumn;
	alreadyColumn.strName = REWARD_COLUMN_ALREADY_REWARD;
	alreadyColumn.strValue = bAlready?_T("1"):_T("0");
	columnVec.push_back(alreadyColumn);	

	nResult = dbUtil.Insert(REWARD_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to insert reward(%s) to database, error=%d."), strCustomer, nResult);
		return false;
	}

	//���浽�ڴ���
	bool bAlreadyInVector = false;
	for (vector<REWARD_ITEM>::iterator it=m_alreadyRewardVec.begin(); it!=m_alreadyRewardVec.end(); it++)
	{
		if (it->strCustomer == strCustomer)
		{
			it->bAlreadyReward = bAlready;
			bAlreadyInVector = true;
			break;
		}
	}

	if (!bAlreadyInVector)
	{
		REWARD_ITEM item;
		item.strCustomer = strCustomer;
		item.bAlreadyReward = bAlready;
		m_alreadyRewardVec.push_back(item);
	}

	return true;
}

__int64 CRewardManager::CalcSelfProfit()
{
	CSynObject synObj(&m_synCCS);

	//�Լ����������
	int nWinCode = CConfManager::Instance()->WinCode();
	__int64 nTotalSelfBuy = 0;
	__int64 nWin = 0;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		__int64 nSelfBuy = COrderManager::Instance()->GetSelfBuy(i);
		nTotalSelfBuy += nSelfBuy;
		if (i == nWinCode)
		{
			nWin = nSelfBuy;
		}
	}	
	__int64 nSelfBuyProfit = nWin*CConfManager::Instance()->SelfBuyBettingOdds() - nTotalSelfBuy*(1000-CConfManager::Instance()->SelfBuyDiscount())/1000;
	
	//��ׯ���Լ����������
	vector<REWARD_ITEM> rewardInfo = GetRewardInfo();	
	__int64 nSum = 0;
	for (vector<REWARD_ITEM>::const_iterator it=rewardInfo.begin(); it!=rewardInfo.end(); it++)
	{
		nSum += it->nProfit;
	}

	//�Լ����� = ����ׯ�������� - ��ׯ���Լ����������
	return nSelfBuyProfit-nSum;
}

void CRewardManager::PrintRewardTable()
{
	CSynObject synObj(&m_synCCS);

	//��ʽ����ׯ  ����  ��ע����  ��ˮ(ǧ�ֵ�)  �н���  ����  �Ѷҽ�
	//ÿ���ֶ�Ϊ15���ַ�
	const int nBlockLen = 15;
	CString strTotalInfo;
	CString strEnterLine = _T("\r\n");

	//ͷ��
	strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(19), nBlockLen);  //19=��ׯ
	strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(20), nBlockLen);  //20=����
	strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(21), nBlockLen);  //21=��ע����
	strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(22), nBlockLen);  //22=��ˮ(ǧ�ֵ�)
	strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(23), nBlockLen);  //23=�н���
	strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(24), nBlockLen);  //24=����
	strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(25), nBlockLen);  //25=�Ѷҽ�
	strTotalInfo += strEnterLine;

	//ͷ���ָ���
	for (int i=1; i<=7*nBlockLen; i++)
	{
		strTotalInfo += _T('-');		
	}
	strTotalInfo += strEnterLine;

	//ÿ����ׯ�Ķҽ���Ϣ
	vector<REWARD_ITEM> rewardInfo = GetRewardInfo();
	for (vector<REWARD_ITEM>::const_iterator it=rewardInfo.begin(); it!=rewardInfo.end(); it++)
	{
		strTotalInfo += PaddingWhiteSpace(it->strCustomer, nBlockLen);
		strTotalInfo += PaddingWhiteSpace(CStringUtil::Int64ToStr(it->nProfit), nBlockLen);
		strTotalInfo += PaddingWhiteSpace(CStringUtil::Int64ToStr(it->nBuy), nBlockLen);
		strTotalInfo += PaddingWhiteSpace(CStringUtil::IntToStr(it->nDiscount), nBlockLen);
		strTotalInfo += PaddingWhiteSpace(CStringUtil::Int64ToStr(it->nWin), nBlockLen);
		strTotalInfo += PaddingWhiteSpace(CStringUtil::IntToStr(it->nBettingOdds), nBlockLen);

		if (it->bAlreadyReward)
		{
			strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(30), nBlockLen);  //30=��
		}
		else
		{
			strTotalInfo += PaddingWhiteSpace(CLanguageManager::Instance()->LoadString(31), nBlockLen);  //31=��
		}

		strTotalInfo += strEnterLine;
	}	

	//д�뵽�ļ���  32=�ҽ���
	CString strFile = CPathUtilEx::Instance()->GetImagePath()+CLanguageManager::Instance()->LoadString(32)+_T(".txt");
	CUnicodeFile file;
	if (file.Open(strFile, CFile::modeWrite|CFile::modeCreate))
	{
		file.WriteString(strTotalInfo);
		file.Close();

		::ShellExecute(NULL, _T("open"), strFile, NULL, NULL, SW_SHOWNORMAL);
	}
}

CString CRewardManager::PaddingWhiteSpace(const CString& strSource, int nMaxChar)
{
	int num = 0;
	for (int i=0; i<strSource.GetLength(); i++)
	{
		if (strSource[i] >= _T(' ') && strSource[i] <= _T('~'))
		{
			num++;
		}
		else
		{
			num += 2;  //�����ַ�����Ϊ��2���ַ�
		}
	}

	CString strTemp = strSource;
	if (num >= nMaxChar)
	{
		return strTemp;
	}
	else
	{
		for (int i=1; i<= nMaxChar-num; i++)
		{
			strTemp += _T(' ');
		}

		return strTemp;
	}
}

bool CRewardManager::IsAlreadyReward(const CString& strCustomer)
{
	for (vector<REWARD_ITEM>::const_iterator it=m_alreadyRewardVec.begin(); it!=m_alreadyRewardVec.end(); it++)
	{
		if (it->strCustomer == strCustomer)
		{
			return it->bAlreadyReward;
		}
	}

	return false;  //Ĭ��Ϊû�ҽ�
}

bool CRewardManager::EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec)
{
	CRewardManager* thisObj = (CRewardManager*)context;
	REWARD_ITEM item;
	for (vector<COLUMN_ITEM>::const_iterator it=recordVec.begin(); it!=recordVec.end(); it++)
	{
		if (it->strName == REWARD_COLUMN_NAME)
		{
			item.strCustomer = it->strValue;
		}
		else if (it->strName == REWARD_COLUMN_ALREADY_REWARD)
		{
			item.bAlreadyReward = (it->strValue == _T("1"))? true:false;
		}		
	}

	thisObj->m_alreadyRewardVec.push_back(item);

	return true;
}