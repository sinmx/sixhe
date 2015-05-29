#include "StdAfx.h"
#include "FileLog.h"

CFileLog::CFileLog(void)
{
}

CFileLog::~CFileLog(void)
{
}

BOOL CFileLog::SetFileLogProperty(const FILELOGPROPERTY& fileLogProperty)
{
	CSynObject synObj(&m_cs);

	m_fileLogProperty=fileLogProperty;

	//�ļ�Ŀ¼���ܲ����ڣ��ڴ˴���
	int nIndex=m_fileLogProperty.strFileName.ReverseFind(_T('\\'));
	if( nIndex!=-1)
	{
		MakeDir(m_fileLogProperty.strFileName.Left(nIndex));
	}

	return TRUE;
}

BOOL CFileLog::Clear()
{
	static BOOL bFirst=TRUE;
	if( bFirst)
	{
		bFirst=FALSE;		
	}
	else
	{
		CTimeSpan tmSpan=CTime::GetCurrentTime()-m_lastClearTime;
		if( tmSpan.GetTotalMinutes()<30)		//over 30 minutes
		{
			return TRUE;	//do nothing
		}
	}

	BOOL bClearOK=TRUE;

	if( m_fileLogProperty.bBaseDay)
	{
		for( DWORD dwDay=m_fileLogProperty.dwMaxSaveDays;dwDay<m_fileLogProperty.dwMaxSaveDays+30;dwDay++)		
		{
			CTimeSpan tmSpan(dwDay,0,0,0);
			CTime tm=CTime::GetCurrentTime()-tmSpan;
			CString strLogFile=_T("");
			if( !GetLogFileNameWithDate(tm,strLogFile))
			{
				break;
			}
			CFileFind fileFind;
			BOOL bFind=fileFind.FindFile(strLogFile);
			fileFind.Close();
			if( !bFind)
			{
				break;
			}

			if( !::DeleteFile(strLogFile))
			{
				bClearOK=FALSE;
			}			
		}
	}
	else
	{
		CStdioFile file;
		if( file.Open(m_fileLogProperty.strFileName,CFile::modeRead))
		{
			BOOL bDelete=FALSE;
			if( file.GetLength()>=m_fileLogProperty.dwFileMaxKB*1024)
			{
				bDelete=TRUE;				
			}

			file.Close();

			//ֱ�ӽ��ļ�ɾ�������������ļ���СΪ0����Ϊ��unicode�µ�һ��д��־ʱ
			//��Ҫ�ڿ�ͷλ������0xfffe��ʶ���ж��Ƿ�Ϊ��һ�ε�����������ļ��Ƿ����
			if( bDelete)
			{
				DeleteFile(m_fileLogProperty.strFileName);
			}
		}
	}

	m_lastClearTime=CTime::GetCurrentTime();

	return bClearOK;
}

void CFileLog::MakeDir(const CString& strPathName)
{	
	if( strPathName.IsEmpty())
	{
		return;
	}
	
	if( !::CreateDirectory(strPathName,NULL))
	{
		//�ݹ鴴����Ŀ¼
		int nIndex=strPathName.ReverseFind(_T('\\'));
		if( nIndex!=-1)
		{
			MakeDir(strPathName.Left(nIndex));
		}

		//��������Ŀ¼
		::CreateDirectory(strPathName,NULL);
	}
}

BOOL CFileLog::Log(LogLevel logLevel,const CString& strLog)
{
	//�����־
	Clear();

	if( strLog.IsEmpty())
	{
		return TRUE;
	}

	CTime tmCurrent=CTime::GetCurrentTime();
	CString strTime=_T("");
	if( m_fileLogProperty.bBaseDay)
	{
		strTime=tmCurrent.Format(_T("[%H:%M:%S]"));
	}
	else
	{
		strTime=tmCurrent.Format(_T("[%Y-%m-%d %H:%M:%S]"));
	}

	CString strLogLevel=_T("");
	if( logLevel==LOG_ERROR)
	{
		strLogLevel=_T("[error]");
	}
	else if( logLevel==LOG_WARN)
	{
		strLogLevel=_T("[warn]");
	}
	else
	{
		strLogLevel=_T("[info]");
	}

	CString strThreadID=_T("");
	strThreadID.Format(_T("[%d]"),GetCurrentThreadId());

	CString strLogFileName=m_fileLogProperty.strFileName;
	if( m_fileLogProperty.bBaseDay)
	{
		if( !GetLogFileNameWithDate(tmCurrent,strLogFileName))
		{
			return FALSE;
		}
	}

#ifdef UNICODE
	BOOL bLogFileExist=IsFileExist(strLogFileName);
#endif
	
	BOOL bWriteOK=FALSE;
	CStdioFile file;	
	if( file.Open(strLogFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeBinary))
	{			
		file.SeekToEnd();

#ifdef UNICODE
		if( !bLogFileExist)  //����unicode�洢��ʶ
		{
			USHORT unicodeFlag=0xfeff;
			file.Write(&unicodeFlag,sizeof(USHORT));
		}
#endif	
		
		CString strInfo=strTime+strThreadID+strLogLevel+_T(":")+strLog+_T("\r\n");
		file.WriteString(strInfo);		
		file.Close();

		bWriteOK=TRUE;
	}

	return bWriteOK;
}

BOOL CFileLog::GetLogFileNameWithDate(const CTime& tm,CString& strFileName)
{
	CString strTime=tm.Format(_T("_%Y_%m_%d"));
	int nIndex=m_fileLogProperty.strFileName.ReverseFind(_T('.'));
	if( nIndex!=-1)
	{
		strFileName=m_fileLogProperty.strFileName.Left(nIndex)+strTime+m_fileLogProperty.strFileName.Mid(nIndex);
	}
	else
	{
		strFileName=m_fileLogProperty.strFileName+strTime;
	}

	return TRUE;
}

BOOL CFileLog::IsFileExist(const CString& strFileName)
{
	if( strFileName.IsEmpty())
	{
		return FALSE;
	}

	CFileFind fileFind;
	BOOL bFind=fileFind.FindFile(strFileName);
	fileFind.Close();

	return bFind;
}