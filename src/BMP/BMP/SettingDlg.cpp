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

	//��ӵ�ͨ�Ź�������
	CCommunicationManager::Instance()->Attach(this);

	//�������ƻ�ȡ�ؼ������ַ
	GetLogicControlObject();

	//��ʼ���߼��ؼ�����
	InitLogicControlObject();		
}

void CSettingDlg::OnFinalMessage(HWND hWnd)
{
	//��ͨ�Ź�������ȥ��
	CCommunicationManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CSettingDlg::GetLogicControlObject()
{
	//
	//ͨ�����Ʋ��ҿؼ����󣬲���dynamic_cast���а�ȫת��	
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
	//��ʼ������
	m_pGoldEdit->SetText(CConfManager::Instance()->GoldCode());
	m_pWoodEdit->SetText(CConfManager::Instance()->WoodCode());
	m_pWaterEdit->SetText(CConfManager::Instance()->WaterCode());
	m_pFireEdit->SetText(CConfManager::Instance()->FireCode());
	m_pEarthEdit->SetText(CConfManager::Instance()->EarthCode());

	//��ʼ������У��
	m_pSoundCheckOpt->Selected(CConfManager::Instance()->IsSoundCheck());
}

void CSettingDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ������������ťʶ��ͳһ�����ƱȽ�
	//

	CUIDialogFrameWnd::OnClick(pSender);
}

void CSettingDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ����������
	//	

	if (pSender == m_pSoundCheckOpt)  //����У��
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
	//����ÿ��if��һ����������
	//	

	if (pSender == m_pGoldEdit)  //��
	{
		OnGoldEditKillFocus();
	}
	else if (pSender == m_pWoodEdit)  //ľ
	{
		OnWoodEditKillFocus();
	}
	else if (pSender == m_pWaterEdit)  //ˮ
	{
		OnWaterEditKillFocus();
	}
	else if (pSender == m_pFireEdit)  //��
	{
		OnFireEditKillFocus();
	}
	else if (pSender == m_pEarthEdit)  //��
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
	//����ÿ��if��һ����������
	//	

	CUIDialogFrameWnd::OnTextReturn(pSender);
}

void CSettingDlg::OnTextChange(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ����������
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
	int nLastLength = (int)pEdit->GetTag();  //�ϴγ���
	if (nLength > nLastLength)  //�ַ�����
	{
		//�������ӿո�
		if (nLength >= 2 && strTemp[nLength-1] != _T(' ') && strTemp[nLength-2] != _T(' '))
		{
			strTemp += _T(' ');
			pEdit->SetText(strTemp);
		}
	}

	pEdit->SetTag(nLength);  //�ַ����ȱ����ڴ�
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

	//37=����ɹ�
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

	//37=����ɹ�
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

	//37=����ɹ�
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

	//37=����ɹ�
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

	//37=����ɹ�
	ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, m_pEarthEdit);
}

void CSettingDlg::OnSoundCheckOptSelChange()
{
	CConfManager::Instance()->SoundCheck(m_pSoundCheckOpt->IsSelected());
}

LRESULT CSettingDlg::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������ʱҪ�ÿؼ�ʧȥ���㣬�����û���Ϣ�޷���������
	m_pm.SetFocus(NULL);
	return 0;
}