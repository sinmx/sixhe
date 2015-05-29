
//
//������CPlayImageUI  ����ͼƬ��
//���ܣ����ö�ʱ������ʱ�������õ�ͼƬ��
//

#pragma once

namespace DuiLib {

class UILIB_API CPlayImageUI : public CLabelUI
{	
public:
	enum { PLAYIMAGEUI_TIMERID = 110 };
	CPlayImageUI()
	{
		m_iTime = 0;
		m_nIndex = 0;
		m_iSize.cx=0;
		m_iSize.cy=0;
		m_nCount = 0;
		m_nZoom = 100;
		m_bStart = false;
		::ZeroMemory(&m_forePos, sizeof(RECT));
		::ZeroMemory(&m_relativePos, sizeof(TRelativePosUI));
		m_sForeImage.Empty();
	}

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("timer")) == 0 ) {			
			LPTSTR pstr = NULL;
			m_iTime = _tcstol(pstrValue, &pstr, 10);  			
		}		
		else if( _tcscmp(pstrName, _T("foreimagelist")) == 0 ) {			
			m_strImageList = pstrValue; 			
		}		
		else if( _tcscmp(pstrName, _T("foreimagewidth")) == 0 ) {			
			LPTSTR pstr = NULL;
			m_iSize.cx = _tcstol(pstrValue, &pstr, 10);  			
		}
		else if( _tcscmp(pstrName, _T("foreimageheight")) == 0 ) {			
			LPTSTR pstr = NULL;
			m_iSize.cy = _tcstol(pstrValue, &pstr, 10);  			
		}
		else if( _tcscmp(pstrName, _T("foreimagecount")) == 0 ) {			
			LPTSTR pstr = NULL;
			m_nCount = _tcstol(pstrValue, &pstr, 10); 
		}
		else if( _tcscmp(pstrName, _T("foreimagepos")) == 0 ) {			
			LPTSTR pstr = NULL;
			m_forePos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			m_forePos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			m_forePos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			m_forePos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);   
		}
		else if( _tcscmp(pstrName, _T("foreimagerelativepos")) == 0 ) {	
			LPTSTR pstr = NULL;
			m_relativePos.nMoveXPercent = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			m_relativePos.nMoveYPercent = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			m_relativePos.nZoomXPercent = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			m_relativePos.nZoomYPercent = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);  
			m_relativePos.bRelative = true;
		}
		else if ( _tcscmp(pstrName, _T("foreimagezoom")) == 0 )	{
			LPTSTR pstr = NULL;
			m_nZoom = _tcstol(pstrValue, &pstr, 10);  
		}
		else CLabelUI::SetAttribute(pstrName,pstrValue);
	}

	//���ܣ���ʼ����ͼƬ
	//����ֵ��TRUE��ʾ���ųɹ���FALSE��ʾ����ʧ��
	bool Play()
	{
		if (m_iTime>0){
			m_bStart = m_pManager->SetTimer(this,PLAYIMAGEUI_TIMERID,m_iTime);
			return m_bStart;
		}

		return false;
	}

	//���ܣ�ֹͣ����ͼƬ
	//����ֵ��TRUE��ʾֹͣ���ųɹ���FALSE��ʾֹͣ����ʧ��
	bool Stop()
	{
		if (m_bStart){
			bool bOK = m_pManager->KillTimer(this,PLAYIMAGEUI_TIMERID);
			m_bStart = bOK?false:true;
			return bOK;
		}
		return true;		
	}

	void DoEvent(TEventUI& event) 
	{
		if( event.Type == UIEVENT_TIMER && event.wParam == PLAYIMAGEUI_TIMERID )
		{
			if (!IsVisible())	// ������ɼ�����ˢ��
			{
				return;
			}

			if (m_nIndex >= m_nCount){  //���»ص���һ��ͼ			
				m_nIndex = 0;
			}

			//�������¸�ʽ���ڻ�ͼ��file='1.png' source='0,0,16,16' 
			CStdString strImage;
			if (m_strImageList.Find(_T("file")) == -1) {  //δ����file����
				strImage.Format(_T("file='%s' "),m_strImageList.GetData());
			}
			else {
				strImage = m_strImageList + _T(" ");
			}
			CStdString strTemp;
			strTemp.Format(_T("source='%d,%d,%d,%d' "),m_iSize.cx*m_nIndex,0,m_iSize.cx*(m_nIndex+1),m_iSize.cy);
			strImage += strTemp;	
			m_sForeImage = strImage;
			Invalidate();
			//this->SetBkImage(strImage.GetData());
			m_nIndex ++;
			return;
		}
		CLabelUI::DoEvent(event);
	}

	// ����SetPosĿ�����ڼ������λ��
	virtual void SetPos(RECT rc)
	{
		CLabelUI::SetPos(rc);

		RECT rect={0};
		rect.left = m_forePos.left;
		rect.top = m_forePos.top;
		rect.right = rect.left + m_iSize.cx*m_nZoom/100;
		rect.bottom = rect.top + m_iSize.cy*m_nZoom/100;

		// �������λ��
		if (m_relativePos.bRelative)
		{
			SIZE szParent = {m_rcItem.right-m_rcItem.left,m_rcItem.bottom-m_rcItem.top};
			if(m_relativePos.szParent.cx != 0)
			{
				int nIncrementX = szParent.cx-m_relativePos.szParent.cx;
				int nIncrementY = szParent.cy-m_relativePos.szParent.cy;
				rect.left += (nIncrementX*m_relativePos.nMoveXPercent/100);
				rect.top += (nIncrementY*m_relativePos.nMoveYPercent/100);
				rect.right = rect.left+m_iSize.cx*m_nZoom/100+(nIncrementX*m_relativePos.nZoomXPercent/100);
				rect.bottom = rect.top+m_iSize.cy*m_nZoom/100+(nIncrementY*m_relativePos.nZoomYPercent/100);
			}
			m_relativePos.szParent = szParent;
		}
		m_sForeImageModify.Empty();
		m_sForeImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rect.left, rect.top, rect.right, rect.bottom);

		// ͬUIContainer�е�SetFloatPos�����޶���ʽ
		if ( (m_rcItem.right-m_rcItem.left) > 0 && (m_rcItem.bottom-m_rcItem.top) > 0 )
			m_forePos = rect;
	}

	// ����״̬ͼƬ
	virtual void PaintStatusImage(HDC hDC)
	{		
		if( !m_sForeImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage,(LPCTSTR)m_sForeImageModify) ) m_sForeImage.Empty();
		}
	}

protected:
	bool m_bStart;	// �Ƿ�ʼ����
	UINT m_iTime;	// ���ʱ��(����)
	UINT m_nIndex;	// ��ǰ��ʾ��ͼƬID
	SIZE m_iSize;	// ͼƬ�ĳߴ�
	UINT m_nCount;	// ͼƬ����
	UINT m_nZoom;	// ͼƬ�Ŵ����
	CStdString m_strImageList;	// ͼƬ�б�����
	CStdString m_sForeImage;	// ǰ��ͼ
	CStdString m_sForeImageModify;	// ǰ��ͼ�޶���Ŀ��λ��
	RECT m_forePos;	// ǰ��λ��
	TRelativePosUI m_relativePos;	//ǰ����Ա���λ��
};

}  // namespace DuiLib