#include "stdafx.h"
#include "PathUtilEx.h"

CPathUtilEx::CPathUtilEx()
{
	//��ȡ���̾����ļ�����·��
	TCHAR lpszPath[MAX_PATH] = {0};
	if (GetModuleFileName(NULL, lpszPath, MAX_PATH) == 0)  //fail
	{
		m_strImagePath = _T("");
		m_strImageFile = _T("");
	}
	else
	{
		m_strImageFile = lpszPath;
		int nIndex = m_strImageFile.ReverseFind('\\');
		if (-1 == nIndex)  //δ�ҵ�
		{
			m_strImagePath = _T("");
		}
		else
		{
			m_strImagePath = m_strImageFile.Left(nIndex+1);
		}		
	}	
}

CPathUtilEx::~CPathUtilEx()
{
	//
}

CPathUtilEx* CPathUtilEx::Instance()
{
	static CPathUtilEx thisObj;
	return &thisObj;
}

BOOL CPathUtilEx::IsFileExist(const CString& strFile)
{
	CFileFind finder;
	BOOL bExist = finder.FindFile(strFile);
	finder.Close();

	return bExist;
}

BOOL CPathUtilEx::CreateFullDirectory(const CString& strTempPath)
{
	CString strPath = strTempPath;
	strPath.Replace(_T('/'), _T('\\'));
	if (strPath.IsEmpty())
	{
		return FALSE;
	}

	for (int i=0; i<strPath.GetLength(); ++i)
	{
		if (strPath[i] == _T('\\'))
		{
			CString strDir = strPath.Left(i);
			if (strDir.IsEmpty() || strDir[strDir.GetLength()-1] == _T(':'))
			{
				continue;
			}			

			if (!CreateDirectory(strDir, NULL))
			{
				if (GetLastError() != ERROR_ALREADY_EXISTS)
				{
					return FALSE;
				}
				else  //�ļ����Ѿ�������
				{
					continue;
				}
			}
			else  //�����ļ��гɹ�������
			{
				continue;
			}
		}
		else //������б����
		{
			continue;
		}
	}

	//·�����ܲ���б���߽�β�ģ����һ���ļ���ҲҪ����
	if (strPath[strPath.GetLength()-1] != _T('\\'))
	{
		if (!CreateDirectory(strPath, NULL))
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

CString CPathUtilEx::ExtractFileName(const CString& strFile)
{
	CString strTemp = strFile;
	strTemp.Replace(_T('/'), _T('\\'));
	
	int nIndex = strTemp.ReverseFind(_T('\\'));
	if (nIndex == -1)
	{
		return strTemp;
	}
	else
	{
		return strTemp.Mid(nIndex+1);
	}
}

bool CPathUtilEx::RemoveDot(CString& strPathOrFile)
{
	if (strPathOrFile.IsEmpty())
	{
		return true;
	}
	else
	{
		strPathOrFile.Replace(_T('/'), _T('\\'));
	}

	//����б�ܽ�����ÿ���ļ�������
	CStringArray strTempArray;
	int nStart = 0;
	for (int i=0; i<strPathOrFile.GetLength(); ++i)
	{
		if (strPathOrFile[i] == _T('\\'))  //�ҵ�б��
		{
			CString strTemp = strPathOrFile.Mid(nStart, i-nStart);
			if (!strTemp.IsEmpty())
			{
				strTempArray.Add(strTemp);
			}
			nStart = i+1;
		}
	}
	if (strPathOrFile[strPathOrFile.GetLength()-1] != _T('\\'))
	{
		strTempArray.Add(strPathOrFile.Mid(nStart));
	}

	//����ǵ��ֱ��ȥ��,������������ȥ����һ��Ŀ¼
	for (int i=0; i<strTempArray.GetSize(); ++i)
	{
		if (strTempArray[i] == _T('.'))
		{
			strTempArray.RemoveAt(i);
			i--;
		}
		else if (strTempArray[i] == _T(".."))
		{
			if (i == 0) //��һ����..����Ϊ�Ǵ���·��
			{
				return false;
			}
			else
			{
				int nIndex = strTempArray[i-1].Find(_T(':'));
				if (nIndex != -1)  //ǰһ���Ǹ��̷�, ��Ϊ�Ǵ���·��
				{
					return false;
				}
				else  //ȥ�������㼰ǰһ���ļ���
				{
					strTempArray.RemoveAt(i-1, 2);
					i -= 2;
				}
			}
		}
		else
		{
			continue;
		}
	}

	//�������ְ���б����������
	strPathOrFile = _T("");
	for (int i=0; i<strTempArray.GetSize(); ++i)
	{
		strPathOrFile += strTempArray[i];
		if (i < strTempArray.GetSize()-1) //���һ������б��
		{
			strPathOrFile += _T('\\');
		}		
	}

	return true;
}

CString CPathUtilEx::GetRelativePath(const CString& strMainPathOrFile2, const CString& strRelativePathOrFile2)
{	
	//ȥ�����
	CString strMainPathOrFile = strMainPathOrFile2;
	CString strRelativePathOrFile = strRelativePathOrFile2;
	if (strMainPathOrFile.IsEmpty() || strRelativePathOrFile.IsEmpty())
	{
		return _T("");
	}
	if (!RemoveDot(strMainPathOrFile))
	{
		return _T("");
	}

	if (!RemoveDot(strRelativePathOrFile))
	{
		return _T("");
	}

	if (strRelativePathOrFile.Find(strMainPathOrFile) == 0) //�����Ӵ�
	{
		return strRelativePathOrFile;
	}

	//����б�ܽ�����ÿ���ļ�������
	CStringArray strTempMainArray;
	int nStart = 0;
	for (int i=0; i<strMainPathOrFile.GetLength(); ++i)
	{
		if (strMainPathOrFile[i] == _T('\\'))  //�ҵ�б��
		{
			CString strTemp = strMainPathOrFile.Mid(nStart, i-nStart);
			if (!strTemp.IsEmpty())
			{
				strTempMainArray.Add(strTemp);
			}
			nStart = i+1;
		}
	}
	if (strMainPathOrFile[strMainPathOrFile.GetLength()-1] != _T('\\'))
	{
		strTempMainArray.Add(strMainPathOrFile.Mid(nStart));
	}

	CStringArray strTempRelativeArray;
	nStart = 0;
	for (int i=0; i<strRelativePathOrFile.GetLength(); ++i)
	{
		if (strRelativePathOrFile[i] == _T('\\'))  //�ҵ�б��
		{
			CString strTemp = strRelativePathOrFile.Mid(nStart, i-nStart);
			if (!strTemp.IsEmpty())
			{
				strTempRelativeArray.Add(strTemp);
			}
			nStart = i+1;
		}
	}
	if (strRelativePathOrFile[strRelativePathOrFile.GetLength()-1] != _T('\\'))
	{
		strTempRelativeArray.Add(strRelativePathOrFile.Mid(nStart));
	}

	//�������·��
	if (strTempMainArray.GetSize() == 0 || strTempRelativeArray.GetSize() == 0)
	{
		return strRelativePathOrFile;
	}

	if (strTempMainArray[0].CompareNoCase(strTempRelativeArray[0]) != 0)  //���̷���һ����ֱ�ӷ��ؾ���·��
	{
		return strRelativePathOrFile;
	}

	int nNotSameIndex=0;
	for (; nNotSameIndex<strTempMainArray.GetSize() && nNotSameIndex<strTempRelativeArray.GetSize(); ++nNotSameIndex)
	{
		if (strTempMainArray[nNotSameIndex].CompareNoCase(strTempRelativeArray[nNotSameIndex]) != 0)
		{
			break;
		}
	}

	int nFolderCount = (int)strTempMainArray.GetSize()-nNotSameIndex-1;
	CString strPath;
	for (int i=0; i<nFolderCount; i++)
	{
		strPath +=_T("..\\");
	}

	for (int i=nNotSameIndex; i<strTempRelativeArray.GetSize(); i++)
	{
		strPath += strTempRelativeArray[i];
		if (i < strTempRelativeArray.GetSize()-1)  //�������һ��
		{
			strPath += _T('\\');
		}
	}

	return strPath;
}