
//
//CRewardDlg	兑奖管理窗口
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class CRewardDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CRewardDlg(void);
	~CRewardDlg(void);

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

	//选中项改变处理函数，pSender发送通告消息的控件对象
	//nCurSel 当前选中项  nOldSel 旧选中项
	virtual void OnItemSelect(DuiLib::CControlUI* pSender, int nCurSel, int nOldSel);

	//通过SendEvent发送的事件消息处理函数，其中参数同SendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

private:  //事件处理函数
	void OnPrintRewardBtn();  //打印兑奖信息
	void OnOrderBtn();   //接单
	void OnDeleteAllOrderBtn();  //删除所有单子

	void OnWinCodeEditTextReturn();  //中奖号码
	void OnSelfBuyDiscountEditTextReturn();  //自己购买回水
	void OnSelfBuyBettingOddsEditTextReturn();  //自己购买赔率

	void OnAlreadyRewardComboItemSelectChange(DuiLib::CComboUI* m_pControl);  //是否已经兑奖变化

	void OnOrderChange();		//单子发生变化

	void OnCustomerChange(int nChangeMask);  //用户发生变化

private:
	void GetLogicControlObject();		//获取逻辑控件对象
	void InitLogicControlObject();		//初始化逻辑控件对象
	void InitSelfProfitControlObject();		//初始化自己利润控件

private:  //逻辑控件对象
	DuiLib::CContainerUI* m_pNoOrderContainer;		//没单子时的界面
	DuiLib::CButtonUI* m_pOrderBtn;		//接单

	DuiLib::CEditUI* m_pWinCodeEdit;		//开奖特码
	DuiLib::CButtonUI* m_pPrintRewardBtn;	//打印兑奖

	DuiLib::CContainerUI* m_pConfigureWinCodeContainer;  //配置兑奖号码后的显示内容
	DuiLib::CContainerUI* m_pRewardContainer;	//兑奖列表容器
	DuiLib::CListUI* m_pRewardList;			//兑奖详细信息表

	DuiLib::CContainerUI* m_pSelfProfitContainer;		//自己利润容器
	DuiLib::CContainerUI* m_pSelfBuyConfContainer;		//自己购买配置容器
	DuiLib::CEditUI* m_pSelfBuyDiscountEdit;		//自己购买回水
	DuiLib::CEditUI* m_pSelfBuyBettingOddsEdit;	//自己购买赔率
	DuiLib::CLabelUI* m_pSelfProfitLabel;	//自己利润

	DuiLib::CButtonUI* m_pDeleteAllOrderBtn;		//删除所有单子
};