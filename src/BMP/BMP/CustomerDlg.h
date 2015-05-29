
//
//CCustomerDlg	下庄用户管理窗口
//

#pragma once

#include "CustomerManager.h"

class CCustomerDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CCustomerDlg(void);
	~CCustomerDlg(void);

public:
	//获取通信唯一ID值
	virtual int GetID();

	//接收事件，wParam lParam 返回值依赖于具体的nEventCode
	virtual int AcceptEvent(int nEventCode, WPARAM wParam=0, LPARAM lParam=0);

protected:
	//窗口创建后会调用该函数，重载该函数做初始化工作
	virtual void OnInitDialog();	

	//窗口的最后一条消息即WM_NCDESTORY,
	//参数hWnd指明该窗口的句柄值，但已是无效的句柄
	virtual void OnFinalMessage(HWND hWnd);

	//单击通告消息处理函数，pSender发送通告消息的控件对象
	virtual void OnClick(DuiLib::CControlUI* pSender);

	//选择状态改变处理函数，pSender发送通告消息的控件对象
	virtual void OnSelectChange(DuiLib::CControlUI* pSender);

	//killfocus处理函数，pSender发送通告消息的控件对象
	virtual void OnKillFocus(DuiLib::CControlUI* pSender);

	//消息WM_SHOWWINDOW处理函数
	virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

private: //事件处理函数
	void OnCustomerAddBtn();			//添加用户
	void OnCustomerDeleteBtn(DuiLib::CControlUI* pDelBtn);  //删除用户
	void OnCustomerItemKillFocus(DuiLib::CControlUI* pControl);		//用户项配置变更
	void OnOrderBtn();		//接单

private:
	void GetLogicControlObject();		//获取逻辑控件对象
	void InitLogicControlObject();		//初始化逻辑控件对象
	void InitCustomerList();					//初始化用户列表

private:
	void AddOneCustomer(const CUSTOMER& customer, bool bNeedFocus);			//添加一个用户到用户列表控件中
	
private: //逻辑控件对象
	DuiLib::CContainerUI* m_pCustomerContainer;	
	DuiLib::CListUI* m_pCustomerList;
	DuiLib::CButtonUI* m_pCustomerAddBtn;
};