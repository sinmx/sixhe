#include "StdAfx.h"
#include "SettingDlg.h"
#include "CommunicationDefine.h"
#include "ConfManager.h"

CSettingDlg::CSettingDlg(void) :
m_pGoldEdit(NULL),
m_pWoodEdit(NULL),
m_pWaterEdit(NULL),
m_pFireEdit(NULL),
m_pEarthEdit(NULL),
m_pSoundCheckOpt(NULL)
{
}

CSettingDlg::~CSettingDlg(void)
{
}

int CSettingDlg::GetID()
{
	return COMID_SettingDlg;
}

int CSettingDlg::AcceptEvent(int nEventCode, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CSettingDlg::OnInitDialog()
{
	CUIDialogFrameWnd::OnInitDialog(); //call base

	//添加到通信管理器中
	CCommunicationManager::Instance()->Attach(this);

	//根据名称获取控件对象地址
	GetLogicControlObject();

	//初始化逻辑控件对象
	InitLogicControlObject();		
}

void CSettingDlg::OnFinalMessage(HWND hWnd)
{
	//从通信管理器中去除
	CCommunicationManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CSettingDlg::GetLogicControlObject()
{
	//
	//通过名称查找控件对象，采用dynamic_cast进行安全转化	
	//

	m_pGoldEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("goldEdit")));
	ASSERT(m_pGoldEdit);
	m_pWoodEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("woodEdit")));
	ASSERT(m_pWoodEdit);
	m_pWaterEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("waterEdit")));
	ASSERT(m_pWaterEdit);
	m_pFireEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("fireEdit")));
	ASSERT(m_pFireEdit);
	m_pEarthEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("earthEdit")));
	ASSERT(m_pEarthEdit);

	m_pSoundCheckOpt = dynamic_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("soundCheckOpt")));
	ASSERT(m_pSoundCheckOpt);
}

void CSettingDlg::InitLogicControlObject()
{
	//初始化五行
	m_pGoldEdit->SetText(CConfManager::Instance()->GoldCode());
	m_pWoodEdit->SetText(CConfManager::Instance()->WoodCode());
	m_pWaterEdit->SetText(CConfManager::Instance()->WaterCode());
	m_pFireEdit->SetText(CConfManager::Instance()->FireCode());
	m_pEarthEdit->SetText(CConfManager::Instance()->EarthCode());

	//初始化声音校对
	m_pSoundCheckOpt->Selected(CConfManager::Instance()->IsSoundCheck());
}

void CSettingDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理，按钮识别统一用名称比较
	//

	CUIDialogFrameWnd::OnClick(pSender);
}

void CSettingDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//	

	if (pSender == m_pSoundCheckOpt)  //声音校对
	{
		OnSoundCheckOptSelChange();
	}
	else  //call base
	{
		CUIDialogFrameWnd::OnSelectChange(pSender);	
	}
}

void CSettingDlg::OnKillFocus(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//	

	if (pSender == m_pGoldEdit)  //金
	{
		OnGoldEditKillFocus();
	}
	else if (pSender == m_pWoodEdit)  //木
	{
		OnWoodEditKillFocus();
	}
	else if (pSender == m_pWaterEdit)  //水
	{
		OnWaterEditKillFocus();
	}
	else if (pSender == m_pFireEdit)  //火
	{
		OnFireEditKillFocus();
	}
	else if (pSender == m_pEarthEdit)  //土
	{
		OnEarthEditKillFocus();
	}
	else
	{
		CUIDialogFrameWnd::OnKillFocus(pSender);
	}
}

void CSettingDlg::OnTextReturn(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//	

	CUIDialogFrameWnd::OnTextReturn(pSender);
}

void CSettingDlg::OnTextChange(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//	

	if (pSender->GetUserData() == _T("wuhang"))
	{
		OnWuHangTextChange(pSender);
	}
}

void CSettingDlg::OnWuHangTextChange(DuiLib::CControlUI* pEdit)
{
	CString strTemp = pEdit->GetText().GetData();
	int nLength = strTemp.GetLength();
	int nLastLength = (int)pEdit->GetTag();  //上次长度
	if (nLength > nLastLength)  //字符增加
	{
		//智能增加空格
		if (nLength >= 2 && strTemp[nLength-1] != _T(' ') && strTemp[nLength-2] != _T(' '))
		{
			strTemp += _T(' ');
			pEdit->SetText(strTemp);
		}
	}

	pEdit->SetTag(nLength);  //字符长度保存在此
}

void CSettingDlg::OnGoldEditKillFocus()
{
	if (CConfManager::Instance()->GoldCode() == m_pGoldEdit->GetText() .GetData())
	{
		return;
	}

	CConfManager::Instance()->GoldCode(m_pGoldEdit->GetText().GetData());
	if (!CConfManager::Instance()->IsWuHangDefaultValue())
	{
		CConfManager::Instance()->NeedHintConfigureWuHang(false);
	}

	//37=保存成功
	ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, m_pGoldEdit);
}

void CSettingDlg::OnWoodEditKillFocus()
{
	if (CConfManager::Instance()->WoodCode() == m_pWoodEdit->GetText().GetData())
	{
		return;
	}

	CConfManager::Instance()->WoodCode(m_pWoodEdit->GetText().GetData());
	if (!CConfManager::Instance()->IsWuHangDefaultValue())
	{
		CConfManager::Instance()->NeedHintConfigureWuHang(false);
	}

	//37=保存成功
	ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, m_pWoodEdit);
}

void CSettingDlg::OnWaterEditKillFocus()
{
	if (CConfManager::Instance()->WaterCode() == m_pWaterEdit->GetText().GetData())
	{
		return;
	}

	CConfManager::Instance()->WaterCode(m_pWaterEdit->GetText().GetData());
	if (!CConfManager::Instance()->IsWuHangDefaultValue())
	{
		CConfManager::Instance()->NeedHintConfigureWuHang(false);
	}

	//37=保存成功
	ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, m_pWaterEdit);
}

void CSettingDlg::OnFireEditKillFocus()
{
	if (CConfManager::Instance()->FireCode() == m_pFireEdit->GetText().GetData())
	{
		return;
	}

	CConfManager::Instance()->FireCode(m_pFireEdit->GetText().GetData());
	if (!CConfManager::Instance()->IsWuHangDefaultValue())
	{
		CConfManager::Instance()->NeedHintConfigureWuHang(false);
	}

	//37=保存成功
	ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, m_pFireEdit);
}

void CSettingDlg::OnEarthEditKillFocus()
{
	if (CConfManager::Instance()->EarthCode() == m_pEarthEdit->GetText().GetData())
	{
		return;
	}

	CConfManager::Instance()->EarthCode(m_pEarthEdit->GetText().GetData());
	if (!CConfManager::Instance()->IsWuHangDefaultValue())
	{
		CConfManager::Instance()->NeedHintConfigureWuHang(false);
	}

	//37=保存成功
	ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, m_pEarthEdit);
}

void CSettingDlg::OnSoundCheckOptSelChange()
{
	CConfManager::Instance()->SoundCheck(m_pSoundCheckOpt->IsSelected());
}

LRESULT CSettingDlg::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//窗口隐藏时要让控件失去焦点，否则用户信息无法正常保存
	m_pm.SetFocus(NULL);
	return 0;
}