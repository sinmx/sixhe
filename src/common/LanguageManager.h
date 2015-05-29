///////////////////////////////////////////////
//类名：CLanguageManager	语言管理器
//职责：单一实例，程序启动时加载语言，后续读取，无需同步，用于界面、消息等文字的显示，语言配置文件需为unicode格式
//////////////////////////////////////////////////////////

#pragma once

#include "SynObject.h"
#include <vector>
using namespace std;

typedef struct tagSectionUnit   //存放每节数据
{
	CString strSecName;				//节名
	CDWordArray saLanguageID;		//消息ID列表
	CStringArray saLanguageText;	//消息列表，与消息ID一一对应

	tagSectionUnit()
	{
		//
	}

	//拷贝构造函数，以便支持vector
	tagSectionUnit(const tagSectionUnit& val) : strSecName(val.strSecName)		
	{
		saLanguageID.Copy(val.saLanguageID);
		saLanguageText.Copy(val.saLanguageText);
	}

	//重载赋值运算符，以便支持vector
	tagSectionUnit& operator= (const tagSectionUnit& val)
	{
		if (this == &val)  //just me
		{
			return *this;
		}

		strSecName = val.strSecName;		
		saLanguageID.Copy(val.saLanguageID);
		saLanguageText.Copy(val.saLanguageText);
		return *this;
	}

}SECTION_UNIT, *PSECTION_UNIT;

class CLanguageManager
{
protected:  //阻止实例化
	CLanguageManager();
	virtual ~CLanguageManager();

public:
	enum eLanguage_ID
	{
		LANGUAGE_CHS_SIMPLE = 1,	// 简体中文
		LANGUAGE_ENG = 2,          // 英文
	};

	enum  //错误代号
	{
		Error_Language_Manager_Success = 0,		//成功
		Error_Language_Manager_File_Not_Found = 1,		//文件不存在
		Error_Language_Manager_File_Open_Fail = 2,		//文件打开失败
	};

public:
	//读取单一实例对象
	static CLanguageManager* Instance();

	//功能：加载语言内容
	//参数：nLanguageID		输入参数		eLanguage_ID枚举值
	//		strLanguageFile	输入参数		语言文件完整路径，空串时，简体中文为EXE所在目录下的uiCHS.ini, 其他语言为EXE所在目录下的uiENG.ini
	//返回值：0表示成功，其他值表示错误代号
	int LoadLanguage(int nLanguageID, const CString& strLanguageFile = _T(""));

	//功能：读取指定的消息内容
	//参数：id		输入参数		消息的ID
	//返回值：消息ID对应的消息内容
	CString LoadString(int id);

	//功能：读取当前使用的语言
	//返回值：eLanguage_ID枚举值
	int GetLanguageID();

	//功能：更新指定的对话框标题及控件文本
	//参数：dlg	输入参数	指定的对话框对象
	//		dlgID	输入参数	指定的对话框ID
	void UpdateDialogUIText(CDialog *dlg, UINT dlgID);

private:
	void CleanLanguage();		//清空加载的语言
	int ReadLanguage(const CString& strLanguageFile);	//加载语言	
	
private:
	int m_nLanguageID;		//当前使用的语言, eLanguage_ID枚举值
	vector<SECTION_UNIT> m_vecSecList;		//节向量
	bool m_bLoadSuccess;			//标识是否加载成功
	static CCS m_ccsSync;			//同步外部的调用
};
