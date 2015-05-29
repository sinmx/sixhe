#include "stdafx.h"
#include "DBUtil.h"
#include "CharSetConversionUtil.h"

struct EnumRecordStruct
{
	PEnumRecord EnumRecord;
	LPARAM context;
};

CDBUtil::CDBUtil() :
m_pDBConnection(NULL)
{
	//
}

CDBUtil::~CDBUtil()
{
	if (m_pDBConnection)
	{
		int nRet = sqlite3_close(m_pDBConnection);
		if (nRet != SQLITE_OK)
		{
			OutputSqliteError(_T("Failed to sqlite3_close, error=%d."), nRet);			
		}
		m_pDBConnection = NULL;
	}
}

int CDBUtil::Open(const CString& strDBFile)
{
	ASSERT(m_pDBConnection == NULL);
	if (m_pDBConnection)
	{
		return ERROR_DBUtil_HasOpen;
	}

	auto_ptr<char> dbfile(ToUtf8(strDBFile));
	if (dbfile.get() == NULL)
	{
		return ERROR_DBUtil_ToUtf8;
	}

	int nRet = sqlite3_open(dbfile.get(), &m_pDBConnection);
	if (nRet != SQLITE_OK)
	{
		OutputSqliteError(_T("Failed to sqlite3_open, "), nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Close()
{
	if (m_pDBConnection == NULL) //已关闭
	{
		return ERROR_DBUtil_Success;
	}

	int nRet = sqlite3_close(m_pDBConnection);
	m_pDBConnection = NULL;
	if (nRet != SQLITE_OK)
	{
		OutputSqliteError(_T("Failed to sqlite3_close, "), nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::CreateTable(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	CString strColumnDefine;	
	for (vector<COLUMN_ITEM>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strColumnDefine += _T(", ");
		}

		strColumnDefine += (*it).strName;  //列名

		if (it->dataType == DB_DATA_TYPE_INT)
		{
			strColumnDefine += _T(" INT");
		}
		else
		{
			strColumnDefine += _T(" CHAR");
		}

		if (it->IsPrimaryKey)
		{
			strColumnDefine += _T(" PRIMARY KEY ");
		}
	}

	CString strSQL;
	strSQL.Format(_T("create table if not exists %s (%s)"), strTableName, strColumnDefine);
	auto_ptr<char> sql(ToUtf8(strSQL));
	int nRet = sqlite3_exec(m_pDBConnection, sql.get(), NULL, NULL, NULL );
	if (nRet != SQLITE_OK)
	{
		CString strTemp;
		strTemp.Format(_T("Failed to execute sql(%s), "), strSQL);
		OutputSqliteError(strTemp, nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::DropTable(const CString& strTableName)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	CString strSQL;
	strSQL.Format(_T("DROP TABLE %s"), strTableName);
	auto_ptr<char> sql(ToUtf8(strSQL));
	int nRet = sqlite3_exec(m_pDBConnection, sql.get(), NULL, NULL, NULL );
	if (nRet != SQLITE_OK)
	{
		CString strTemp;
		strTemp.Format(_T("Failed to execute sql(%s), "), strSQL);
		OutputSqliteError(strTemp, nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

CString CDBUtil::ConstructInsertSqlStatement(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	CString strColumnName;	
	CString strColumnValue;
	for (vector<COLUMN_ITEM>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strColumnName += _T(", ");
			strColumnValue += _T(", ");
		}

		strColumnName += it->strName;  //列名
		strColumnValue += _T("\"")+it->strValue+_T("\"");  //值	
	}

	CString strSQL;
	strSQL.Format(_T("insert or replace into %s (%s) values (%s)"), strTableName, strColumnName, strColumnValue);
	return strSQL;
}

int CDBUtil::Insert(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	CString strSqlStatement = ConstructInsertSqlStatement(strTableName, columnVec);
	auto_ptr<char> sql(ToUtf8(strSqlStatement));
	int nRet = sqlite3_exec(m_pDBConnection, sql.get(), NULL, NULL, NULL );
	if (nRet != SQLITE_OK)
	{
		CString strTemp;
		strTemp.Format(_T("Failed to execute sql(%s), "), strSqlStatement);
		OutputSqliteError(strTemp, nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Insert(const CString& strTableName, const vector<vector<COLUMN_ITEM>>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	CString strSqlStatement;
	for (vector<vector<COLUMN_ITEM>>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strSqlStatement += _T("; ");
		}

		strSqlStatement += ConstructInsertSqlStatement(strTableName, (*it));
	}

	auto_ptr<char> sql(ToUtf8(strSqlStatement));
	int nRet = sqlite3_exec(m_pDBConnection, sql.get(), NULL, NULL, NULL );
	if (nRet != SQLITE_OK)
	{
		CString strTemp;
		strTemp.Format(_T("Failed to execute sql(%s), "), strSqlStatement);
		OutputSqliteError(strTemp, nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Delete(const CString& strTableName, const vector<COLUMN_ITEM>& columnVec)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	CString strWhere;
	for (vector<COLUMN_ITEM>::const_iterator it=columnVec.begin(); it!=columnVec.end(); it++)
	{
		if (it != columnVec.begin())
		{
			strWhere += _T(" && ");
		}

		strWhere += it->strName+_T("=")+_T("\"")+it->strValue+_T("\"");		
	}

	CString strSQL;
	if (strWhere.IsEmpty())  //无条件
	{
		strSQL.Format(_T("delete from %s"), strTableName);
	}
	else
	{
		strSQL.Format(_T("delete from %s where %s"), strTableName, strWhere);
	}
	auto_ptr<char> sql(ToUtf8(strSQL));
	int nRet = sqlite3_exec(m_pDBConnection, sql.get(), NULL, NULL, NULL );
	if (nRet != SQLITE_OK)
	{
		CString strTemp;
		strTemp.Format(_T("Failed to execute sql(%s), "), strSQL);
		OutputSqliteError(strTemp, nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

int CDBUtil::Query(const CString& strTableName, const vector<COLUMN_ITEM>& whereVec, PEnumRecord EnumRecord, LPARAM context)
{
	if (m_pDBConnection == NULL)
	{
		return ERROR_DBUtil_HasNotOpen;
	}

	CString strWhere;
	for (vector<COLUMN_ITEM>::const_iterator it=whereVec.begin(); it!=whereVec.end(); it++)
	{
		if (it != whereVec.begin())
		{
			strWhere += _T(" && ");
		}

		strWhere += it->strName+_T("=")+_T("\"")+it->strValue+_T("\"");		
	}

	EnumRecordStruct tempStruct;
	tempStruct.EnumRecord = EnumRecord;
	tempStruct.context = context;

	CString strSQL;
	if (strWhere.IsEmpty())  //没有条件
	{
		strSQL.Format(_T("select * from %s"), strTableName);
	}
	else
	{
		strSQL.Format(_T("select * from %s where %s"), strTableName, strWhere);
	}
	auto_ptr<char> sql(ToUtf8(strSQL));
	int nRet = sqlite3_exec(m_pDBConnection, sql.get(), ExecuteCallbac, &tempStruct, NULL );
	if (nRet != SQLITE_OK)
	{
		CString strTemp;
		strTemp.Format(_T("Failed to execute sql(%s), "), strSQL);
		OutputSqliteError(strTemp, nRet);
		return ERROR_DBUtil_SQLiteLib_Base+nRet;
	}

	return ERROR_DBUtil_Success;
}

char* CDBUtil::ToUtf8(const CString& str)
{
#ifdef _UNICODE
	return CCharSetConversionUtil::WideCharToUtf8((const unsigned char*)(LPCTSTR)str, str.GetLength()*sizeof(TCHAR));
#else
	return CCharSetConversionUtil::MultiByteToUtf8(str, str.GetLength());
#endif
}

CString CDBUtil::FromUtf8(const char* szStr)
{
	CString strTemp;
#ifdef _UNICODE
	wchar_t* szUnicode = CCharSetConversionUtil::Utf8ToWideChar(szStr, (int)strlen(szStr));
	if (szUnicode)
	{
		strTemp = szUnicode;
		delete[] szUnicode;
		szUnicode = NULL;
	}
#else
	char* szMultiByte = CCharSetConversionUtil::Utf8ToMultiByte(szStr, strlen(szStr));
	if (szMultiByte)
	{
		strTemp = szMultiByte;
		delete[] szMultiByte;
		szMultiByte = NULL;
	}
#endif

	return strTemp;
}

void CDBUtil::OutputSqliteError(const CString& strPrefix, int nErrorCode)
{
	CString strErrMsg;
	const char* szErrMsg = sqlite3_errstr(nErrorCode);
	if (szErrMsg)
	{
		strErrMsg = FromUtf8(szErrMsg);		
		szErrMsg = NULL;
	}

	::OutputDebugString(strPrefix+strErrMsg+_T("\r\n"));
}

int CDBUtil::ExecuteCallbac(void* context, int nColumnCount, char** ppColumnValue, char** ppColumnName)
{
	EnumRecordStruct* pContext = (EnumRecordStruct*)context;
	if (pContext->EnumRecord == NULL)  //没有回调
	{
		return 0;  //继续
	}

	vector<COLUMN_ITEM> columnVector;
	for (int i=0; i<nColumnCount; i++)
	{
		COLUMN_ITEM item;
		item.strName = FromUtf8(ppColumnName[i]);
		item.strValue = FromUtf8(ppColumnValue[i]);
		columnVector.push_back(item);
	}

	if (pContext->EnumRecord((LPARAM)(pContext->context), columnVector)) //继续
	{
		return 0;  //继续
	}
	else
	{
		return 1;  //退出
	}
}