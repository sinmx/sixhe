#pragma once

namespace DuiLib {

class UILIB_API CUILanguage
{
public:
	CUILanguage(void);
	~CUILanguage(void);

	//
	// 功能：加载语言文件
	// 参数：xml表示语言的xml文件
	//		  type表示资源类型，参考FindResource中的资源类型，默认为0
	//		  pManager表示该语言文件所属的绘图管理器,不能为空
	// 返回值: true表示加载成功，false表示加载失败
	//
	bool LoadLanguage(STRINGorID xml, CPaintManagerUI* pManager, LPCTSTR type=0);

protected:
	bool _Parse(CMarkupNode* pRoot, CPaintManagerUI* pManager);
protected:
	CMarkup m_xml;
	LPCTSTR m_pstrtype;
};

} // namespace DuiLib
