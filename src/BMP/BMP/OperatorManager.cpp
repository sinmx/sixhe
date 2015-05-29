#include "StdAfx.h"
#include "OperatorManager.h"

//数据库文件名
#define DB_NAME		_T("bmp.db")

//日志表名称
#define LOG_TABLE_NAME		_T("log")

//日志表各列名称
#define LOG_COLUMN_TYPE				_T("type")
#define LOG_COLUMN_CONTENT		_T("content")

//操作日志类型
#define LOG_TYPE_BUY						_T("buy")

CCS COperatorManager::m_synCCs;
COperatorManager::COperatorManager(void)
{
}

COperatorManager::~COperatorManager(void)
{
}

COperatorManager* COperatorManager::Instance()
{
	CSynObject synObj(&m_synCCs);

	static COperatorManager thisObj;
	return &thisObj;
}

void COperatorManager::Init()
{
	CSynObject synObj(&m_synCCs);

	//创建操作日志表并读取数据
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM typeColumn;
	typeColumn.strName = LOG_COLUMN_TYPE;
	typeColumn.dataType = DB_DATA_TYPE_STR;
	typeColumn.IsPrimaryKey = true;
	columnVec.push_back(typeColumn);

	COLUMN_ITEM contentColumn;
	contentColumn.strName = LOG_COLUMN_CONTENT;
	contentColumn.dataType = DB_DATA_TYPE_STR;
	columnVec.push_back(contentColumn);

	nResult = dbUtil.CreateTable(LOG_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to create log table, error=%d."), nResult);
		return;
	}

	m_strBuyLog = _T("");
	vector<COLUMN_ITEM> whereVec;
	nResult = dbUtil.Query(LOG_TABLE_NAME, whereVec, EnumRecord, (LPARAM)this);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to query log, error=%d."), nResult);
		return;
	}
}

CString COperatorManager::GetBuyLog()
{
	CSynObject synObj(&m_synCCs);

	return m_strBuyLog;
}

bool COperatorManager::ClearBuyLog()
{
	CSynObject synObj(&m_synCCs);

	return InsertBuyLog(_T(""));
}

bool COperatorManager::AppendBuyLog(const CString& strNewLog)
{
	CSynObject synObj(&m_synCCs);

	if (strNewLog.IsEmpty())
	{
		return true;
	}

	CString strLog = m_strBuyLog + AppendTime(strNewLog) + _T("\r\n");
	return InsertBuyLog(strLog);	
}

bool __stdcall  COperatorManager::EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec)
{
	COperatorManager* thisObj = (COperatorManager*)context;
	CString strType, strContent;
	for (vector<COLUMN_ITEM>::const_iterator it=recordVec.begin(); it!=recordVec.end(); it++)
	{
		if (it->strName == LOG_COLUMN_TYPE) 
		{
			strType = it->strValue;
		}
		else if (it->strName == LOG_COLUMN_CONTENT)
		{
			strContent = it->strValue;
		}
	}

	if (strType == LOG_TYPE_BUY)
	{
		thisObj->m_strBuyLog = strContent;
	}

	return true;
}

bool COperatorManager::InsertBuyLog(const CString& strLog)
{
	//将下注日志插入到数据库中
	CDBUtil dbUtil;
	int nResult = dbUtil.Open(CPathUtilEx::Instance()->GetImagePath()+DB_NAME);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to open database, error=%d."), nResult);
		return false;
	}

	vector<COLUMN_ITEM> columnVec;
	COLUMN_ITEM typeColumn;
	typeColumn.strName = LOG_COLUMN_TYPE;
	typeColumn.strValue = LOG_TYPE_BUY;
	columnVec.push_back(typeColumn);

	COLUMN_ITEM contentColumn;
	contentColumn.strName = LOG_COLUMN_CONTENT;
	contentColumn.strValue = strLog;
	columnVec.push_back(contentColumn);	

	nResult = dbUtil.Insert(LOG_TABLE_NAME, columnVec);
	if (nResult != 0)
	{
		theLog.LogError(_T("Failed to insert buy log to database, error=%d."), nResult);
		return false;
	}
	else
	{
		m_strBuyLog = strLog;
		return true;
	}
}

CString COperatorManager::AppendTime(const CString& strLog)
{
	CTime tm = CTime::GetCurrentTime();
	return tm.Format(_T("[%m-%d %H:%M:%S] "))+strLog;
}