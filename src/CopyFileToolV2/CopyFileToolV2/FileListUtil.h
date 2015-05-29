//////////////////////////////////////////////////////
//������CFileListUtil	�ļ��嵥����
//ְ��ʵ���ļ��嵥��ʽ��V1��INI��V2��XML��ʽת��
////////////////////////////////////////////////////////

#pragma once

typedef struct _tagV1ToV2_Information
{
	CString strProjectName;		//��Ŀ����
	CString strX86IniFile;		//X86 INI �ļ�����·��
	CString strX64IniFile;		//X64 INI �ļ�����·��
	CString strXmlFile;			//XML �ļ�����·��

}V1ToV2_Information, *PV1ToV2_Information;

class CFileListUtil
{
public:
	CFileListUtil(void);
	~CFileListUtil(void);

public:
	enum
	{
		Error_File_List_Util_Success = 0,		//�ɹ�
		Error_File_List_Util_Load_Ini_File_Fail = 1,  //����INI�嵥�ļ�ʧ��
		Error_File_List_Util_Save_Xml_File_Fail = 2,  //���浽XML�嵥�ļ�ʧ��
	};

public:
	//�ļ��嵥��ʽ��V1��INI��V2��XML��ʽת��������ֵ0��ʾ�ɹ�������ֵ��ʾ�������
	static int ConvertIniToXml(V1ToV2_Information info);
};