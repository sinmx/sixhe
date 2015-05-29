#include "StdAfx.h"
#include "MainDlg.h"
#include "CommunicationDefine.h"
#include "WndContainerCallback.h"
#include "CustomerManager.h"

//工具栏TAB页面索引
#define TABINDEX_OrderManage			0x00		//单子管理
#define TABINDEX_EatCode				0x01		//吃码
#define TABINDEX_Reward					0x02		//兑奖
#define TABINDEX_Customer				0x03		//下庄用户
#define TABINDEX_Setting				0x04		//设置

CMainDlg::CMainDlg(void):
m_pToolbarTab(NULL),
m_pOrderManageOpt(NULL),
m_pCustomerOpt(NULL),
m_pEatCodeOpt(NULL),
m_pSettingOpt(NULL)
{
}

CMainDlg::~CMainDlg(void)
{
}

int CMainDlg::GetID()
{
	return COMID_MainDlg;
}

int CMainDlg::AcceptEvent(int nEventCode, WPARAM wParam, LPARAM lParam)
{
	return SendEvent(nEventCode, wParam, lParam);
}

LRESULT CMainDlg::OnAcceptEvent(int nEventCode, WPARAM wParam, LPARAM lParam)
{
	//每个事件用一个函数
	if (nEventCode == COMEVENT_ShowOrderPage)  //显示单子管理页面
	{
		OnShowOrderPage();
	}
	else if (nEventCode == COMEVENT_ShowCustomerPage)  //显示下庄管理页面
	{
		OnShowCustomerPage();
	}
	else if (nEventCode == COMEVENT_ShowEatCodePage)  //显示吃吗上报页面
	{
		OnShowEatCodePage();
	}
	else if (nEventCode == COMEVENT_ShowSettingPage)  //显示设置页面
	{
		OnShowSettingPage();
	}

	return 0;
}

void CMainDlg::OnShowOrderPage()
{
	m_pOrderManageOpt->Selected(true);
}

void CMainDlg::OnShowCustomerPage()
{
	m_pCustomerOpt->Selected(true);
}

void CMainDlg::OnShowEatCodePage()
{
	m_pEatCodeOpt->Selected(true);
}

void CMainDlg::OnShowSettingPage()
{
	m_pSettingOpt->Selected(true);
}

LRESULT CMainDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//do nothing
	return 0;
}

LRESULT CMainDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 1;
}

DuiLib::IUIWndContainerCallback* CMainDlg::GetWndContainerCallback()
{	
	return &g_theWndContainerCallback;
}

void CMainDlg::OnInitDialog()
{
	CUIDialogFrameWnd::OnInitDialog(); //call base

	//添加到通信管理器中
	CCommunicationManager::Instance()->Attach(this);

	//根据名称获取控件对象地址
	GetLogicControlObject();

	//初始化逻辑控件对象
	InitLogicControlObject();		
}

void CMainDlg::OnFinalMessage(HWND hWnd)
{
	//从通信管理器中去除
	CCommunicationManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CMainDlg::GetLogicControlObject()
{
	//
	//通过名称查找控件对象，采用dynamic_cast进行安全转化	
	//
	m_pToolbarTab = dynamic_cast<DuiLib::CTabLayoutUI*>(m_pm.FindControl(_T("toolbarTL")));
	ASSERT(m_pToolbarTab);
	m_pOrderManageOpt = dynamic_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("orderManageOpt")));;
	ASSERT(m_pOrderManageOpt);
	m_pCustomerOpt = dynamic_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("customerOpt")));;
	ASSERT(m_pCustomerOpt);
	m_pEatCodeOpt = dynamic_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("eatCodeOpt")));;
	ASSERT(m_pEatCodeOpt);	
	m_pSettingOpt = dynamic_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("settingOpt")));;
	ASSERT(m_pSettingOpt);	
}

void CMainDlg::InitLogicControlObject()
{
	//如果没有任何一个用户，显示用户管理页面
	if (!CCustomerManager::Instance()->IsHasCustomer())
	{
		m_pCustomerOpt->Selected(true);
	}
}

void CMainDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//

	CUIDialogFrameWnd::OnClick(pSender);
}

void CMainDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//
	if (pSender->GetName() == _T("orderManageOpt"))
	{
		OnOrderManageOptSelChange();
	}
	else if (pSender->GetName() == _T("eatCodeOpt"))
	{
		OnEatCodeOptSelChange();
	}
	else if (pSender->GetName() == _T("rewardOpt"))
	{
		OnRewardOptSelChange();
	}
	else if (pSender->GetName() == _T("customerOpt"))
	{
		OnCustomerOptSelChange();
	}	
	else if (pSender->GetName() == _T("settingOpt"))
	{
		OnSettingOptSelChange();
	}
	else
	{
		CUIDialogFrameWnd::OnSelectChange(pSender);	
	}
}

void CMainDlg::OnOrderManageOptSelChange()
{
	m_pToolbarTab->SelectItem(TABINDEX_OrderManage);
}

void CMainDlg::OnEatCodeOptSelChange()
{
	m_pToolbarTab->SelectItem(TABINDEX_EatCode);
}

void CMainDlg::OnRewardOptSelChange()
{
	m_pToolbarTab->SelectItem(TABINDEX_Reward);
}

void CMainDlg::OnCustomerOptSelChange()
{
	m_pToolbarTab->SelectItem(TABINDEX_Customer);
}

void CMainDlg::OnSettingOptSelChange()
{
	m_pToolbarTab->SelectItem(TABINDEX_Setting);
}
