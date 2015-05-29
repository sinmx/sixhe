#pragma once

//确定或是时返回IDOK，取消或否时返回IDCANCEL
UINT DuiMessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType );

class CDuiMessageBoxDlg :	public DuiLib::CUIDialogFrameWnd
{
public:
	CDuiMessageBoxDlg(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
	~CDuiMessageBoxDlg(void);

protected:
	//窗口创建后会调用该函数，重载该函数做初始化工作
	virtual void OnInitDialog();

	//单击通告消息处理函数，pSender发送通告消息的控件对象
	virtual void OnClick(DuiLib::CControlUI* pSender);

private:
	void GetLoginControlObject();		//获取逻辑控件对象
	int GetTextHeightByWidth(DuiLib::CStdString strText,int nWidth,int iFont);
	void InitMessageType(UINT uType);

private:  //逻辑控件
	DuiLib::CTextUI* m_pMsgText;
	DuiLib::CButtonUI* m_pOKBtn;
	DuiLib::CButtonUI* m_pCancelBtn;
	DuiLib::CControlUI* m_pSpaceCtrl;
	DuiLib::CControlUI* m_pIconCtrl;

private:
	DuiLib::CStdString m_strText;
	DuiLib::CStdString m_strCaption;
	UINT m_uType;
	bool m_bIsChineseOS;		//标志是否为中文操作系统
};
