/////////////////////////////////////////////////////////
//类名：CFileListBase	文件清单基类
//职责：存放文件拷贝列表信息，不能被实例化，子类CFileListIni和CFileListXml才可被实例化，分别代表着用INI和XML文件格式
//		存储的拷贝文件清单

//类名：CFileListIni	INI格式的文件清单类
//职责：根据INI文件的格式保存读取文件列表

//类名：CFileListXml	XML格式的文件清单类
//职责：根据XML文件的格式保存读取文件列表
/////////////////////////////////////////////////////////////

#pragma once

#include "XML_PARSER.h"
#include <vector>
using namespace std;

typedef struct _tagFileItem  //拷贝项，存放拷贝一个文件的信息
{
	CString strName;		//拷贝项的名称
	CString strSrcPath;		//拷贝的文件源路径
	CString strDestPath;	//拷贝的文件目的路径

	_tagFileItem()
	{
		//
	}

	//拷贝构造函数，以便支持vector
	_tagFileItem(const _tagFileItem& right)
	{
		*this = right;
	}

	//重载赋值运算符，以便支持vector
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
		Error_File_List_Success = 0,		//成功
		Error_File_List_Path_Is_NULL = 1,	//路径为空
		Error_File_List_File_Is_Not_Exist = 2,	//文件不存在
		Error_File_List_Load_Xml_Declaration = 3,	//XML加载声明失败
		Error_File_List_Unknown_Reason = 4,			//未知原因
		Error_File_List_Xml_Format_Wrong = 5,		//XML格式错误
		Error_File_List_Greater_Than_Not_Find = 6,	//INI每项中大于号未找到
		Error_File_List_Source_File_Is_Empty = 7,	//源文件为空
		Error_File_List_Dest_File_Is_Empty = 8,		//目的文件为空
		Error_File_List_Source_File_Is_Path = 9,	//源文件是路径 
	};

public:
	//属性读取设置
	void SetProjectName(const CString& strProjectName){m_strProjectName = strProjectName;}
	CString GetProjectName() const {return m_strProjectName;}
	vector<FILE_ITEM>& GetX86FileList() {return m_x86FileList;}
	vector<FILE_ITEM>& GetX64FileList() {return m_x64FileList;}

	//加载文件列表，param依赖子类的实现，XML格式只需要一个XML文件路径，INI格式需要X86和X64的文件路径
	int Load(const void* param);

	//保存文件列表，strFile如果为空，取Load时的路径
	int Save(const CString& strFile);

protected:
	//子类需根据不同的存储格式，重载实现，param同Load的param
	virtual int Onload(const void* param) = 0;

	//子类需根据不同的存储格式，重做实现
	virtual int Onsave(const CString& strFile) = 0;

protected:
	void ResetObj();		//重载对象数据

protected:
	CString m_strProjectName;		//项目名称
	vector<FILE_ITEM> m_x86FileList;	//X86拷贝文件列表
	vector<FILE_ITEM> m_x64FileList;	//X64拷贝文件列表	
};

typedef struct _tagFileListINI_Load_Param
{
	CString strX86File;		//X86文件路径
	CString strX64File;		//X64文件路径
}FILE_LIST_INI_LOAD_PARAM, *PFILE_LIST_INI_LOAD_PARAM;

class CFileListIni : public CFileListBase
{
protected:
	//根据INI存储格式，从清单加载文件列表
	virtual int Onload(const void* param);

	//根据INI存储格式，保存文件列表到清单中
	virtual int Onsave(const CString& strFile);

protected:
	CString StringAppendNumber(const CString& strPrefix, int nNum);		//字符串后面追加数字
	CString GetPrivateProfileString(const CString& strSection, const CString& strKey, const CString& strDefaultValue, const CString& strFile); //读取指定的属性值
};

class CFileListXml : public CFileListBase
{
protected:
	//根据XML存储格式，从清单加载文件列表
	virtual int Onload(const void* param);

	//根据XML存储格式，保存文件列表到清单中
	virtual int Onsave(const CString& strFile);

private:
	void SetAttribute(XML_PARSER& parser, const char* AttribName,const char* AttribValue);  //设置属性值
	CString GetAttribute(XML_PARSER& parser, const char* AttribName);  //读取属性值

protected:
	CString m_strFileListXML;		//拷贝文件清单XML文件
};