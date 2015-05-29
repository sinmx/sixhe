#pragma once
#include "logbase.h"

//�ļ���־����
typedef struct _tagFileLogProperty
{
	//��־�ļ����ƣ�����ȫ·��
	CString strFileName;

	//��ʶ�Ƿ�������¼��־��TRUEʱ��־���ƽ���strFileName����������������Ϊ��־�ļ�����
	//��logfile_2011_08_16.log
	BOOL bBaseDay;

	//��־�ļ���С���ֵ(��KBΪ��λ)����bBaseDayΪFALSEʱ��ʾ��־�ļ���������������
	//��bBaseDayΪTRUEʱ������
	DWORD dwFileMaxKB;

	//��־���������������bBaseDayΪTRUEʱ��ʾ��������֮ǰ����־���������
	//��bBaseDayΪFALSEʱ������
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
	//�����ļ���־����
	BOOL SetFileLogProperty(const FILELOGPROPERTY& fileLogProperty);

protected:
	virtual BOOL Clear();
	virtual BOOL Log(LogLevel logLevel,const CString& strLog);

private:
	BOOL GetLogFileNameWithDate(const CTime& tm,CString& strFileName);
	BOOL IsFileExist(const CString& strFileName);
	void MakeDir(const CString& strPathName);		//����Ŀ¼

private:
	//�ļ���־����
	FILELOGPROPERTY m_fileLogProperty;

	//���һ�������־��ʱ��
	CTime m_lastClearTime;
};
