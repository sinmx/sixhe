/////////////////////////////////////////////////////////
//������CFileListBase	�ļ��嵥����
//ְ�𣺴���ļ������б���Ϣ�����ܱ�ʵ����������CFileListIni��CFileListXml�ſɱ�ʵ�������ֱ��������INI��XML�ļ���ʽ
//		�洢�Ŀ����ļ��嵥

//������CFileListIni	INI��ʽ���ļ��嵥��
//ְ�𣺸���INI�ļ��ĸ�ʽ�����ȡ�ļ��б�

//������CFileListXml	XML��ʽ���ļ��嵥��
//ְ�𣺸���XML�ļ��ĸ�ʽ�����ȡ�ļ��б�
/////////////////////////////////////////////////////////////

#pragma once

#include "XML_PARSER.h"
#include <vector>
using namespace std;

typedef struct _tagFileItem  //�������ſ���һ���ļ�����Ϣ
{
	CString strName;		//�����������
	CString strSrcPath;		//�������ļ�Դ·��
	CString strDestPath;	//�������ļ�Ŀ��·��

	_tagFileItem()
	{
		//
	}

	//�������캯�����Ա�֧��vector
	_tagFileItem(const _tagFileItem& right)
	{
		*this = right;
	}

	//���ظ�ֵ��������Ա�֧��vector
	_tagFileItem& operator= (const _tagFileItem& right)
	{
		if (this == &right)  //just me
		{
			return *this;
		}

		strName = right.strName;
		strSrcPath = right.strSrcPath;
		strDestPath = right.strDestPath;

		return *this;
	}
}FILE_ITEM, *PFILE_ITEM;

class CFileListBase
{
public:
	CFileListBase(void);
	~CFileListBase(void);

public:
	enum
	{
		Error_File_List_Success = 0,		//�ɹ�
		Error_File_List_Path_Is_NULL = 1,	//·��Ϊ��
		Error_File_List_File_Is_Not_Exist = 2,	//�ļ�������
		Error_File_List_Load_Xml_Declaration = 3,	//XML��������ʧ��
		Error_File_List_Unknown_Reason = 4,			//δ֪ԭ��
		Error_File_List_Xml_Format_Wrong = 5,		//XML��ʽ����
		Error_File_List_Greater_Than_Not_Find = 6,	//INIÿ���д��ں�δ�ҵ�
		Error_File_List_Source_File_Is_Empty = 7,	//Դ�ļ�Ϊ��
		Error_File_List_Dest_File_Is_Empty = 8,		//Ŀ���ļ�Ϊ��
		Error_File_List_Source_File_Is_Path = 9,	//Դ�ļ���·�� 
	};

public:
	//���Զ�ȡ����
	void SetProjectName(const CString& strProjectName){m_strProjectName = strProjectName;}
	CString GetProjectName() const {return m_strProjectName;}
	vector<FILE_ITEM>& GetX86FileList() {return m_x86FileList;}
	vector<FILE_ITEM>& GetX64FileList() {return m_x64FileList;}

	//�����ļ��б�param���������ʵ�֣�XML��ʽֻ��Ҫһ��XML�ļ�·����INI��ʽ��ҪX86��X64���ļ�·��
	int Load(const void* param);

	//�����ļ��б�strFile���Ϊ�գ�ȡLoadʱ��·��
	int Save(const CString& strFile);

protected:
	//��������ݲ�ͬ�Ĵ洢��ʽ������ʵ�֣�paramͬLoad��param
	virtual int Onload(const void* param) = 0;

	//��������ݲ�ͬ�Ĵ洢��ʽ������ʵ��
	virtual int Onsave(const CString& strFile) = 0;

protected:
	void ResetObj();		//���ض�������

protected:
	CString m_strProjectName;		//��Ŀ����
	vector<FILE_ITEM> m_x86FileList;	//X86�����ļ��б�
	vector<FILE_ITEM> m_x64FileList;	//X64�����ļ��б�	
};

typedef struct _tagFileListINI_Load_Param
{
	CString strX86File;		//X86�ļ�·��
	CString strX64File;		//X64�ļ�·��
}FILE_LIST_INI_LOAD_PARAM, *PFILE_LIST_INI_LOAD_PARAM;

class CFileListIni : public CFileListBase
{
protected:
	//����INI�洢��ʽ�����嵥�����ļ��б�
	virtual int Onload(const void* param);

	//����INI�洢��ʽ�������ļ��б��嵥��
	virtual int Onsave(const CString& strFile);

protected:
	CString StringAppendNumber(const CString& strPrefix, int nNum);		//�ַ�������׷������
	CString GetPrivateProfileString(const CString& strSection, const CString& strKey, const CString& strDefaultValue, const CString& strFile); //��ȡָ��������ֵ
};

class CFileListXml : public CFileListBase
{
protected:
	//����XML�洢��ʽ�����嵥�����ļ��б�
	virtual int Onload(const void* param);

	//����XML�洢��ʽ�������ļ��б��嵥��
	virtual int Onsave(const CString& strFile);

private:
	void SetAttribute(XML_PARSER& parser, const char* AttribName,const char* AttribValue);  //��������ֵ
	CString GetAttribute(XML_PARSER& parser, const char* AttribName);  //��ȡ����ֵ

protected:
	CString m_strFileListXML;		//�����ļ��嵥XML�ļ�
};