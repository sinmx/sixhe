#include "stdafx.h"
#include "LanguageManager.h"
#include "UnicodeFile.h"
#include "PathUtilEx.h"


//���������ļ���������
#define SECTION_Message_Information			_T("MESSAGEINFO")

//staic���ݳ�Ա����
CCS CLanguageManager::m_ccsSync;

CLanguageManager::CLanguageManager() :
m_nLanguageID(LANGUAGE_CHS_SIMPLE),  //���ļ���
m_bLoadSuccess(false)
{
	//
}

CLanguageManager::~CLanguageManager()
{
}

CLanguageManager* CLanguageManager::Instance()
{
	//ͬ���ⲿ����
	CSynObject synObj(&m_ccsSync);

	static CLanguageManager thisObj;
	return &thisObj;
}

int CLanguageManager::LoadLanguage(int nLanguageID, const CString& strLanguageFile)
{
	//ͬ���ⲿ����
	CSynObject synObj(&m_ccsSync);

	//�����֮ǰ���ص�����
	CleanLanguage();

	//���δָ�������ļ�����������Ĭ��ȡΪuiCHS.ini������ȡuiENG.ini
	m_nLanguageID = nLanguageID;
	CString strIniFile = strLanguageFile;
	if (strIniFile.IsEmpty())
	{
		if (m_nLanguageID == LANGUAGE_CHS_SIMPLE)  //���ļ���
		{
			strIniFile = CPathUtilEx::Instance()->GetImagePath() + _T("uiCHS.ini");
		}
		else
		{
			strIniFile = CPathUtilEx::Instance()->GetImagePath() + _T("uiENG.ini");
		}		
	}

	if (!CPathUtilEx::IsFileExist(strIniFile))  //�ļ�������
	{
		CleanLanguage();
		return Error_Language_Manager_File_Not_Found;
	}

	//��ȡ��������
	int nResult = ReadLanguage(strIniFile);
	if (nResult != Error_Language_Manager_Success) //fail
	{
		CleanLanguage();
		return nResult;
	}
	else
	{
		m_bLoadSuccess = true;
		return Error_Language_Manager_Success;
	}
}

CString CLanguageManager::LoadString(int id)
{
	//ͬ���ⲿ����
	CSynObject synObj(&m_ccsSync);

	if (!m_bLoadSuccess)  //δ���سɹ������ؿմ�
	{
		return _T("");
	}	

	if (m_vecSecList.size() <= 0)  //û���κ�����
	{
		return _T("");
	}

	//����MESSAGEINFO����ID��Ӧ����Ϣ
	for (vector<SECTION_UNIT>::iterator it = m_vecSecList.begin(); it != m_vecSecList.end(); it++)
	{	
		if (it->strSecName.CompareNoCase(SECTION_Message_Information) != 0) //����MESSAGEINFO�ڣ�����
		{
			continue;
		}

		for (int i=0; i<it->saLanguageID.GetSize(); i++)
		{
			if (id == it->saLanguageID[i])  //�ҵ�ָ����ID��
			{
				CString strMsg = it->saLanguageText[i];
				strMsg.Replace(_T("\\r"),_T("\r"));
				strMsg.Replace(_T("\\n"),_T("\n"));
				return strMsg;
			}
		}
	}

	//���ߵ����˵��δ�ҵ������ؿմ�
	return _T("");
}

int CLanguageManager::GetLanguageID()
{
	//ͬ���ⲿ����
	CSynObject synObj(&m_ccsSync);

	return m_nLanguageID;
}

void CLanguageManager::UpdateDialogUIText(CDialog *dlg, UINT dlgID)
{
	//ͬ���ⲿ����
	CSynObject synObj(&m_ccsSync);

	//У�����
	ASSERT(dlg);
	if (dlg == NULL)
	{
		return;
	}

	if (!m_bLoadSuccess)  //δ���سɹ������ؿմ�
	{
		return;
	}	

	if (m_vecSecList.size() < 0)  //û���κ�����
	{
		return;
	}	

	//���ݶԻ���ID�ҵ�ָ���Ľڣ�������ÿؼ��ı���idΪ0��ʾ����	
	CString secName;
	secName.Format(_T("DLG%d"), dlgID);
	for (vector<SECTION_UNIT>::const_iterator itor = m_vecSecList.begin(); itor != m_vecSecList.end(); itor++)
	{			
		if(secName.CompareNoCase(itor->strSecName) != 0)  //����Ҫ�ҵĽڣ���һ��
		{
			continue;
		}
		else
		{
			for (int i=0; i<itor->saLanguageID.GetSize(); i++)
			{
				int key= itor->saLanguageID.GetAt(i);
				if (key == 0)  //����
				{
					dlg->SetWindowText(itor->saLanguageText.GetAt(i));
				}
				else //�ؼ�
				{
					CString str = itor->saLanguageText.GetAt(i);					
					str.Replace(_T("\\r\\n"), _T("\r"));
					dlg->SetDlgItemText(key, str);
				}
			}

			return;  //��������
		}		
	}

	//���ߵ����˵��δ�ҵ�
	ASSERT(FALSE);
}

void CLanguageManager::CleanLanguage()
{
	m_nLanguageID = LANGUAGE_CHS_SIMPLE;
	m_vecSecList.clear();
	m_bLoadSuccess = false;
}

int CLanguageManager::ReadLanguage(const CString& strLanguageFile)
{
	CUnicodeFile fileLan;
	if (!fileLan.Open(strLanguageFile, CFile::modeRead|CFile::shareDenyNone))
	{
		return Error_Language_Manager_File_Open_Fail;
	}	

	CString strLine=_T("");
	BOOL bRead = fileLan.ReadString(strLine);
	while(bRead)
	{
		strLine.Trim();		
		if (strLine.IsEmpty())
		{
			bRead = fileLan.ReadString(strLine);
			continue;
		}

		if (strLine[0]==_T('#'))  //��һ���ַ�Ϊ#��ʾע�ͣ�����
		{
			bRead = fileLan.ReadString(strLine);
			continue;
		}

		if (strLine[0] == _T('[') && strLine[strLine.GetLength()-1] == _T(']')) //����
		{
			strLine.Replace(_T("["), _T(""));
			strLine.Replace(_T("]"), _T(""));

			SECTION_UNIT secUnit;			
			secUnit.strSecName = strLine;

			//������ȡ�ýڵ�����
			bRead = fileLan.ReadString(strLine);
			while (bRead)
			{
				strLine.Trim();
				if (strLine.IsEmpty())
				{
					bRead = fileLan.ReadString(strLine);
					continue;
				}

				if (strLine[0] == _T('[') && strLine[strLine.GetLength()-1] == _T(']')) //�µ�һ����
				{
					break;
				}

				int nFind = strLine.Find(_T('='));
				if (nFind != -1)
				{
					CString strLeft = strLine.Left(nFind);
					CString strRight = strLine.Mid(nFind+1);					

					secUnit.saLanguageID.Add(_ttoi(strLeft));
					secUnit.saLanguageText.Add(strRight);
				}
				else  //û��=�ţ���Ϊ�Ƿ�
				{
					//
				}

				bRead = fileLan.ReadString(strLine);
			}

			m_vecSecList.push_back(secUnit);
		}
		else  //δ�ڽ����µĶ���Ϊ�Ƿ�������
		{
			bRead = fileLan.ReadString(strLine);
			continue;
		}
	}

	fileLan.Close();
	return Error_Language_Manager_Success;
}