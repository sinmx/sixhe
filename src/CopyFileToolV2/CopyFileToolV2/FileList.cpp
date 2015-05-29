#include "StdAfx.h"
#include "FileList.h"
#include "PathUtilEx.h"
#include "XmlDefine.h"
#include "FileListINIDefine.h"
#include "CharSetConversionUtil.h"

#pragma comment(lib, "tinyxmlSTL.lib")

/////////////////////////////////////////////////////////////////////////////
//
//

CFileListBase::CFileListBase(void)
{
}

CFileListBase::~CFileListBase(void)
{
}

int CFileListBase::Load(const void* param)
{
	//重载对象数据
	ResetObj();

	return Onload(param);	
}

int CFileListBase::Save(const CString& strFile)
{	
	return Onsave(strFile);
}

void CFileListBase::ResetObj()
{
	m_strProjectName = _T("");
	m_x86FileList.clear();
	m_x64FileList.clear();	
}


/////////////////////////////////////////////////////////////////////////
//
//

int CFileListIni::Onload(const void* param)
{
	PFILE_LIST_INI_LOAD_PARAM pLoadParam = (PFILE_LIST_INI_LOAD_PARAM)param;
	if (!CPathUtilEx::IsFileExist(pLoadParam->strX86File) && !CPathUtilEx::IsFileExist(pLoadParam->strX64File))
	{
		return Error_File_List_File_Is_Not_Exist;
	}

	//读取X86文件列表
	int nCount = GetPrivateProfileInt(SECTION_FILE_LIST, FILE_LIST_COUNT, 0, pLoadParam->strX86File);
	for (int i=0; i<nCount; i++)
	{
		CString strItem = GetPrivateProfileString(SECTION_FILE_LIST, StringAppendNumber(FILE_LIST_NAME_PREFIX, i), _T(""), pLoadParam->strX86File);
		int nIndex = strItem.Find(_T('>'));
		if (nIndex == -1)  //大于号未找到
		{
			return Error_File_List_Greater_Than_Not_Find;
		}
		else
		{
			CString strSrcFile = strItem.Left(nIndex);
			CString strDestFile = strItem.Mid(nIndex+1);
			if (strSrcFile.IsEmpty())
			{
				return Error_File_List_Source_File_Is_Empty;
			}
			else if (strDestFile.IsEmpty())
			{
				return Error_File_List_Dest_File_Is_Empty;
			}
			else
			{
				FILE_ITEM fileItem;
				fileItem.strName = CPathUtilEx::ExtractFileName(strSrcFile);
				if (fileItem.strName.IsEmpty())
				{
					return Error_File_List_Source_File_Is_Path;
				}
				else
				{
					fileItem.strSrcPath = strSrcFile;
					fileItem.strDestPath = strDestFile;
					m_x86FileList.push_back(fileItem);
				}				
			}
		}
	}

	//读取X64文件列表
	nCount = GetPrivateProfileInt(SECTION_FILE_LIST, FILE_LIST_COUNT, 0, pLoadParam->strX64File);
	for (int i=0; i<nCount; i++)
	{
		CString strItem = GetPrivateProfileString(SECTION_FILE_LIST, StringAppendNumber(FILE_LIST_NAME_PREFIX, i), _T(""), pLoadParam->strX64File);
		int nIndex = strItem.Find(_T('>'));
		if (nIndex == -1)  //大于号未找到
		{
			return Error_File_List_Greater_Than_Not_Find;
		}
		else
		{
			CString strSrcFile = strItem.Left(nIndex);
			CString strDestFile = strItem.Mid(nIndex+1);
			if (strSrcFile.IsEmpty())
			{
				return Error_File_List_Source_File_Is_Empty;
			}
			else if (strDestFile.IsEmpty())
			{
				return Error_File_List_Dest_File_Is_Empty;
			}
			else
			{
				FILE_ITEM fileItem;
				fileItem.strName = CPathUtilEx::ExtractFileName(strSrcFile);
				if (fileItem.strName.IsEmpty())
				{
					return Error_File_List_Source_File_Is_Path;
				}
				else
				{
					fileItem.strSrcPath = strSrcFile;
					fileItem.strDestPath = strDestFile;
					m_x64FileList.push_back(fileItem);
				}				
			}
		}
	}

	return Error_File_List_Success;
}

int CFileListIni::Onsave(const CString& strFile)
{
	//不会用到，无需实现，直接返回错误
	ASSERT(FALSE);
	return Error_File_List_Unknown_Reason;
}

CString CFileListIni::StringAppendNumber(const CString& strPrefix, int nNum)
{
	CString strTemp;
	strTemp.Format(_T("%s%d"), strPrefix, nNum);
	return strTemp;
}

CString CFileListIni::GetPrivateProfileString(const CString& strSection, const CString& strKey, const CString& strDefaultValue, const CString& strFile)
{
	const int nValueMaxLength = 1024;
	TCHAR szTemp[nValueMaxLength];
	::GetPrivateProfileString(strSection, strKey, strDefaultValue, szTemp, nValueMaxLength, strFile);
	return szTemp;
}


/////////////////////////////////////////////////////////////////////
//
//

int CFileListXml::Onload(const void* param)
{
	m_strFileListXML = _T("");	//上一次加载的文件路径清空
	CString strFile = (LPCTSTR)param;
	if (!CPathUtilEx::IsFileExist(strFile))
	{
		return Error_File_List_File_Is_Not_Exist;
	}

	XML_PARSER parser;
	if (!parser.Load_XML_Document(strFile))
	{
		return Error_File_List_Xml_Format_Wrong;
	}

	//项目名称
	if (parser.Go_to_Child(NODE_Project))
	{
		m_strProjectName = GetAttribute(parser, ATTRIBUTE_Project_Name);
	}
	parser.Go_to_Root();

	//X86文件列表
	if (parser.Go_to_Child(NODE_X86) && parser.Go_to_Child(NODE_ITEM))
	{
		do 
		{
			FILE_ITEM item;
			item.strName = GetAttribute(parser, ATTRIBUTE_ITEM_Name);
			item.strSrcPath = GetAttribute(parser, ATTRIBUTE_ITEM_Source_Path);
			item.strDestPath =GetAttribute(parser, ATTRIBUTE_ITEM_Dest_Path);			
			m_x86FileList.push_back(item);

		} while (parser.Go_to_NextSibling(NODE_ITEM));
	}
	parser.Go_to_Root();

	//X64文件列表
	if (parser.Go_to_Child(NODE_X64) && parser.Go_to_Child(NODE_ITEM))
	{
		do 
		{
			FILE_ITEM item;
			item.strName = GetAttribute(parser, ATTRIBUTE_ITEM_Name);
			item.strSrcPath = GetAttribute(parser, ATTRIBUTE_ITEM_Source_Path);
			item.strDestPath = GetAttribute(parser, ATTRIBUTE_ITEM_Dest_Path);			
			m_x64FileList.push_back(item);

		} while (parser.Go_to_NextSibling(NODE_ITEM));
	}
	parser.Go_to_Root();

	m_strFileListXML = strFile;		//以便保存时无需设置路径
	return Error_File_List_Success;
}

int CFileListXml::Onsave(const CString& strFileTemp)
{
	CString strFile = strFileTemp;
	if (strFile.IsEmpty())  //调用者未设置保存路径，选择上次加载的路径
	{
		strFile = m_strFileListXML;

		if (strFile.IsEmpty())  //还是空
		{
			return Error_File_List_Path_Is_NULL;
		}
	}	

	XML_PARSER parser;
	if (!parser.Load_XML_String("<?xml version=\"1.0\" encoding=\"utf-8\" ?><root version=\"2.0\"/>"))
	{
		return Error_File_List_Load_Xml_Declaration;
	}

	//项目名称
	parser.Add_LastChildElement(NODE_Project);
	SetAttribute(parser, ATTRIBUTE_Project_Name, m_strProjectName);
	parser.Go_to_Root();

	//X86节点及文件列表
	parser.Add_LastChildElement(NODE_X86);
	for (vector<FILE_ITEM>::const_iterator it=m_x86FileList.begin(); it!=m_x86FileList.end(); it++)
	{
		parser.Add_LastChildElement(NODE_ITEM);
		SetAttribute(parser, ATTRIBUTE_ITEM_Name, it->strName);
		SetAttribute(parser, ATTRIBUTE_ITEM_Source_Path, it->strSrcPath);
		SetAttribute(parser, ATTRIBUTE_ITEM_Dest_Path, it->strDestPath);
		parser.Go_to_Parent(NODE_X86);
	}
	parser.Go_to_Root();

	//X64节点及文件列表
	parser.Add_LastChildElement(NODE_X64);
	for (vector<FILE_ITEM>::const_iterator it=m_x64FileList.begin(); it!=m_x64FileList.end(); it++)
	{
		parser.Add_LastChildElement(NODE_ITEM);
		SetAttribute(parser, ATTRIBUTE_ITEM_Name, it->strName);
		SetAttribute(parser, ATTRIBUTE_ITEM_Source_Path, it->strSrcPath);
		SetAttribute(parser, ATTRIBUTE_ITEM_Dest_Path, it->strDestPath);
		parser.Go_to_Parent(NODE_X64);
	}
	parser.Go_to_Root();

	//保存到文件
	if (!parser.Save_XML_Document(strFile))
	{
		return Error_File_List_Unknown_Reason;
	}
	else
	{
		return Error_File_List_Success;
	}
}

void CFileListXml::SetAttribute(XML_PARSER& parser, const char* AttribName,const char* AttribValue)
{
	//多字节转化为UTF-8后再保存
	char* szUtf8 = CCharSetConversionUtil::MultiByteToUtf8(AttribValue, (int)strlen(AttribValue));
	if (szUtf8 == NULL)
	{
		return;
	}
	else
	{
		parser.Set_Attribute(AttribName, szUtf8);
		delete[] szUtf8;
		szUtf8 = NULL;
	}
}

CString CFileListXml::GetAttribute(XML_PARSER& parser, const char* AttribName)
{
	std::string str = parser.GetAttributeValue(AttribName);
	char* szMultiByte = CCharSetConversionUtil::Utf8ToMultiByte(str.c_str(), (int)strlen(str.c_str()));
	if (szMultiByte == NULL)
	{
		return _T("");
	}
	else
	{
		CString strTemp = szMultiByte;
		delete[] szMultiByte;
		szMultiByte = NULL;
		return strTemp;
	}
}