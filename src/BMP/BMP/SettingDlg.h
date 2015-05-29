
//
//CSettingDlg	设置窗口对象
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class CSettingDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CSettingDlg(void);
	~CSettingDlg(void);

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

	//Edit文本回车处理函数，pSender发送通告消息的控件对象
	virtual void OnTextReturn(DuiLib::CControlUI* pSender);

	//文本改变处理函数，pSender发送通告消息的控件对象
	virtual void OnTextChange(DuiLib::CControlUI* pSender);

	//killfocus处理函数，pSender发送通告消息的控件对象
	virtual void OnKillFocus(DuiLib::CControlUI* pSender);

	//消息WM_SHOWWINDOW处理函数
	virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:  //事件处理函数
	void OnGoldEditKillFocus();  //五行
	void OnWoodEditKillFocus();
	void OnWaterEditKillFocus();
	void OnFireEditKillFocus();
	void OnEarthEditKillFocus();

	void OnWuHangTextChange(DuiLib::CControlUI* pEdit);  //五行内容发生变化

	void OnSoundCheckOptSelChange();		//声音校对

private:
	void GetLogicControlObject();		//获取逻辑控件对象
	void InitLogicControlObject();		//初始化逻辑控件对象

private:  //逻辑控件
	DuiLib::CEditUI* m_pGoldEdit;		//金
	DuiLib::CEditUI* m_pWoodEdit;	//木
	DuiLib::CEditUI* m_pWaterEdit;  //水
	DuiLib::CEditUI* m_pFireEdit;  //火
	DuiLib::CEditUI* m_pEarthEdit;  //土
	DuiLib::COptionUI* m_pSoundCheckOpt;  //声音校对
};