#include "stdafx.h"
#include "LanguageManager.h"
#include "UnicodeFile.h"
#include "PathUtilEx.h"


//语言配置文件节名定义
#define SECTION_Message_Information			_T("MESSAGEINFO")

//staic数据成员定义
CCS CLanguageManager::m_ccsSync;

CLanguageManager::CLanguageManager() :
m_nLanguageID(LANGUAGE_CHS_SIMPLE),  //中文简体
m_bLoadSuccess(false)
{
	//
}

CLanguageManager::~CLanguageManager()
{
}

CLanguageManager* CLanguageManager::Instance()
{
	//同步外部调用
	CSynObject synObj(&m_ccsSync);

	static CLanguageManager thisObj;
	return &thisObj;
}

int CLanguageManager::LoadLanguage(int nLanguageID, const CString& strLanguageFile)
{
	//同步外部调用
	CSynObject synObj(&m_ccsSync);

	//先清空之前加载的数据
	CleanLanguage();

	//如果未指定语言文件，简体中文默认取为uiCHS.ini，其他取uiENG.ini
	m_nLanguageID = nLanguageID;
	CString strIniFile = strLanguageFile;
	if (strIniFile.IsEmpty())
	{
		if (m_nLanguageID == LANGUAGE_CHS_SIMPLE)  //中文简体
		{
			strIniFile = CPathUtilEx::Instance()->GetImagePath() + _T("uiCHS.ini");
		}
		else
		{
			strIniFile = CPathUtilEx::Instance()->GetImagePath() + _T("uiENG.ini");
		}		
	}

	if (!CPathUtilEx::IsFileExist(strIniFile))  //文件不存在
	{
		CleanLanguage();
		return Error_Language_Manager_File_Not_Found;
	}

	//读取语言内容
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
	//同步外部调用
	CSynObject synObj(&m_ccsSync);

	if (!m_bLoadSuccess)  //未加载成功，返回空串
	{
		return _T("");
	}	

	if (m_vecSecList.size() <= 0)  //没有任何内容
	{
		return _T("");
	}

	//查找MESSAGEINFO节下ID对应的消息
	for (vector<SECTION_UNIT>::iterator it = m_vecSecList.begin(); it != m_vecSecList.end(); it++)
	{	
		if (it->strSecName.CompareNoCase(SECTION_Message_Information) != 0) //不是MESSAGEINFO节，跳过
		{
			continue;
		}

		for (int i=0; i<it->saLanguageID.GetSize(); i++)
		{
			if (id == it->saLanguageID[i])  //找到指定的ID了
			{
				CString strMsg = it->saLanguageText[i];
				strMsg.Replace(_T("\\r"),_T("\r"));
				strMsg.Replace(_T("\\n"),_T("\n"));
				return strMsg;
			}
		}
	}

	//能走到这里，说明未找到，返回空串
	return _T("");
}

int CLanguageManager::GetLanguageID()
{
	//同步外部调用
	CSynObject synObj(&m_ccsSync);

	return m_nLanguageID;
}

void CLanguageManager::UpdateDialogUIText(CDialog *dlg, UINT dlgID)
{
	//同步外部调用
	CSynObject synObj(&m_ccsSync);

	//校验参数
	ASSERT(dlg);
	if (dlg == NULL)
	{
		return;
	}

	if (!m_bLoadSuccess)  //未加载成功，返回空串
	{
		return;
	}	

	if (m_vecSecList.size() < 0)  //没有任何内容
	{
		return;
	}	

	//根据对话框ID找到指定的节，逐个设置控件文本，id为0表示标题	
	CString secName;
	secName.Format(_T("DLG%d"), dlgID);
	for (vector<SECTION_UNIT>::const_iterator itor = m_vecSecList.begin(); itor != m_vecSecList.end(); itor++)
	{			
		if(secName.CompareNoCase(itor->strSecName) != 0)  //不是要找的节，下一个
		{
			continue;
		}
		else
		{
			for (int i=0; i<itor->saLanguageID.GetSize(); i++)
			{
				int key= itor->saLanguageID.GetAt(i);
				if (key == 0)  //标题
				{
					dlg->SetWindowText(itor->saLanguageText.GetAt(i));
				}
				else //控件
				{
					CString str = itor->saLanguageText.GetAt(i);					
					str.Replace(_T("\\r\\n"), _T("\r"));
					dlg->SetDlgItemText(key, str);
				}
			}

			return;  //处理完了
		}		
	}

	//能走到这里，说明未找到
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

		if (strLine[0]==_T('#'))  //第一个字符为#表示注释，跳过
		{
			bRead = fileLan.ReadString(strLine);
			continue;
		}

		if (strLine[0] == _T('[') && strLine[strLine.GetLength()-1] == _T(']')) //节名
		{
			strLine.Replace(_T("["), _T(""));
			strLine.Replace(_T("]"), _T(""));

			SECTION_UNIT secUnit;			
			secUnit.strSecName = strLine;

			//继续读取该节的内容
			bRead = fileLan.ReadString(strLine);
			while (bRead)
			{
				strLine.Trim();
				if (strLine.IsEmpty())
				{
					bRead = fileLan.ReadString(strLine);
					continue;
				}

				if (strLine[0] == _T('[') && strLine[strLine.GetLength()-1] == _T(']')) //新的一节了
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
				else  //没有=号，认为非法
				{
					//
				}

				bRead = fileLan.ReadString(strLine);
			}

			m_vecSecList.push_back(secUnit);
		}
		else  //未在节名下的都认为非法，跳过
		{
			bRead = fileLan.ReadString(strLine);
			continue;
		}
	}

	fileLan.Close();
	return Error_Language_Manager_Success;
}