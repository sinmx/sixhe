#include "StdAfx.h"
#include "FileListUtil.h"
#include "FileList.h"

CFileListUtil::CFileListUtil(void)
{
}

CFileListUtil::~CFileListUtil(void)
{
}

int CFileListUtil::ConvertIniToXml(V1ToV2_Information info)
{
	CFileListIni iniFileList;
	FILE_LIST_INI_LOAD_PARAM loadParam;
	loadParam.strX86File = info.strX86IniFile;
	loadParam.strX64File = info.strX64IniFile;	
	if (iniFileList.Load((void*)&loadParam) != 0)
	{
		return Error_File_List_Util_Load_Ini_File_Fail;
	}

	CFileListXml xmlFileList;
	xmlFileList.SetProjectName(info.strProjectName);
	xmlFileList.GetX86FileList().assign(iniFileList.GetX86FileList().begin(), iniFileList.GetX86FileList().end());
	xmlFileList.GetX64FileList().assign(iniFileList.GetX64FileList().begin(), iniFileList.GetX64FileList().end());
	if (xmlFileList.Save(info.strXmlFile) != 0)
	{
		return Error_File_List_Util_Save_Xml_File_Fail;
	}

	return Error_File_List_Util_Success;
}