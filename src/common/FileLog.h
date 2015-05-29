#pragma once
#include "logbase.h"

//文件日志属性
typedef struct _tagFileLogProperty
{
	//日志文件名称，包含全路径
	CString strFileName;

	//标识是否基于天记录日志，TRUE时日志名称将在strFileName基础上增加日期作为日志文件名，
	//如logfile_2011_08_16.log
	BOOL bBaseDay;

	//日志文件大小最大值(以KB为单位)，当bBaseDay为FALSE时表示日志文件保存的最大容量，
	//当bBaseDay为TRUE时无意义
	DWORD dwFileMaxKB;

	//日志保存最大天数，当bBaseDay为TRUE时表示在这天数之前的日志将被清除，
	//当bBaseDay为FALSE时无意义
	DWORD dwMaxSaveDays;

	_tagFileLogProperty()
	{
		strFileName="";
		bBaseDay=FALSE;
		dwFileMaxKB=0;
		dwMaxSaveDays=0;
	}

	_tagFileLogProperty& operator=(const _tagFileLogProperty& right)
	{
		if( this!=&right)
		{
			strFileName=right.strFileName;
			bBaseDay=right.bBaseDay;
			dwFileMaxKB=right.dwFileMaxKB;
			dwMaxSaveDays=right.dwMaxSaveDays;
		}

		return *this;
	}
}FILELOGPROPERTY,*PFILELOGPROPERTY;

class CFileLog :
	public CLogBase
{
public:
	CFileLog(void);
	~CFileLog(void);

public:
	//设置文件日志属性
	BOOL SetFileLogProperty(const FILELOGPROPERTY& fileLogProperty);

protected:
	virtual BOOL Clear();
	virtual BOOL Log(LogLevel logLevel,const CString& strLog);

private:
	BOOL GetLogFileNameWithDate(const CTime& tm,CString& strFileName);
	BOOL IsFileExist(const CString& strFileName);
	void MakeDir(const CString& strPathName);		//创建目录

private:
	//文件日志属性
	FILELOGPROPERTY m_fileLogProperty;

	//最后一次清空日志的时间
	CTime m_lastClearTime;
};
