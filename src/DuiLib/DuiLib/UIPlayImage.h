
//
//类名：CPlayImageUI  播放图片类
//功能：设置定时器，定时播放设置的图片组
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

	//功能：开始播放图片
	//返回值：TRUE表示播放成功，FALSE表示播放失败
	bool Play()
	{
		if (m_iTime>0){
			m_bStart = m_pManager->SetTimer(this,PLAYIMAGEUI_TIMERID,m_iTime);
			return m_bStart;
		}

		return false;
	}

	//功能：停止播放图片
	//返回值：TRUE表示停止播放成功，FALSE表示停止播放失败
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
			if (!IsVisible())	// 如果不可见，则不刷新
			{
				return;
			}

			if (m_nIndex >= m_nCount){  //从新回到第一张图			
				m_nIndex = 0;
			}

			//构造如下格式用于绘图：file='1.png' source='0,0,16,16' 
			CStdString strImage;
			if (m_strImageList.Find(_T("file")) == -1) {  //未设置file属性
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

	// 重载SetPos目的在于计算相对位置
	virtual void SetPos(RECT rc)
	{
		CLabelUI::SetPos(rc);

		RECT rect={0};
		rect.left = m_forePos.left;
		rect.top = m_forePos.top;
		rect.right = rect.left + m_iSize.cx*m_nZoom/100;
		rect.bottom = rect.top + m_iSize.cy*m_nZoom/100;

		// 计算相对位置
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

		// 同UIContainer中的SetFloatPos函数修订方式
		if ( (m_rcItem.right-m_rcItem.left) > 0 && (m_rcItem.bottom-m_rcItem.top) > 0 )
			m_forePos = rect;
	}

	// 绘制状态图片
	virtual void PaintStatusImage(HDC hDC)
	{		
		if( !m_sForeImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage,(LPCTSTR)m_sForeImageModify) ) m_sForeImage.Empty();
		}
	}

protected:
	bool m_bStart;	// 是否开始播放
	UINT m_iTime;	// 间隔时间(毫秒)
	UINT m_nIndex;	// 当前显示的图片ID
	SIZE m_iSize;	// 图片的尺寸
	UINT m_nCount;	// 图片数量
	UINT m_nZoom;	// 图片放大比例
	CStdString m_strImageList;	// 图片列表名称
	CStdString m_sForeImage;	// 前景图
	CStdString m_sForeImageModify;	// 前景图修订，目的位置
	RECT m_forePos;	// 前景位置
	TRelativePosUI m_relativePos;	//前景相对背景位置
};

}  // namespace DuiLib