
////////////////////////////////////////////////////////////////////
//类名：CUIWndContainer 窗口容器类
//功能：在逻辑容器的位置上创建一个子窗口，子窗口的位置、是否显示等属性依赖于逻辑容器的属性
/////////////////////////////////////////////////////////////////////////

#pragma once

namespace DuiLib 
{
	class CUIDialogFrameWnd;
	class IUIWndContainerCallback
	{
	public:
		virtual CUIDialogFrameWnd* NewWndObject(LPCTSTR pstrWndClass) = 0;
	};

	class UILIB_API CUIWndContainer : public CContainerUI
	{
	public:
		CUIWndContainer(IUIWndContainerCallback* pCallback);
		virtual ~CUIWndContainer(void);

	public:		//容器属性的设置
		void SetPos(RECT rc);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetXMLFile(LPCTSTR pstrValue);
		void SetWndClass(LPCTSTR pstrValue);
		CUIDialogFrameWnd* GetPhysicalWndObjectPtr() {return m_pWnd;}

	protected:
		IUIWndContainerCallback* m_pCallback;	//回调
		CUIDialogFrameWnd* m_pWnd;			//关联的窗口对象
		CStdString m_strXMLFile;			//关联的窗口加载的xml文件
		CStdString m_strWndClassName;		//窗口类名称
	};
}  // namespace DuiLib