#include "StdAfx.h"
#include "UIWndContainer.h"
#include "UIDialogFrameWnd.h"

namespace DuiLib
{

CUIWndContainer::CUIWndContainer(IUIWndContainerCallback* pCallback) : 
m_pWnd(NULL)
{
	m_pCallback = pCallback;
}

CUIWndContainer::~CUIWndContainer(void)
{
	if (m_pWnd != NULL)
	{
		delete m_pWnd;
		m_pWnd = NULL;
	}
}

void CUIWndContainer::SetPos(RECT rc)
{
	CContainerUI::SetPos(rc);

	if (m_strXMLFile.IsEmpty() || m_strWndClassName.IsEmpty())
	{
		return;
	}

	if (m_pWnd != NULL)
	{
		if (!::IsWindow(m_pWnd->GetSafeHwnd()))
		{
			if (m_pWnd->Create(m_strXMLFile.GetData(), GetManager()->GetPaintWindow(), WS_CHILD|WS_VISIBLE, 0) == NULL)  //failed
			{			
				return;
			}
		}

		//调整为容器那么大
		::SetWindowPos(m_pWnd->GetSafeHwnd(), NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CUIWndContainer::SetVisible(bool bVisible)
{
	CContainerUI::SetVisible(bVisible);
	
	if (m_pWnd != NULL && ::IsWindow(m_pWnd->GetSafeHwnd()))
	{
		if (IsVisible())
		{
			m_pWnd->ShowWindow(SW_SHOWNORMAL);
		}
		else
		{
			m_pWnd->ShowWindow(SW_HIDE);
		}
	}	
}

void CUIWndContainer::SetInternVisible(bool bVisible)
{
	CContainerUI::SetInternVisible(bVisible);

	if (m_pWnd != NULL && ::IsWindow(m_pWnd->GetSafeHwnd()))
	{
		if (IsVisible())
		{
			m_pWnd->ShowWindow(SW_SHOWNORMAL);
		}
		else
		{
			m_pWnd->ShowWindow(SW_HIDE);
		}
	}
}

void CUIWndContainer::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("wndclassname")) == 0)
	{
		SetWndClass(pstrValue);		
	}
	else if(_tcscmp(pstrName, _T("xmlfile")) == 0)
	{
		m_strXMLFile = pstrValue;
	}
	else
	{
		CContainerUI::SetAttribute(pstrName, pstrValue);
	}
}

void CUIWndContainer::SetXMLFile(LPCTSTR pstrValue)
{
	SetAttribute(_T("xmlfile"), pstrValue);
}

void CUIWndContainer::SetWndClass(LPCTSTR pstrValue)
{
	m_strWndClassName = pstrValue;	
	if (m_pCallback != NULL && m_pWnd == NULL)
	{
		m_pWnd = m_pCallback->NewWndObject(pstrValue);
		ASSERT(m_pWnd);
	}
}

} //namespace DuiLib