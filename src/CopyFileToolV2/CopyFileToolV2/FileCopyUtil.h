/////////////////////////////////////////////////////
//������CFileCopyUtil	�ļ�����������
//ְ�𣺸����ļ�������Ϣ�����ļ�
//////////////////////////////////////////////////////////

#pragma once

#include "FileList.h"

typedef struct _tagCopyResultInformation  //�������
{
	int nResult;		//�����eCopy_Resultö��ֵ��0��ʾ�ɹ�������ֵ��ʾ�������
	vector<FILE_ITEM> newFileList; //�����ļ��б�
	FILE_ITEM failFileItem;		//ʧ�ܿ������ļ���

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
		Error_File_Copy_Success = 0,  //�ɹ�
		Error_File_Copy_Unknown_Reason = 1,  //δ֪ԭ��
	};

public:
	//�����ļ������嵥�������ļ�
	static void Copy(CFileListBase& fileList, COPY_RESULT_INFORMATION& result);
};
