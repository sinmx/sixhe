
//
//CWndContainerCallback	 XML��WndContainer�ڵ���󴴽��ص�
//

#pragma once

class CWndContainerCallback : public DuiLib::IUIWndContainerCallback
{
public:
	CWndContainerCallback(void);
	~CWndContainerCallback(void);

public:
	virtual DuiLib::CUIDialogFrameWnd* NewWndObject(LPCTSTR pstrWndClass);
};

extern CWndContainerCallback g_theWndContainerCallback;		//global object