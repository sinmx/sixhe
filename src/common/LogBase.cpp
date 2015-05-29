#include "StdAfx.h"
#include "LogBase.h"

CLogBase::CLogBase(void)
{
	m_logLevel=LOG_NONE;
}

CLogBase::~CLogBase(void)
{
}

BOOL CLogBase::SetLogLevel(const CString& strLogLevel)
{
	CSynObject synObj(&m_cs);

	LogLevel logLevel=LOG_ERROR;
	if(LOGLEVELNAME_INFO==strLogLevel)
	{	
		logLevel=LOG_INFO;
	}
	else if( LOGLEVELNAME_WARN==strLogLevel)
	{
		logLevel=LOG_WARN;
	}
	else if( LOGLEVELNAME_ERROR==strLogLevel)
	{
		logLevel=LOG_ERROR;
	}
	else if( LOGLEVELNAME_NONE==strLogLevel)
	{
		logLevel=LOG_NONE;
	}
	else 
	{
		return FALSE;
	}	

	if( logLevel!=m_logLevel)
	{
		m_logLevel=logLevel;
	}	

	return TRUE;
}

BOOL CLogBase::HexPrinter(const unsigned char * unchBuf, unsigned int unBufLen)
{
	if( m_logLevel > LOG_INFO) //日志级别高于信息级，不打印
	{
		return TRUE;
	}

	int nLength = unBufLen;
	if( nLength==0 || unchBuf==NULL)
	{
		return FALSE;
	}

	CString strHexString = _T("\r\n--Hex Stream BEGIN-------------------------------------\r\n");
	CString strTmpChar = _T("");
	for(int nA=0; nA<nLength; nA++)
	{
		if( nA != 0 && nA%8==0 && nA%16!=0)
		{
			strHexString += _T("-- ");
		}

		if( nA != 0 && nA%16==0 )
		{
			strHexString += _T("\r\n");
		}

		strTmpChar.Format(_T("%0.2X"), unchBuf[nA]);
		strHexString += strTmpChar;	
		strHexString += _T(" ");
	}
	strHexString += _T("\r\n--Hex Stream E N D------------------------------------");

	CSynObject synObj(&m_cs);

	if( !Log(LOG_INFO,strHexString))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLogBase::LogInfo(TCHAR const*  pFmt, ...)
{
	CSynObject synObj(&m_cs);

	if( m_logLevel > LOG_INFO)  //日志级别高于信息级，不打印
	{
		return TRUE;
	}
	
	CString strLogInfo=_T("");
	va_list args;
	va_start(args,pFmt);
	if( !FormatString(pFmt,args,strLogInfo))
	{
		return FALSE;
	}

	if( !Log(LOG_INFO,strLogInfo))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CLogBase::LogWarn(TCHAR const* pFmt, ...)
{
	CSynObject synObj(&m_cs);

	if( m_logLevel > LOG_WARN)  //日志级别高于警告级，不打印
	{
		return TRUE;
	}

	CString strLogWarn=_T("");
	va_list args;
	va_start(args,pFmt);
	if( !FormatString(pFmt,args,strLogWarn))
	{
		return FALSE;
	}

	if( !Log(LOG_WARN,strLogWarn))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLogBase::LogError(TCHAR const* pFmt, ...)
{
	CSynObject synObj(&m_cs);

	if( m_logLevel > LOG_ERROR)  //日志级别高于错误级，不打印
	{
		return TRUE;
	}

	CString strLogError=_T("");
	va_list args;
	va_start(args,pFmt);
	if( !FormatString(pFmt,args,strLogError))
	{
		return FALSE;
	}

	if( !Log(LOG_ERROR,strLogError))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLogBase::ClearLog()
{
	CSynObject synObj(&m_cs);

	return Clear();	
}

BOOL CLogBase::Clear()
{
	//do nothing

	return TRUE;
}

BOOL CLogBase::FormatString(TCHAR const* pFmt,va_list args,CString& strFormatResult)
{	
	// 动态计算所需长度
	int len = _vsctprintf( pFmt, args )+ 1 ;
	TCHAR *pBuffer=new TCHAR[len];
	if( pBuffer==NULL)
	{
		return FALSE;
	}

	BOOL bFormatSuccess=TRUE;
	try
	{
		if( _vstprintf_s(pBuffer,len,pFmt,args)<=0)
		{
			bFormatSuccess=FALSE;
		}
	}
	catch(...)
	{
		bFormatSuccess=FALSE;
	}	

	if( bFormatSuccess)
	{
		strFormatResult=pBuffer;
	}

	if( pBuffer!=NULL)
	{
		delete []pBuffer;
		pBuffer=NULL;
	}

	return bFormatSuccess;
}

CString CLogBase::MemoryToHexString(const unsigned char* const lpszBuf, int nBufLen)
{
	CString str;
	CString strTemp;
	for (int i=0; i<nBufLen; ++i)
	{
		strTemp.Format(_T("%0.2X"), lpszBuf[i]);
		str += strTemp;
	}

	return str;
}
