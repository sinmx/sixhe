#pragma once

#include "SynObject.h"

//日志级别名称定义
#define LOGLEVELNAME_INFO				_T("LogInfo")
#define LOGLEVELNAME_WARN				_T("LogWarn")
#define LOGLEVELNAME_ERROR				_T("LogError")
#define LOGLEVELNAME_NONE				_T("LogNone")

class CLogBase
{
public:
	CLogBase(void);
	~CLogBase(void);

public:
	//设置日志级别
	BOOL SetLogLevel(const CString& strLogLevel);

	//记录信息日志
	BOOL LogInfo(TCHAR const* pFmt, ...);

	//记录信息日志，以16进制记录，级别是信息级别
	BOOL HexPrinter(const unsigned char * unchBuf, unsigned int unBufLen);

	//记录警告日志
	BOOL LogWarn(TCHAR const* pFmt, ...);

	//记录错误日志
	BOOL LogError(TCHAR const* pFmt, ...);	

	//清除日志
	BOOL ClearLog();	 

	//将指定的内存格式化成16进制的字符串
	static CString MemoryToHexString(const unsigned char* const lpszBuf, int nBufLen);

protected:
	enum LogLevel {LOG_INFO,LOG_WARN,LOG_ERROR,LOG_NONE};

protected:
	BOOL FormatString(TCHAR const* pFmt,va_list args,CString& strFormatResult);

protected:
	virtual BOOL Log(LogLevel logLevel,const CString& strLog)=0;
	virtual BOOL Clear();
	
protected:
	//日志级别
	LogLevel m_logLevel;

	//临界区，用于同步线程访问
	CCS m_cs;
};
