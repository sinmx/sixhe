#include "StdAfx.h"
#include "MainDlg.h"
#include "CommunicationDefine.h"
#include "WndContainerCallback.h"
#include "CustomerManager.h"

//������TABҳ������
#define TABINDEX_OrderManage			0x00		//���ӹ���
#define TABINDEX_EatCode				0x01		//����
#define TABINDEX_Reward					0x02		//�ҽ�
#define TABINDEX_Customer				0x03		//��ׯ�û�
#define TABINDEX_Setting				0x04		//����

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
	//ÿ���¼���һ������
	if (nEventCode == COMEVENT_ShowOrderPage)  //��ʾ���ӹ���ҳ��
	{
		OnShowOrderPage();
	}
	else if (nEventCode == COMEVENT_ShowCustomerPage)  //��ʾ��ׯ����ҳ��
	{
		OnShowCustomerPage();
	}
	else if (nEventCode == COMEVENT_ShowEatCodePage)  //��ʾ�����ϱ�ҳ��
	{
		OnShowEatCodePage();
	}
	else if (nEventCode == COMEVENT_ShowSettingPage)  //��ʾ����ҳ��
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

	//��ӵ�ͨ�Ź�������
	CCommunicationManager::Instance()->Attach(this);

	//�������ƻ�ȡ�ؼ������ַ
	GetLogicControlObject();

	//��ʼ���߼��ؼ�����
	InitLogicControlObject();		
}

void CMainDlg::OnFinalMessage(HWND hWnd)
{
	//��ͨ�Ź�������ȥ��
	CCommunicationManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CMainDlg::GetLogicControlObject()
{
	//
	//ͨ�����Ʋ��ҿؼ����󣬲���dynamic_cast���а�ȫת��	
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
	//���û���κ�һ���û�����ʾ�û�����ҳ��
	if (!CCustomerManager::Instance()->IsHasCustomer())
	{
		m_pCustomerOpt->Selected(true);
	}
}

void CMainDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ����������
	//

	CUIDialogFrameWnd::OnClick(pSender);
}

void CMainDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ����������
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
