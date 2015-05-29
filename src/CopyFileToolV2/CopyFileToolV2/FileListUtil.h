//////////////////////////////////////////////////////
//类名：CFileListUtil	文件清单工具
//职责：实现文件清单格式从V1的INI到V2的XML格式转化
////////////////////////////////////////////////////////

#pragma once

typedef struct _tagV1ToV2_Information
{
	CString strProjectName;		//项目名称
	CString strX86IniFile;		//X86 INI 文件完整路径
	CString strX64IniFile;		//X64 INI 文件完整路径
	CString strXmlFile;			//XML 文件完整路径

}V1ToV2_Information, *PV1ToV2_Information;

class CFileListUtil
{
public:
	CFileListUtil(void);
	~CFileListUtil(void);

public:
	enum
	{
		Error_File_List_Util_Success = 0,		//成功
		Error_File_List_Util_Load_Ini_File_Fail = 1,  //加载INI清单文件失败
		Error_File_List_Util_Save_Xml_File_Fail = 2,  //保存到XML清单文件失败
	};

public:
	//文件清单格式从V1的INI到V2的XML格式转化，返回值0表示成功，其他值表示错误代号
	static int ConvertIniToXml(V1ToV2_Information info);
};