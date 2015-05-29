
//
//CEatCodeDlg	吃码上报窗口
//

#pragma once

#include "..\..\common\CommunicationManager.h"
#include "OrderManager.h"

class CEatCodeDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CEatCodeDlg(void);
	~CEatCodeDlg(void);

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

	//通过SendEvent发送的事件消息处理函数，其中参数同SendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//char处理函数，pSender发送通告消息的控件对象，char为输入的字符
	virtual void OnChar(DuiLib::CControlUI* pSender, TCHAR ch);

private:  //事件处理函数
	void OnAvgDiscountEditReturn();		//平均回水
	void OnAvgBettingOddsEditReturn();		//平均赔率
	void OnEatEditReturn(DuiLib::CControlUI* pControl);  //单个吃码
	void OnAvgEatEditReturn();		//平均吃码
	void OnCalcEatEditReturn();		//最多亏损多少	

	void OnCancelEatBtn();		//取消吃码
	void OnPrintSellBtn();		//打印上报	
	void OnOrderBtn();		//接单

	void OnOrderChange();		//单子发生变化

private:
	void GetLogicControlObject();		//获取逻辑控件对象
	void InitLogicControlObject();		//初始化逻辑控件对象
	void InitEat();			//初始化吃码相关控件
	void AddOneCode(DuiLib::CContainerUI* pContainer, int nCode);		//新增一个码	

private:  //逻辑控件
	DuiLib::CContainerUI* m_pNoOrderContainer;		//没有订单容器
	DuiLib::CButtonUI* m_pOrderBtn;		//接单

	DuiLib::CEditUI* m_pAvgDiscountEdit;		//平均回水
	DuiLib::CEditUI* m_pAvgBettingOddsEdit;		//平均赔率

	DuiLib::CButtonUI* m_pCancelEatBtn;		//取消吃码
	DuiLib::CButtonUI* m_pPrintSellBtn;		//打印上报
	DuiLib::CLabelUI* m_pMinMaxProfitLabel;  //最小最大利润

	DuiLib::CListUI* m_pFirstColumnList;		//第一列号码
	DuiLib::CListUI* m_pSecondColumnList;		//第二列号码
	DuiLib::CListUI* m_pThirdColumnList;		//第三列号码	

	DuiLib::CLabelUI* m_pTotalInfoLabel;	//汇总信息

	DuiLib::CLabelUI* m_pSuggestionLabel;		//建议平均吃码
	DuiLib::CEditUI* m_pAvgEatEdit;		//平均吃码

	DuiLib::CEditUI* m_pCalcEatEdit;		//亏损多少下计算吃码
	DuiLib::CLabelUI* m_pCalcAvgEatEdit;	//计算平均吃码		
};