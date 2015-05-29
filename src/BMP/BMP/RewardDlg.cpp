#include "StdAfx.h"
#include "RewardDlg.h"
#include "CommunicationDefine.h"
#include "ConfManager.h"
#include "RewardManager.h"
#include "OrderManager.h"
#include "DuiMessageBox.h"
#include "CustomerManager.h"

CRewardDlg::CRewardDlg(void):
m_pNoOrderContainer(NULL),
m_pOrderBtn(NULL),
m_pWinCodeEdit(NULL),
m_pPrintRewardBtn(NULL),

m_pConfigureWinCodeContainer(NULL),
m_pRewardContainer(NULL),
m_pRewardList(NULL),

m_pSelfProfitContainer(NULL),
m_pSelfBuyConfContainer(NULL),
m_pSelfBuyDiscountEdit(NULL),
m_pSelfBuyBettingOddsEdit(NULL),
m_pSelfProfitLabel(NULL),

m_pDeleteAllOrderBtn(NULL)
{
}

CRewardDlg::~CRewardDlg(void)
{
}

int CRewardDlg::GetID()
{
	return COMID_RewardDlg;
}

int CRewardDlg::AcceptEvent(int nEventCode, WPARAM wParam, LPARAM lParam)
{
	return SendEvent(nEventCode, wParam, lParam);
}

void CRewardDlg::OnInitDialog()
{
	CUIDialogFrameWnd::OnInitDialog(); //call base

	//添加到通信管理器中
	CCommunicationManager::Instance()->Attach(this);

	//监听单子数据的变化
	COrderManager::Instance()->Attach(this);

	//监听用户数据的变化
	CCustomerManager::Instance()->Attach(this);

	//根据名称获取控件对象地址
	GetLogicControlObject();

	//初始化逻辑控件对象
	InitLogicControlObject();		
}

void CRewardDlg::OnFinalMessage(HWND hWnd)
{
	//从通信管理器中去除
	CCommunicationManager::Instance()->Detach(this);

	//从单子管理器监听列表中去除
	COrderManager::Instance()->Detach(this);

	//取消监听用户数据
	CCustomerManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CRewardDlg::OnPrintRewardBtn()
{
	CRewardManager::Instance()->PrintRewardTable();
}

void CRewardDlg::GetLogicControlObject()
{
	//
	//通过名称查找控件对象，采用dynamic_cast进行安全转化	
	//

	m_pNoOrderContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("noOrderContainer")));
	ASSERT(m_pNoOrderContainer);	
	m_pOrderBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("orderBtn")));
	ASSERT(m_pOrderBtn);

	m_pWinCodeEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("winCodeEdit")));
	ASSERT(m_pWinCodeEdit);
	m_pPrintRewardBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("printRewardBtn")));
	ASSERT(m_pPrintRewardBtn);

	m_pConfigureWinCodeContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("configureWinCodeContainer")));
	ASSERT(m_pConfigureWinCodeContainer);	
	m_pRewardContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("rewardContainer")));
	ASSERT(m_pRewardContainer);	
	m_pRewardList = dynamic_cast<DuiLib::CListUI*>(m_pm.FindControl(_T("rewardList")));
	ASSERT(m_pRewardList);	

	m_pSelfProfitContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("selfProfitContainer")));
	ASSERT(m_pSelfProfitContainer);	
	m_pSelfBuyConfContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("selfBuyConfContainer")));
	ASSERT(m_pSelfBuyConfContainer);	
	m_pSelfBuyDiscountEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("selfBuyDiscountEdit")));
	ASSERT(m_pSelfBuyDiscountEdit);
	m_pSelfBuyBettingOddsEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("selfBuyBettingOddsEdit")));
	ASSERT(m_pSelfBuyBettingOddsEdit);
	m_pSelfProfitLabel = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("selfProfitLabel")));
	ASSERT(m_pSelfProfitLabel);

	m_pDeleteAllOrderBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("deleteAllOrderBtn")));
	ASSERT(m_pDeleteAllOrderBtn);	
}

void CRewardDlg::InitSelfProfitControlObject()
{
	bool bAlreadyConfigureWinCode = CConfManager::Instance()->WinCode() == 0 ? false:true;	
	if (bAlreadyConfigureWinCode)
	{		
		m_pSelfBuyConfContainer->SetVisible(!COrderManager::Instance()->IsAllEat()); //都吃了也就是没有上报
		m_pSelfBuyDiscountEdit->SetText(CStringUtil::IntToStr(CConfManager::Instance()->SelfBuyDiscount()));
		m_pSelfBuyBettingOddsEdit->SetText(CStringUtil::IntToStr(CConfManager::Instance()->SelfBuyBettingOdds()));
		__int64 nSelfTotalProfit = CRewardManager::Instance()->CalcSelfProfit();
		if (nSelfTotalProfit >= 0)
		{
			//17=您自己利润
			m_pSelfProfitLabel->SetText(CLanguageManager::Instance()->LoadString(17)+CStringUtil::Int64ToStr(nSelfTotalProfit));
		}
		else
		{
			//18=您自己亏损
			m_pSelfProfitLabel->SetText(CLanguageManager::Instance()->LoadString(18)+CStringUtil::Int64ToStr(nSelfTotalProfit*(-1)));
		}
	}	
}

void CRewardDlg::InitLogicControlObject()
{
	//中奖号码
	bool bAlreadyConfigureWinCode = false;
	if (CConfManager::Instance()->WinCode() != 0)
	{
		bAlreadyConfigureWinCode = true;
		m_pWinCodeEdit->SetText(CStringUtil::IntToStr(CConfManager::Instance()->WinCode()));
	}
	else
	{
		bAlreadyConfigureWinCode = false;
		m_pWinCodeEdit->SetText(_T(""));
	}

	//打印兑奖
	m_pPrintRewardBtn->SetVisible(bAlreadyConfigureWinCode);

	//显示配置兑奖号码后的内容
	m_pConfigureWinCodeContainer->SetVisible(bAlreadyConfigureWinCode);

	//兑奖详细信息
	if (bAlreadyConfigureWinCode)
	{
		m_pRewardList->RemoveAll();
		m_pRewardContainer->SetFixedHeight(m_pRewardContainer->GetMinHeight());
		vector<REWARD_ITEM> rewardInfoVec = CRewardManager::Instance()->GetRewardInfo();
		for (vector<REWARD_ITEM>::const_iterator it=rewardInfoVec.begin(); it!=rewardInfoVec.end(); it++)
		{
			DuiLib::CDialogBuilder builder;
			DuiLib::CControlUI* pControl = builder.Create(_T("item_reward.xml"), NULL, NULL, &m_pm);
			if (pControl == NULL)
			{
				continue;
			}

			m_pRewardList->Add(pControl);
			DuiLib::CLabelUI* pName = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pControl, _T("customerLabel"));
			DuiLib::CLabelUI* pProfit = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pControl, _T("profitLabel"));
			DuiLib::CLabelUI* pBuy = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pControl, _T("buyLabel"));
			DuiLib::CLabelUI* pDiscount = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pControl, _T("discountLabel"));
			DuiLib::CLabelUI* pWin = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pControl, _T("winLabel"));
			DuiLib::CLabelUI* pBettingOdds = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pControl, _T("bettingOddsLabel"));
			DuiLib::CComboUI* pAlready = (DuiLib::CComboUI*)m_pm.FindSubControlByName(pControl, _T("alreadyRewardCombo"));
			pName->SetText(it->strCustomer);
			pProfit->SetText(CStringUtil::Int64ToStr(it->nProfit));
			pBuy->SetText(CStringUtil::Int64ToStr(it->nBuy));
			pDiscount->SetText(CStringUtil::IntToStr(it->nDiscount));
			pWin->SetText(CStringUtil::Int64ToStr(it->nWin));
			pBettingOdds->SetText(CStringUtil::IntToStr(it->nBettingOdds));
			if (it->bAlreadyReward)
			{
				pAlready->SelectItem(0);
			}
			else
			{
				pAlready->SelectItem(1);
			}
			pAlready->SetUserData(it->strCustomer);  //用户名作为自定义数据

			m_pRewardContainer->SetFixedHeight(m_pRewardContainer->GetFixedHeight()+pControl->GetFixedHeight());
		}
	}	

	//自己利润
	InitSelfProfitControlObject();

	//没有单子，显示接单页面
	m_pNoOrderContainer->SetVisible(!COrderManager::Instance()->IsHasOrder());
}

void CRewardDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理，按钮识别统一用名称比较
	//

	if (pSender == m_pPrintRewardBtn)  //打印兑奖信息
	{
		OnPrintRewardBtn();
	}
	else if (pSender == m_pOrderBtn)  //接单
	{
		OnOrderBtn();
	}
	else if (pSender == m_pDeleteAllOrderBtn)  //删除所有单子
	{
		OnDeleteAllOrderBtn();
	}
	else // call base
	{
		CUIDialogFrameWnd::OnClick(pSender);
	}	
}

void CRewardDlg::OnTextReturn(DuiLib::CControlUI* pSender)
{
	if (pSender == m_pWinCodeEdit)  //中奖号码
	{
		OnWinCodeEditTextReturn();
	}
	else if (pSender == m_pSelfBuyDiscountEdit)  //自己购买回水
	{
		OnSelfBuyDiscountEditTextReturn();
	}
	else if (pSender == m_pSelfBuyBettingOddsEdit)  //自己购买赔率
	{
		OnSelfBuyBettingOddsEditTextReturn();
	}
}

void CRewardDlg::OnItemSelect(DuiLib::CControlUI* pSender, int nCurSel, int nOldSel)
{
	if (pSender->GetName() == _T("alreadyRewardCombo"))  //是否已兑奖
	{
		OnAlreadyRewardComboItemSelectChange((DuiLib::CComboUI*)pSender);
	}
}

void CRewardDlg::OnWinCodeEditTextReturn()
{
	CConfManager::Instance()->WinCode(_ttoi(m_pWinCodeEdit->GetText().GetData()));
	InitLogicControlObject();
}

void CRewardDlg::OnSelfBuyDiscountEditTextReturn()
{
	CConfManager::Instance()->SelfBuyDiscount(_ttoi(m_pSelfBuyDiscountEdit->GetText().GetData()));
	InitSelfProfitControlObject();
}

void CRewardDlg::OnSelfBuyBettingOddsEditTextReturn()
{
	CConfManager::Instance()->SelfBuyBettingOdds(_ttoi(m_pSelfBuyBettingOddsEdit->GetText().GetData()));
	InitSelfProfitControlObject();
}

void CRewardDlg::OnAlreadyRewardComboItemSelectChange(DuiLib::CComboUI* m_pControl)
{
	if (m_pControl->GetCurSel() == 0)  //已对奖过
	{
		CRewardManager::Instance()->SaveAlreadyReward((LPCTSTR)(m_pControl->GetUserData()), true);
	}
	else if (m_pControl->GetCurSel() == 1)  //未对过奖
	{
		CRewardManager::Instance()->SaveAlreadyReward((LPCTSTR)(m_pControl->GetUserData()), false);
	}
}

void CRewardDlg::OnOrderChange()
{
	InitLogicControlObject();
}

void CRewardDlg::OnCustomerChange(int nChangeMask)
{
	if (nChangeMask == CUSTOMER_CHANGE_NEW_CUSTOMER)
	{
		//新增一个用户，肯定没有单子信息，不影响，无需更新界面
		return;
	}

	//删除用户或用户的回水赔率变更，都会影响到兑奖，重新初始化界面
	InitLogicControlObject();
}

LRESULT CRewardDlg::OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	//每个if用一个函数处理
	if (nCode == COMEVENT_OrderChange)  //单子变更
	{
		OnOrderChange();
	}
	else if (nCode == COMEVENT_CustomerChange)  //用户变更
	{
		OnCustomerChange((int)wParam);
	}

	return 0;
}

void CRewardDlg::OnOrderBtn()
{
	CCommunicationManager::Instance()->Notify(COMID_MainDlg, COMEVENT_ShowOrderPage);
}

void CRewardDlg::OnDeleteAllOrderBtn()
{
	//让用户再次确认 14=删除所有单子后，下注日志也将被清除，是否继续?
	if (DuiMessageBox(m_hWnd, CLanguageManager::Instance()->LoadString(14), CLanguageManager::Instance()->LoadString(1), MB_YESNO) == IDCANCEL)
	{
		return;
	}

	CCommunicationManager::Instance()->Notify(COMID_OrderManageDlg, COMEVENT_DeleteAllOrder);
	CCommunicationManager::Instance()->Notify(COMID_MainDlg, COMEVENT_ShowOrderPage);
}

void CRewardDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理，按钮识别统一用名称比较
	//	

	CUIDialogFrameWnd::OnSelectChange(pSender);	
}