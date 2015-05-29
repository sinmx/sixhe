
//
//COrderManageDlg	单子管理窗口
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class COrderManageDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	COrderManageDlg(void);
	~COrderManageDlg(void);

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

	//通过SendEvent发送的事件消息处理函数，其中参数同SendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//选中项改变处理函数，pSender发送通告消息的控件对象
	//nCurSel 当前选中项  nOldSel 旧选中项
	virtual void OnItemSelect(DuiLib::CControlUI* pSender, int nCurSel, int nOldSel);

	//Edit文本回车处理函数，pSender发送通告消息的控件对象
	virtual void OnTextReturn(DuiLib::CControlUI* pSender);

	//文本改变处理函数，pSender发送通告消息的控件对象
	virtual void OnTextChange(DuiLib::CControlUI* pSender);

	//char处理函数，pSender发送通告消息的控件对象，char为输入的字符
	virtual void OnChar(DuiLib::CControlUI* pSender, TCHAR ch);

private:
	void GetLogicControlObject();		//获取逻辑控件对象
	void InitLogicControlObject();		//初始化逻辑控件对象
	void InitCustomer();						//初始化下庄
	void InitOrder();							//初始化单子
	DuiLib::CControlUI* AddCode(int nCode);				//添加一个号码
	DuiLib::CControlUI* FindCode(int nCode);  //在容器中查找指定的号码
	void InitBuyLog();							//初始化下注日志
	void InitGroupBuy();						//初始化分组下注
	void AddGroup(DuiLib::CContainerUI* pContainer, int nGroupID);		//增加一分组	
	void AddBuyLog(const CString& strBuyLog);  //增加购买日志

private:  //事件或消息处理函数
	void OnNewCustomer(const CString& strCustomer);		//新增一下庄
	void OnDeleteCustomer(const CString& strCustomer);		//删除一下庄
	void OnCustomerItemChange();	  //选择另一个下庄
	void OnAddCustomerBtn();			//添加下庄

	void OnDeleteOrderBtn();				//删除当前用户单子
	void OnDeleteAllOrderBtn();			//删除所有单子	
	void OnMoneyEditReturn(DuiLib::CControlUI* pMoneyEdit);		//逐个号码下注

	void OnSingleBuyEditReturn();		//单号串加注减注
	void OnSingleBuyStatus(bool bShow);	//单号串输入状态
	void OnSingleBuyEditTextChange();		//单号购买串发生变化

	void OnIntervalBuyEditReturn();		//区间段购买
	void OnEqualBuyEditReturn();			//等额购买
	void OnEqualCodeEditTextChange();  //等额购买号码
	void OnSequenceBuyEditReturn();		//顺号购买
	void OnGroupBuyEditReturn();			//分组购买

	void OnOrderChange(DWORD dwChangeMask);	//单子数据发生变更

	void OnCustomerChange(int nChangeMask, const CString& strCustomer);  //下庄用户发生变更

	void OnCodeGroupSelChange(DuiLib::COptionUI* pControl);  //分组选中状态变化	

	void OnDeleteAllOrder();		//删除所有单子

private:  //逻辑控件
	DuiLib::CContainerUI* m_pOrderContainer;		//最外层容器
	DuiLib::CComboUI* m_pCustomerCombo;		//下庄列表
	DuiLib::CButtonUI* m_pAddCustomerBtn;			//添加下庄

	DuiLib::CButtonUI* m_pDeleteOrderBtn;			//删除单子
	DuiLib::CButtonUI* m_pDeleteAllOrderBtn;		//删除所有单子	

	DuiLib::CContainerUI* m_pCodeContainer;		//号码容器
	DuiLib::CContainerUI* m_pLastCodeContainer;		//最后一个号码容器
	
	DuiLib::CLabelUI* m_pTotalInfoLabel;			//总共下注数
	DuiLib::CRichEditUI* m_pBuyLogRichedit;		//下注日志

	DuiLib::CEditUI* m_pSingleBuyEdit;		//单号下注	
	DuiLib::CLabelUI* m_pSingleBuyWrongLabel;  //单号购买串错误状态指示图

	DuiLib::CEditUI* m_pIntervalStartEdit;	//区间段购买开始号码
	DuiLib::CEditUI* m_pIntervalEndEdit;		//区间段购买结束号码
	DuiLib::CEditUI* m_pIntervalBuyEdit;		//区间段购买注数

	DuiLib::CEditUI* m_pEqualCodeEdit;		//等额购买号码
	DuiLib::CLabelUI* m_pEqualBuyWrongLabel;  //等额购买串错误状态指示图
	DuiLib::CEditUI* m_pEqualBuyEdit;		//等额购买额度

	DuiLib::CEditUI* m_pStartCodeEdit;		//开始号码
	DuiLib::CEditUI* m_pSequenceBuyEdit;		//顺号下注

	DuiLib::CContainerUI* m_pCodeGroupContainer;		//分组容器
	DuiLib::CContainerUI* m_pLastCodeGroupContainer;	//最后一分组容器
	DuiLib::CContainerUI* m_pRecentContainer;		//最近使用分组那块区域容器
	DuiLib::CContainerUI* m_pRecentGroupContainer;	//最近使用分组容器
	DuiLib::CEditUI* m_pGroupBuyEdit;		//分组购买

	DuiLib::CContainerUI* m_pNoCustomerContainer;		//没有下庄用户时显示的页面容器
	DuiLib::CButtonUI*	m_pAddCustomerBtnWhenNoCustomer;	//没有下庄用户时添加下庄
};