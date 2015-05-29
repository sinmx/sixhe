
//
//CMainDlg	最外层的主对话框
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class CMainDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CMainDlg(void);
	~CMainDlg(void);

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

	//消息WM_DESTROY处理函数
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//通过SendEvent发送的事件消息处理函数，其中参数同SendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//自定义消息处理，所有发到本窗口的自定义消息都在该函数中处理，
	//消息已经处理设置bHandled为TRUE，将不再处理
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//获取生成容器窗口对象的回调，有容器窗口时，进行重载
	virtual DuiLib::IUIWndContainerCallback* GetWndContainerCallback();

protected:  //消息处理函数

	//工具栏上的按钮选中状态发生变化
	void OnOrderManageOptSelChange();
	void OnEatCodeOptSelChange();
	void OnRewardOptSelChange();
	void OnCustomerOptSelChange();	
	void OnSettingOptSelChange();

	void OnShowOrderPage();		//显示单子管理页面
	void OnShowCustomerPage();	//显示下庄管理页面
	void OnShowEatCodePage();		//显示吃码上报页面
	void OnShowSettingPage();		//显示设置页面

private:
	void GetLogicControlObject();		//获取逻辑控件对象
	void InitLogicControlObject();		//初始化逻辑控件对象

private:
	DuiLib::CTabLayoutUI* m_pToolbarTab;		//工具栏
	DuiLib::COptionUI* m_pOrderManageOpt;	//单子管理按钮
	DuiLib::COptionUI* m_pCustomerOpt;			//下庄管理按钮
	DuiLib::COptionUI* m_pEatCodeOpt;			//吃码上报按钮
	DuiLib::COptionUI* m_pSettingOpt;				//设置按钮
};