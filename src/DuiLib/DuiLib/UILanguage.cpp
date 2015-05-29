#include "StdAfx.h"
#include "UILanguage.h"

namespace DuiLib {

CUILanguage::CUILanguage(void)
{
}

CUILanguage::~CUILanguage(void)
{
}

bool CUILanguage::LoadLanguage(STRINGorID xml, CPaintManagerUI* pManager, LPCTSTR type )
{
	if( HIWORD(xml.m_lpstr) != NULL ) {
		if( *(xml.m_lpstr) == _T('<') ) {
			if( !m_xml.Load(xml.m_lpstr) ) return NULL;
		}
		else {
			if( !m_xml.LoadFromFile(xml.m_lpstr) ) return NULL;
		}
	}
	else {
		HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), xml.m_lpstr, type);
		if( hResource == NULL ) return NULL;
		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
		if( hGlobal == NULL ) {
			FreeResource(hResource);
			return NULL;
		}

		if( !m_xml.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) return NULL;
		::FreeResource(hResource);
		m_pstrtype = type;
	}

	CMarkupNode root = m_xml.GetRoot();
	if( !root.IsValid() ) 
		return false;
	return _Parse(&root,pManager);
}

bool CUILanguage::_Parse(CMarkupNode* pRoot,CPaintManagerUI* pManager)
{
	if (pManager == NULL)
	{
		return false;
	}

	LPCTSTR pstrName = pRoot->GetName();
	//处理window属性，允许修改大小
	if( _tcscmp(pstrName, _T("Window")) == 0 ) {
		if( pManager->GetPaintWindow() ) {
			LPCTSTR pstrName = NULL;
			LPCTSTR pstrValue = NULL;
			int nAttributes = pRoot->GetAttributeCount();
			for( int i = 0; i < nAttributes; i++ ) {
				pstrName = pRoot->GetAttributeName(i);
				pstrValue = pRoot->GetAttributeValue(i);
				if( _tcscmp(pstrName, _T("size")) == 0 ) {
					LPTSTR pstr = NULL;
					int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
					pManager->SetClientSize(cx, cy);
				} 
				else if( _tcscmp(pstrName, _T("sizebox")) == 0 ) {
					RECT rcSizeBox = { 0 };
					LPTSTR pstr = NULL;
					rcSizeBox.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
					rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
					rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
					pManager->SetSizeBox(rcSizeBox);
				}
				else if( _tcscmp(pstrName, _T("caption")) == 0 ) {
					RECT rcCaption = { 0 };
					LPTSTR pstr = NULL;
					rcCaption.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
					rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
					rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
					pManager->SetCaptionRect(rcCaption);
				}
				else if( _tcscmp(pstrName, _T("roundcorner")) == 0 ) {
					LPTSTR pstr = NULL;
					int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
					pManager->SetRoundCorner(cx, cy);
				} 
				else if( _tcscmp(pstrName, _T("mininfo")) == 0 ) {
					LPTSTR pstr = NULL;
					int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
					pManager->SetMinInfo(cx, cy);
				}
				else if( _tcscmp(pstrName, _T("maxinfo")) == 0 ) {
					LPTSTR pstr = NULL;
					int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
					pManager->SetMaxInfo(cx, cy);
				}
				else if( _tcscmp(pstrName, _T("showdirty")) == 0 ) {
					pManager->SetShowUpdateRect(_tcscmp(pstrValue, _T("true")) == 0);
				} 
				else if( _tcscmp(pstrName, _T("alpha")) == 0 ) {
					pManager->SetTransparent(_ttoi(pstrValue));
				} 
				else if( _tcscmp(pstrName, _T("bktrans")) == 0 ) {
					pManager->SetBackgroundTransparent(_tcscmp(pstrValue, _T("true")) == 0);
				} 
				else if( _tcscmp(pstrName, _T("disabledfontcolor")) == 0 ) {
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					LPTSTR pstr = NULL;
					DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
					pManager->SetDefaultDisabledColor(clrColor);
				} 
				else if( _tcscmp(pstrName, _T("defaultfontcolor")) == 0 ) {
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					LPTSTR pstr = NULL;
					DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
					pManager->SetDefaultFontColor(clrColor);
				}
				else if( _tcscmp(pstrName, _T("linkfontcolor")) == 0 ) {
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					LPTSTR pstr = NULL;
					DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
					pManager->SetDefaultLinkFontColor(clrColor);
				} 
				else if( _tcscmp(pstrName, _T("linkhoverfontcolor")) == 0 ) {
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					LPTSTR pstr = NULL;
					DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
					pManager->SetDefaultLinkHoverFontColor(clrColor);
				} 
				else if( _tcscmp(pstrName, _T("selectedcolor")) == 0 ) {
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					LPTSTR pstr = NULL;
					DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
					pManager->SetDefaultSelectedBkColor(clrColor);
				} 
			}
		}
	}
	// 遍历所有XML节点
	for( CMarkupNode node = pRoot->GetChild() ; node.IsValid(); node = node.GetSibling() ) 
	{
		LPCTSTR pstrClass = node.GetName();		  
		CControlUI* pControl = NULL;
		if ( node.HasAttributes() )
		{
			TCHAR szValue[500] = { 0 };
			SIZE_T cchLen = lengthof(szValue) - 1;
			int nAttributes = node.GetAttributeCount();
			LPCTSTR pstrCtrlName = node.GetAttributeValue(_T("name"));
			if (_tcslen(pstrCtrlName) <= 0) //找不到name属性，继续
			{
				continue;
			}
			else
			{
				pControl = pManager->FindControl(pstrCtrlName);
				if (pControl == NULL)	//找不到对应的控件，继续
				{
					continue;
				}
			}
			// 重新设置属性，不考虑name属性
			for( int i = 0; i < nAttributes; i++ ) 
			{
				pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
			}
		}
	}
	return true;
}

} // namespace DuiLib