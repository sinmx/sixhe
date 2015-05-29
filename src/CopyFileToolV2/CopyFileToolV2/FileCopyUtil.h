/////////////////////////////////////////////////////
//类名：CFileCopyUtil	文件拷贝工具类
//职责：根据文件拷贝信息拷贝文件
//////////////////////////////////////////////////////////

#pragma once

#include "FileList.h"

typedef struct _tagCopyResultInformation  //拷贝结果
{
	int nResult;		//结果，eCopy_Result枚举值，0表示成功，其他值表示错误代号
	vector<FILE_ITEM> newFileList; //新增文件列表
	FILE_ITEM failFileItem;		//失败拷贝的文件项

	_tagCopyResultInformation()
	{
		nResult = 0;
	}
}COPY_RESULT_INFORMATION, *PCOPY_RESULT_INFORMATION;

class CFileCopyUtil
{
public:
	CFileCopyUtil(void);
	~CFileCopyUtil(void);

public:
	enum eCopy_Result
	{
		Error_File_Copy_Success = 0,  //成功
		Error_File_Copy_Unknown_Reason = 1,  //未知原因
	};

public:
	//根据文件拷贝清单，拷贝文件
	static void Copy(CFileListBase& fileList, COPY_RESULT_INFORMATION& result);
};
