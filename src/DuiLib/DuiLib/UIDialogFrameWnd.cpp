#include "StdAfx.h"
#include "UIDialogFrameWnd.h"

namespace DuiLib
{

//�¼������Ĳ���
struct _tagEventParam
{
	WPARAM wParam;
	LPARAM lParam;
};

//�ؼ����ƶ���
#define CTRLNAME_CLOSEBUTTON		_T("closeBtn")			//�رհ�ť
#define CTRLNAME_MAXBUTTON			_T("maxBtn")			//��󻯰�ť
#define CTRLNAME_MINBUTTON			_T("minBtn")			//��С����ť
#define CTRLNAME_RESTOREBUTTON		_T("restoreBtn")		//�ָ���ť
#define CTRLNAME_CAPTIONLABEL		_T("captionLabel")		//�����ǩ�ؼ�

//ͨ���¼�����
#define NOTIFYEVENT_CLICK						_T("click")				//�����¼�
#define NOTIFYEVENT_SELECTCHANGE		_T("selectchanged")		//ѡ��״̬�ı��¼�
#define NOTIFYEVENT_ITEMSELECT				_T("itemselect")		//combo �� list ѡ����仯�¼�
#define NOTIFYEVENT_TEXTCHANGE			_T("textchanged")		//edit �ı��ı�
#define NOTIFYEVENT_WINDOWINIT			_T("windowinit")		//���ڳ�ʼ���¼�
#define NOTIFYEVENT_RETURN					_T("return")			//edit��Ӧ�س��¼�
#define NOTIFYEVENT_KILLFOCUS				_T("killfocus")			//��Ӧkillfocus�¼�
#define NOTIFYEVENT_SETFOCUS				_T("setfocus")			//��Ӧsetfocus�¼�
#define NOTIFYEVENT_HEADER_CLICK			_T("headerclick")		//��Ӧheader click�¼�
#define NOTIFYEVENT_CHAR						_T("char")				//��Ӧchar�¼�
#define NOTIFYEVENT_TIMER						_T("timer")			//��Ӧtimer�¼�

//��ʱ��ID
#define TIMERID_TIP_SHOW				1						//������ʾ��ʾʱ��

//������Ŷ���
#define ERRORCODE_LAYOUTFILE_IS_EMPTY				100000   //�����ļ�Ϊ��
#define ERRORCODE_CREATE_LOGICAL_CONTROL_FAIL		100001	 //�����߼��ؼ�ʧ��

UINT CUIDialogFrameWnd::m_nAcceptEventMessage = RegisterWindowMessage(_T("uilib_accept_event"));
CUIDialogFrameWnd::CUIDialogFrameWnd(void)
{
	m_pCloseBtn = NULL;
	m_pMaxBtn = NULL;
	m_pMinBtn = NULL;
	m_pRestoreBtn = NULL;

	m_pExampleLabel = NULL;
	m_bShowExample = true;

	m_strLayoutFile = _T("");
	m_nLastError = 0;
	m_bAutoDelete = false;

	m_pTipLabel = NULL;
}

CUIDialogFrameWnd::~CUIDialogFrameWnd(void)
{
}

LPCTSTR CUIDialogFrameWnd::GetWindowClassName() const
{
	return _T("DialogFrameWnd");
}

void CUIDialogFrameWnd::OnFinalMessage(HWND hWnd)
{
	CWindowWnd::OnFinalMessage(hWnd);

	//�Զ��ͷŶ���
	if (m_bAutoDelete)
	{
		delete this;
	}
}

void CUIDialogFrameWnd::OnInitDialog()
{
	//do nothing
}

void CUIDialogFrameWnd::OnClick(CControlUI* pSender)
{
	if (pSender == m_pCloseBtn)  //�����رհ�ť
	{
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	}
	else if (pSender == m_pMaxBtn)  //������󻯰�ť
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else if (pSender == m_pRestoreBtn)  //�����ָ���ť
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
	}
	else if (pSender == m_pMinBtn)  //������С����ť
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else  //�����ݲ�����
	{
		//do nothing
	}
}

void CUIDialogFrameWnd::OnSelectChange(CControlUI* pSender)
{
	//do nothing
}

void CUIDialogFrameWnd::OnItemSelect(CControlUI* pSender, int nCurSel, int nOldSel)
{
	//do nothing
}

void CUIDialogFrameWnd::OnTextChange(CControlUI* pSender)
{
	//do nothing
}

void CUIDialogFrameWnd::OnTextReturn(CControlUI* pSender)
{
	//do nothing
}

void CUIDialogFrameWnd::OnKillFocus(CControlUI* pSender)
{
	//do nothing
}

void CUIDialogFrameWnd::OnSetFocus(CControlUI* pSender)
{
	//do nothing
}

void CUIDialogFrameWnd::OnHeaderClick(CControlUI* pSender)
{
	//do nothing
}

void CUIDialogFrameWnd::OnChar(CControlUI* pSender, TCHAR ch)
{
	//do nothing
}

int CUIDialogFrameWnd::GetLastError()
{
	return m_nLastError;
}

BOOL CUIDialogFrameWnd::LoadLanguage(LPCTSTR lpszLanguageXml)
{
	CUILanguage uiLanguage;
	bool bResult = uiLanguage.LoadLanguage(lpszLanguageXml,&m_pm,0);
	RECT rect;
	::GetWindowRect(m_hWnd,&rect);
	::InvalidateRect(m_hWnd,&rect,TRUE);
	return bResult?TRUE:FALSE;
}

BOOL CUIDialogFrameWnd::IsWindowVisible()
{
	ASSERT(::IsWindow(m_hWnd));
	return ::IsWindowVisible(m_hWnd);
}

HWND CUIDialogFrameWnd::GetSafeHwnd() const
{
	return this == NULL ? NULL : m_hWnd;
}

BOOL CUIDialogFrameWnd::ShowWindow(int nCmdShow)
{
	ASSERT(::IsWindow(m_hWnd)); 
	return ::ShowWindow(m_hWnd, nCmdShow);
}

UINT_PTR CUIDialogFrameWnd::SetTimer(UINT_PTR nIDEvent, UINT uElapseMilliSec)
{
	ASSERT(::IsWindow(m_hWnd)); 
	if (nIDEvent <= 0xffff)  //����ʹ�ñ����Ķ�ʱ��ID
	{
		return 0;
	}

	return ::SetTimer(m_hWnd, nIDEvent, uElapseMilliSec, NULL);
}

void CUIDialogFrameWnd::SetWindowText(LPCTSTR lpszString)
{
	ASSERT(::IsWindow(m_hWnd));
	if (!::IsWindow(m_hWnd))
	{
		return;
	}

	::SetWindowText(m_hWnd, lpszString);  //���ô����ı�

	CControlUI* pCaption = m_pm.FindControl(CTRLNAME_CAPTIONLABEL);
	if (pCaption != NULL) //�����˱����ǩ
	{
		pCaption->SetText(lpszString);  //�����ǩ����Ϊ���ڵ�����
	}	
}

int CUIDialogFrameWnd::SendEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	_tagEventParam param;
	param.wParam = wParam;
	param.lParam = lParam;
	return (int)::SendMessage(m_hWnd, m_nAcceptEventMessage, (WPARAM)nCode, (LPARAM)&param);
}

void CUIDialogFrameWnd::HideTip()
{
	if (m_pTipLabel)
	{
		m_pTipLabel->SetVisible(false);
		m_pm.KillTimer(m_pTipLabel, TIMERID_TIP_SHOW);
	}
}

void CUIDialogFrameWnd::ShowTip(LPCTSTR szTip, int nDurationMilliSec, CControlUI* position)
{
	if (m_pTipLabel == NULL)
	{
		return;
	}

	if (_tcslen(szTip) == 0)  //û����ʾ����
	{
		HideTip();
		return;
	}

	if (position == NULL)
	{
		return;
	}

	//��������ʾ��ʾ��ָ���ؼ����Ϸ��м�λ��
	CRect sizeRect;
	CRenderEngine::DrawText(m_pm.GetPaintDC(), &m_pm, sizeRect, szTip, 0xff000000, m_pTipLabel->GetFont(), DT_CALCRECT|DT_SINGLELINE);			
	CRect posRect = position->GetPos();
	int nCenterX = (posRect.left+posRect.right)/2;
	int nCenterY = (posRect.top+posRect.bottom)/2;
	CRect tipRect(nCenterX-sizeRect.GetWidth()/2-2, posRect.top-sizeRect.GetHeight()-6, nCenterX+sizeRect.GetWidth()/2+2, posRect.top-2);
	m_pTipLabel->SetPos(tipRect);	
	m_pTipLabel->SetText(szTip);	
	m_pTipLabel->SetVisible(true);
	if (nDurationMilliSec == -1)
	{
		m_pm.KillTimer(m_pTipLabel, 1);
	}
	else
	{
		//kinglon test
		TRACE(_T("SetTimer start\n"));
		m_pm.SetTimer(m_pTipLabel, TIMERID_TIP_SHOW, nDurationMilliSec);
	}
}

void CUIDialogFrameWnd::ShowTip(LPCTSTR szTip, int nDurationMilliSec, int nAbsLeftPos, int nAbsTopPos)
{
	if (m_pTipLabel == NULL)
	{
		return;
	}

	if (_tcslen(szTip) == 0)  //û����ʾ����
	{
		HideTip();
		return;
	}

	//����ʾ���ı������ݼ��ؼ�λ�ü���ʾ������ʾ��С��λ��
	CRect sizeRect;
	CRenderEngine::DrawText(m_pm.GetPaintDC(), &m_pm, sizeRect, szTip, 0xff000000, m_pTipLabel->GetFont(), DT_CALCRECT|DT_SINGLELINE);			
	CRect tipRect(nAbsLeftPos, nAbsTopPos, nAbsTopPos+sizeRect.GetWidth()+4, nAbsTopPos+sizeRect.GetHeight()+4);
	m_pTipLabel->SetPos(tipRect);	
	m_pTipLabel->SetText(szTip);	
	m_pTipLabel->SetVisible(true);
	if (nDurationMilliSec == -1)
	{
		m_pm.KillTimer(m_pTipLabel, 1);
	}
	else
	{
		m_pm.SetTimer(m_pTipLabel, TIMERID_TIP_SHOW, nDurationMilliSec);
	}
}

BOOL CUIDialogFrameWnd::KillTimer(UINT_PTR nIDEvent)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::KillTimer(m_hWnd, nIDEvent);
}

HWND CUIDialogFrameWnd::Create(LPCTSTR lpszTemplateName, HWND hwndParent, DWORD dwStyle, DWORD dwExStyle)
{
	if (lpszTemplateName == NULL || _tcslen(lpszTemplateName) == 0) //�����ļ�Ϊ��
	{
		m_nLastError = ERRORCODE_LAYOUTFILE_IS_EMPTY;
		return NULL;
	}
	m_strLayoutFile = lpszTemplateName;

	HWND hWnd = CWindowWnd::Create(hwndParent, NULL, dwStyle, dwExStyle); //��������
	if (hWnd == NULL)  //fail
	{
		m_nLastError = ::GetLastError();  //get error code
		return NULL;
	}
	
	if ((dwStyle & WS_CHILD) == 0)  //�����Ӵ���
	{
		CenterWindow();		//Ĭ�Ͼ�����ʾ
	}		

	return hWnd;
}

BOOL CUIDialogFrameWnd::DestroyWindow()
{
	ASSERT(::IsWindow(m_hWnd));
	return ::DestroyWindow(m_hWnd);
}

BOOL CUIDialogFrameWnd::OnClose()
{
	return FALSE;
}

BOOL CUIDialogFrameWnd::OnMaximize()
{
	//do nothing
	return FALSE;
}

BOOL CUIDialogFrameWnd::OnMinimize()
{
	//do nothing
	return FALSE;
}

IDialogBuilderCallback* CUIDialogFrameWnd::GetDialogBuilderCallback()
{
	return NULL;   //Ĭ��û�лص�
}

IUIWndContainerCallback* CUIDialogFrameWnd::GetWndContainerCallback()
{
	return NULL;
}

LRESULT CUIDialogFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;			//ȥ������
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	
	m_pm.Init(m_hWnd);				//�����滭������
	m_pm.SetWndContainerCallback(GetWndContainerCallback());		//���ô��������ص�

	CDialogBuilder builder;	
	IDialogBuilderCallback* pDBCallback = GetDialogBuilderCallback();
	CControlUI* pRoot = builder.Create(STRINGorID(m_strLayoutFile), (UINT)0,  pDBCallback, &m_pm); //�����߼��ؼ�
	ASSERT(pRoot != NULL);
	if (pRoot == NULL) //�����߼��ؼ�ʧ��
	{
		m_nLastError = ERRORCODE_CREATE_LOGICAL_CONTROL_FAIL;
		return -1; //ָʾϵͳ���ٴ���
	}	
	m_pm.AttachDialog(pRoot);  //�߼��ؼ����뵽��ϣ���У��Ա�����ؼ��Ĳ���
	m_pm.AddNotifier(this);   //����Ϊͨ������Ա�����߼��ؼ���ͨ����Ϣ
	
	//�����߼��ؼ���λ��
	RECT rcClient;
	::GetClientRect(m_hWnd,&rcClient);
	pRoot->SetPos(rcClient);

	//��ȡ��ť�����ö�̬ת����ȷ���ǰ�ť����
	m_pCloseBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_CLOSEBUTTON));
	m_pMaxBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_MAXBUTTON));
	m_pRestoreBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_RESTOREBUTTON));
	m_pMinBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_MINBUTTON));

	//���ô��ڱ���
	CControlUI* pCaption = m_pm.FindControl(CTRLNAME_CAPTIONLABEL);
	if (pCaption != NULL) //�����˱����ǩ
	{
		::SetWindowText(m_hWnd, pCaption->GetText().GetData());
	}

	//����ʾ���ı���
	CContainerUI* pContainer = dynamic_cast<CContainerUI*>(pRoot);
	m_pExampleLabel = new CLabelUI;
	if (m_pExampleLabel)
	{
		m_pExampleLabel->SetVisible(false);
		m_pExampleLabel->SetFloat(true);
		
		if (pContainer)
		{
			pContainer->Add(m_pExampleLabel);
		}
	}

	//�������ݿ�
	m_pTipLabel = new CLabelUI;
	if (m_pTipLabel)
	{
		m_pTipLabel->SetVisible(false);
		m_pTipLabel->SetFloat(true);
		m_pTipLabel->SetBkColor(0xff2a2a2a);
		m_pTipLabel->SetTextColor(0xffffffff);
		m_pTipLabel->SetAttribute(_T("align"), _T("center"));

		if (pContainer)
		{
			pContainer->Add(m_pTipLabel);
		}
	}

	//��������Щ��ʼ������
	OnInitDialog();

	return 0;  //������������
}

LRESULT CUIDialogFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//do nothing

	return 0;
}

LRESULT CUIDialogFrameWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//do nothing

	return 0;
}

LRESULT CUIDialogFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if (wParam == 0)  //��ǰδ����
	{
		return TRUE;  //����
	}
	else  //��ǰ����
	{
		return FALSE;   //δ����
	}
}

LRESULT CUIDialogFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//do nothing

	return 0;
}

LRESULT CUIDialogFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//do nothing

	return 0;
}

LRESULT CUIDialogFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt; 
	pt.x = GET_X_LPARAM(lParam); 
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);  //���λ��

	RECT rcClient;
	::GetClientRect(*this, &rcClient);  //�ͻ�����

	RECT rcCaption = m_pm.GetCaptionRect();   //��������
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) //�ڱ���������
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0  \
			&& _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 \
			&& _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )  //�������߼��ؼ��϶����߼��ؼ����ǰ�ť����ѡ��ť���ı���
		{
			return HTCAPTION;  //ָʾ����������
		}
	}

	return HTCLIENT;  //���������ָʾ�ͻ�����
}

LRESULT CUIDialogFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) //������Բ��
	{
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);   //����Բ������
		::DeleteObject(hRgn);
	}

	return 0;
}

LRESULT CUIDialogFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor); //��ȡ��ʾ����С��Ϣ
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	//�������ߴ�Ϊ��Ļ�������Ĵ�С
	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;
	
	return 0;
}

void CUIDialogFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�Ͻ������ദ��
	CWindowWnd::HandleMessage(WM_COMMAND, wParam, lParam);
}

void CUIDialogFrameWnd::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�Ͻ������ദ��
	CWindowWnd::HandleMessage(WM_DRAWITEM, wParam, lParam);
}

void CUIDialogFrameWnd::OnMeasureItem(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�Ͻ������ദ��
	CWindowWnd::HandleMessage(WM_MEASUREITEM, wParam, lParam);
}

BOOL CUIDialogFrameWnd::OnTimer(CControlUI* pSender, DWORD dwTimerID)
{
	//do nothing
	return FALSE;
}

LRESULT CUIDialogFrameWnd::OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	//do nothing
	return 0;
}

LRESULT CUIDialogFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bZoomed = ::IsZoomed(*this);  //��ʶ����ǰ�����Ƿ������
	BOOL bHandle = FALSE;   //��ʶ��Ϣ�Ƿ���
	LRESULT lRes = 0;

	if (wParam == SC_MAXIMIZE)  //���
	{
		bHandle = OnMaximize();
	}
	else if (wParam == SC_MINIMIZE)  //��С��
	{
		bHandle = OnMinimize();
	}
	else if (wParam == SC_CLOSE) //�ر�
	{
		bHandle = OnClose();
	}

	if (!bHandle)  //��δ����
	{
		lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam); //��������Ĭ�ϴ���
	}
	
	if (::IsZoomed(*this) != bZoomed) //��󻯱���С�� �� ��С�������
	{
		if (!bZoomed)  //ԭ�������
		{
			if (m_pMaxBtn != NULL)
			{
				m_pMaxBtn->SetVisible(false);  //������󻯰�ť
			}

			if (m_pRestoreBtn != NULL)
			{
				m_pRestoreBtn->SetVisible(true);  //��ʾ�ָ���ť
			}
		}
		else  //ԭ������С��
		{
			if (m_pMaxBtn != NULL)
			{
				m_pMaxBtn->SetVisible(true);  //��ʾ��󻯰�ť
			}

			if (m_pRestoreBtn != NULL)
			{
				m_pRestoreBtn->SetVisible(false);  //���ػָ���ť
			}
		}
	}

	return lRes;
}

void CUIDialogFrameWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == NOTIFYEVENT_KILLFOCUS)		//killfocus�¼�
	{
		ShowExample(false, NULL);
		OnKillFocus(msg.pSender);
	}
	if (msg.sType == NOTIFYEVENT_SETFOCUS)		//killfocus�¼�
	{
		ShowExample(true, msg.pSender);
		OnSetFocus(msg.pSender);
	}
	else if (msg.sType == NOTIFYEVENT_CLICK)   //�����¼�
	{
		OnClick(msg.pSender);    //���ɵ�����������
	}
	else if (msg.sType == NOTIFYEVENT_SELECTCHANGE)  //ѡ��״̬�ı��¼�
	{
		OnSelectChange(msg.pSender);  //���ɵ�ѡ��״̬�ı䴦����
	}
	else if (msg.sType == NOTIFYEVENT_ITEMSELECT)  //ѡ����仯�¼�
	{
		OnItemSelect(msg.pSender, (int)msg.wParam, (int)msg.lParam);
	}
	else if (msg.sType == NOTIFYEVENT_TEXTCHANGE)  //�ı��ı�
	{
		OnTextChange(msg.pSender);
	}	
	else if (msg.sType == NOTIFYEVENT_RETURN)		//edit�س��¼�
	{
		OnTextReturn(msg.pSender);
	}
	else if (msg.sType == NOTIFYEVENT_HEADER_CLICK)  //header click
	{
		OnHeaderClick(msg.pSender);
	}
	else if (msg.sType == NOTIFYEVENT_CHAR)  //char�¼�
	{
		OnChar(msg.pSender, (TCHAR)msg.wParam);
	}
	else if (msg.sType == NOTIFYEVENT_TIMER)  //timer�¼�
	{
		//�����������ʾ��Ķ�ʱ�¼������ض�ʱ��
		if (msg.pSender == m_pTipLabel)
		{
			TRACE(_T("SetTimer end\n"));
			HideTip();
		}

		OnTimer(msg.pSender, (DWORD)msg.wParam);
	}
	else  //�����¼�Ŀǰδʹ�ã��ݲ�����
	{
		//do nothing
	}
}

void CUIDialogFrameWnd::ShowExample(bool bShow, CControlUI* pControl)
{
	if (m_pExampleLabel == NULL)  //û��ʾ����
	{
		return;
	}

	if (!m_bShowExample) //�Ѿ����ò���ʾ������������
	{		
		m_pExampleLabel->SetVisible(false);
		return;
	}

	if (!bShow) //Ҫ������
	{
		m_pExampleLabel->SetVisible(false);
		return;
	}

	ASSERT(pControl);
	if (pControl == NULL)
	{
		return;
	}

	//Ŀǰֻ��edit֧�ָù���
	CEditUI* pEdit = (CEditUI*)pControl->GetInterface(_T("Edit"));
	if (pEdit == NULL)
	{
		return;
	}
	
	CStdString strText(pEdit->GetExampleText());
	if (strText.IsEmpty())  //û��ʾ���ı�
	{
		m_pExampleLabel->SetVisible(false);
	}
	else
	{			
		//����ʾ���ı������ݼ��ؼ�λ�ü���ʾ������ʾ��С��λ��
		CRect sizeRect;
		CRenderEngine::DrawText(m_pm.GetPaintDC(), &m_pm, sizeRect, strText, 0xff000000, pEdit->GetFont(), DT_CALCRECT|DT_SINGLELINE);			
		const RECT& editRect = pEdit->GetPos();
		CRect exampleRect(editRect.left, editRect.top-sizeRect.GetHeight()-5, editRect.left+sizeRect.GetWidth()+4, editRect.top-1);

		m_pExampleLabel->SetPos(exampleRect);
		m_pExampleLabel->SetTextPadding(CRect(2,0,0,0));
		m_pExampleLabel->SetText(strText);
		m_pExampleLabel->SetTextColor(pEdit->GetExampleTextColor());
		m_pExampleLabel->SetBkColor(pEdit->GetExampleBkColor());
		m_pExampleLabel->SetVisible(true);
	}	
}

LRESULT CUIDialogFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	//�����κ����飬Ϊ�������ܣ�������ʵ�ָú���
	return 0;
}

LRESULT CUIDialogFrameWnd::PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	bHandled = FALSE;   //Ĭ������Ϊδ����

	if (uMsg == m_nAcceptEventMessage)  //�¼�������Ϣ
	{			
		lRes = OnAcceptEvent((int)wParam, ((_tagEventParam*)lParam)->wParam, ((_tagEventParam*)lParam)->lParam);
		bHandled = TRUE;
		return lRes;
	}

	switch (uMsg)
	{
	case WM_CREATE:
		lRes = OnCreate(uMsg, wParam, lParam);
		bHandled = TRUE;
		break;

	case WM_CLOSE:
		m_pm.MessageHandler(uMsg, wParam, lParam, lRes); //�Ƚ����滭����������
		bHandled = OnClose();		
		break;

	case WM_DESTROY:
		lRes = OnDestroy(uMsg, wParam, lParam);
		bHandled = TRUE;
		break;

	case WM_SHOWWINDOW:
		lRes = OnShowWindow(uMsg, wParam, lParam);
		bHandled = TRUE;
		break;
	
	case WM_NCACTIVATE:
		lRes = OnNcActivate(uMsg, wParam, lParam);
		bHandled = TRUE;  //��ܴ�����û�б߿����ڴ˴���
		break;

	case WM_NCCALCSIZE:    
		lRes = OnNcCalcSize(uMsg, wParam, lParam);
		bHandled = TRUE;  //��ܴ�����û�б߿����ڴ˴���
		break;

	case WM_NCPAINT:
		lRes = OnNcPaint(uMsg, wParam, lParam);
		bHandled = TRUE;  //��ܴ�����û�б߿����ڴ˴���
		break;

	case WM_NCHITTEST:
		lRes = OnNcHitTest(uMsg, wParam, lParam);
		bHandled = TRUE; //Ĭ��ʵ�֣�ģ���������ֱ�Ӵ���
		break;

	case WM_SIZE:
		lRes = OnSize(uMsg, wParam, lParam);
		break;

	case WM_GETMINMAXINFO:
		lRes = OnGetMinMaxInfo(uMsg, wParam, lParam);		
		break;

	case WM_SYSCOMMAND:
		lRes = OnSysCommand(uMsg, wParam, lParam);
		bHandled = TRUE;
		break;

	case WM_COMMAND:
		OnCommand(wParam, lParam);		
		break;

	case WM_DRAWITEM:
		OnDrawItem(wParam, lParam);		
		break;

	case WM_MEASUREITEM:
		OnMeasureItem(wParam, lParam);		
		break;

	case WM_MOUSEWHEEL:
		//����������ʾ�����������ʾ��
		ShowExample(false, NULL);
		HideTip();
		break;

	case WM_LBUTTONDOWN:
		HideTip();
		break;

	case WM_LBUTTONDBLCLK:
		HideTip();
		break;

	case WM_TIMER:		
		bHandled = OnTimer(NULL, (DWORD)wParam);
		break;

	default:  //������Ϣ������
		break;
	}

	return lRes;
}


LRESULT CUIDialogFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//�����κ����飬����������ʵ���Զ�����Ϣ����
	return 0;
}

LRESULT CUIDialogFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;   //Ĭ������Ϊδ����

	lRes = PreTranslateMessage(uMsg, wParam, lParam, bHandled);  //Ԥ����
	if (bHandled)   //�Ѵ���
	{
		return lRes;  //����������
	}	

	// �����Զ�����Ϣ
	lRes = HandleCustomMessage(uMsg,wParam,lParam,bHandled);
	if (bHandled)   //�Ѵ���
	{
		return lRes;  //����������
	}

	//�����滭����������ģ���߼��ؼ���Ϣ	
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))  //�Ѵ���
	{
		return lRes; //�������¼�������
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  //�������ദ��
}

}  // namespace DuiLib