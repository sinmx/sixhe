#pragma once

namespace DuiLib {

class UILIB_API CUILanguage
{
public:
	CUILanguage(void);
	~CUILanguage(void);

	//
	// ���ܣ����������ļ�
	// ������xml��ʾ���Ե�xml�ļ�
	//		  type��ʾ��Դ���ͣ��ο�FindResource�е���Դ���ͣ�Ĭ��Ϊ0
	//		  pManager��ʾ�������ļ������Ļ�ͼ������,����Ϊ��
	// ����ֵ: true��ʾ���سɹ���false��ʾ����ʧ��
	//
	bool LoadLanguage(STRINGorID xml, CPaintManagerUI* pManager, LPCTSTR type=0);

protected:
	bool _Parse(CMarkupNode* pRoot, CPaintManagerUI* pManager);
protected:
	CMarkup m_xml;
	LPCTSTR m_pstrtype;
};

} // namespace DuiLib
