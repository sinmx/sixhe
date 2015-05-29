#include "StdAfx.h"
#include "FileCopyUtil.h"
#include "PathUtilEx.h"
#include <shlwapi.h>

#pragma comment (lib, "shlwapi.lib")

CFileCopyUtil::CFileCopyUtil(void)
{
}

CFileCopyUtil::~CFileCopyUtil(void)
{
}

void CFileCopyUtil::Copy(CFileListBase& fileList, COPY_RESULT_INFORMATION& result)
{
	//����X86�ļ�
	vector<FILE_ITEM>& vectorX86 = fileList.GetX86FileList();
	for (vector<FILE_ITEM>::const_iterator it=vectorX86.begin(); it!=vectorX86.end(); ++it)
	{
		CString strSrcPath = it->strSrcPath;
		CString strDestPath = it->strDestPath;
		if (PathIsRelative(strSrcPath))
		{
			strSrcPath = CPathUtilEx::Instance()->GetImagePath()+strSrcPath;
		}
		if (PathIsRelative(strDestPath))
		{
			strDestPath = CPathUtilEx::Instance()->GetImagePath()+strDestPath;
		}
		BOOL bExist = CPathUtilEx::IsFileExist(strDestPath);
		if (!::CopyFile(strSrcPath, strDestPath, FALSE))
		{
			result.nResult = Error_File_Copy_Unknown_Reason;
			result.failFileItem = *it;
			return;
		}
		else
		{
			if (!bExist)  //�����ļ�
			{
				result.newFileList.push_back(*it);
			}
		}
	}

	//����X64�ļ�
	vector<FILE_ITEM>& vectorX64 = fileList.GetX64FileList();
	for (vector<FILE_ITEM>::const_iterator it=vectorX64.begin(); it!=vectorX64.end(); ++it)
	{
		CString strSrcPath = it->strSrcPath;
		CString strDestPath = it->strDestPath;
		if (PathIsRelative(strSrcPath))
		{
			strSrcPath = CPathUtilEx::Instance()->GetImagePath()+strSrcPath;
		}
		if (PathIsRelative(strDestPath))
		{
			strDestPath = CPathUtilEx::Instance()->GetImagePath()+strDestPath;
		}
		BOOL bExist = CPathUtilEx::IsFileExist(strDestPath);
		if (!::CopyFile(strSrcPath, strDestPath, FALSE))
		{
			result.nResult = Error_File_Copy_Unknown_Reason;
			result.failFileItem = *it;
			return;
		}
		else
		{
			if (!bExist)  //�����ļ�
			{
				result.newFileList.push_back(*it);
			}
		}
	}
}