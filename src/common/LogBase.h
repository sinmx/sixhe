#pragma once

#include "SynObject.h"

//��־�������ƶ���
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
	//������־����
	BOOL SetLogLevel(const CString& strLogLevel);

	//��¼��Ϣ��־
	BOOL LogInfo(TCHAR const* pFmt, ...);

	//��¼��Ϣ��־����16���Ƽ�¼����������Ϣ����
	BOOL HexPrinter(const unsigned char * unchBuf, unsigned int unBufLen);

	//��¼������־
	BOOL LogWarn(TCHAR const* pFmt, ...);

	//��¼������־
	BOOL LogError(TCHAR const* pFmt, ...);	

	//�����־
	BOOL ClearLog();	 

	//��ָ�����ڴ��ʽ����16���Ƶ��ַ���
	static CString MemoryToHexString(const unsigned char* const lpszBuf, int nBufLen);

protected:
	enum LogLevel {LOG_INFO,LOG_WARN,LOG_ERROR,LOG_NONE};

protected:
	BOOL FormatString(TCHAR const* pFmt,va_list args,CString& strFormatResult);

protected:
	virtual BOOL Log(LogLevel logLevel,const CString& strLog)=0;
	virtual BOOL Clear();
	
protected:
	//��־����
	LogLevel m_logLevel;

	//�ٽ���������ͬ���̷߳���
	CCS m_cs;
};
