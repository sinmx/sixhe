#include "StdAfx.h"
#include "WndContainerCallback.h"
#include "OrderManageDlg.h"
#include "EatCodeDlg.h"
#include "CustomerDlg.h"
#include "RewardDlg.h"
#include "SettingDlg.h"

CWndContainerCallback g_theWndContainerCallback;

CWndContainerCallback::CWndContainerCallback(void)
{
}

CWndContainerCallback::~CWndContainerCallback(void)
{
}

DuiLib::CUIDialogFrameWnd* CWndContainerCallback::NewWndObject(LPCTSTR pstrWndClass)
{
	if (_tcscmp(pstrWndClass, _T("orderManageContainer")) == 0)
	{
		return new COrderManageDlg;
	}
	else if (_tcscmp(pstrWndClass, _T("eatCodeContainer")) == 0)
	{
		return new CEatCodeDlg;
	}
	else if (_tcscmp(pstrWndClass, _T("rewardContainer")) == 0)
	{
		return new CRewardDlg;
	}
	else if (_tcscmp(pstrWndClass, _T("customerContainer")) == 0)
	{
		return new CCustomerDlg;
	}
	if (_tcscmp(pstrWndClass, _T("settingContainer")) == 0)
	{
		return new CSettingDlg;
	}
	else
	{
		return NULL;
	}
}