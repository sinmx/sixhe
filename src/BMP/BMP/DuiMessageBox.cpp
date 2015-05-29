#include "StdAfx.h"
#include "DuiMessageBox.h"

#define MESSAGEBOX_LEFT_RIGHT_SPACE 40
#define MESSAGEBOX_TOP_BUTTOM_SPACE 110

UINT DuiMessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType )
{
	CDuiMessageBoxDlg dlg(lpText,lpCaption,uType);
	if (dlg.Create(_T("messagebox.xml"), hWnd) == NULL)
	{
		return 0;
	}

	return dlg.ShowModal();		
}

CDuiMessageBoxDlg::CDuiMessageBoxDlg(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType )
{
	m_strText = lpText;
	m_strCaption = lpCaption;
	m_uType = uType;

	if (PRIMARYLANGID(::GetSystemDefaultLangID()) == LANG_CHINESE && SUBLANGID(::GetSystemDefaultLangID())==SUBLANG_CHINESE_SIMPLIFIED)
	{
		m_bIsChineseOS = true;
	}
	else
	{
		m_bIsChineseOS = false;
	}
}

CDuiMessageBoxDlg::~CDuiMessageBoxDlg(void)
{
}


int CDuiMessageBoxDlg::GetTextHeightByWidth(DuiLib::CStdString strText,int nWidth,int iFont)
{
	RECT rcText;
	memset(&rcText,0,sizeof(rcText));
	rcText.right = nWidth;
	UINT uStyle = DT_CALCRECT|DT_WORDBREAK|DT_LEFT|DT_EDITCONTROL;
	DuiLib::CRenderEngine::DrawText(m_pm.GetPaintDC(), &m_pm, rcText, strText, 0, -1, uStyle );	

	int iHeight = m_pm.GetFontInfo(iFont)->tm.tmHeight;	// 字体高度	
	int rcHeight = ((rcText.bottom-rcText.top)/iHeight)*(iHeight+4);//计算高度，添加行距
	return rcHeight;
}

void CDuiMessageBoxDlg::OnInitDialog()
{
	GetLoginControlObject();
	SetWindowText(m_strCaption);

	// 计算消息对话框大小
	RECT rc;
	::GetWindowRect(m_hWnd,&rc);
	int height = GetTextHeightByWidth(m_strText,(rc.right-rc.left-MESSAGEBOX_LEFT_RIGHT_SPACE),m_pMsgText->GetFont());
	if (height > m_pIconCtrl->GetFixedHeight())
	{
		m_pMsgText->SetFixedHeight(height);
		::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right-rc.left, height+MESSAGEBOX_TOP_BUTTOM_SPACE, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
	}

	m_pMsgText->SetText(m_strText);
	InitMessageType(m_uType);	
	
}

void CDuiMessageBoxDlg::InitMessageType(UINT uType)
{
	switch(uType)
	{
	case MB_OKCANCEL:
		if (m_bIsChineseOS)
		{
			m_pOKBtn->SetText(_T("确定"));
			m_pCancelBtn->SetText(_T("取消"));
		}
		else
		{
			m_pOKBtn->SetText(_T("OK"));
			m_pCancelBtn->SetText(_T("Cancel"));
		}
		break;
	case MB_YESNO:
		if (m_bIsChineseOS)
		{
			m_pOKBtn->SetText(_T("是"));
			m_pCancelBtn->SetText(_T("否"));
		}
		else
		{
			m_pOKBtn->SetText(_T("YES"));
			m_pCancelBtn->SetText(_T("NO"));
		}
		break;
	default:
		{
			if (m_bIsChineseOS)
			{
				m_pOKBtn->SetText(_T("确定"));
				m_pCancelBtn->SetVisible(false);
				m_pSpaceCtrl->SetVisible(false);
			}
			else
			{
				m_pOKBtn->SetText(_T("OK"));
				m_pCancelBtn->SetVisible(false);
				m_pSpaceCtrl->SetVisible(false);
			}
			break;
		}
		break;
	}
}

void CDuiMessageBoxDlg::GetLoginControlObject()
{
	m_pMsgText = dynamic_cast<DuiLib::CTextUI*>(m_pm.FindControl(_T("msgText")));	
	m_pOKBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("okBtn")));
	m_pCancelBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("cancelBtn")));
	m_pSpaceCtrl = m_pm.FindControl(_T("spaceCtrl"));
	m_pIconCtrl = m_pm.FindControl(_T("tipIcon"));
}

void CDuiMessageBoxDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//
	if (pSender == m_pOKBtn)  //ok
	{		
		Close(IDOK);
	}	
	else if (pSender == m_pCancelBtn || pSender->GetName()==_T("closeBtn"))
	{
		Close(IDCANCEL);
	}
	else //call base
	{
		CUIDialogFrameWnd::OnClick(pSender);
	}
}