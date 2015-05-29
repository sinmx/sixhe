#include "StdAfx.h"
#include "UIDialogFrameWnd.h"

namespace DuiLib
{

//事件关联的参数
struct _tagEventParam
{
	WPARAM wParam;
	LPARAM lParam;
};

//控件名称定义
#define CTRLNAME_CLOSEBUTTON		_T("closeBtn")			//关闭按钮
#define CTRLNAME_MAXBUTTON			_T("maxBtn")			//最大化按钮
#define CTRLNAME_MINBUTTON			_T("minBtn")			//最小化按钮
#define CTRLNAME_RESTOREBUTTON		_T("restoreBtn")		//恢复按钮
#define CTRLNAME_CAPTIONLABEL		_T("captionLabel")		//标题标签控件

//通告事件定义
#define NOTIFYEVENT_CLICK						_T("click")				//单击事件
#define NOTIFYEVENT_SELECTCHANGE		_T("selectchanged")		//选择状态改变事件
#define NOTIFYEVENT_ITEMSELECT				_T("itemselect")		//combo 或 list 选中项变化事件
#define NOTIFYEVENT_TEXTCHANGE			_T("textchanged")		//edit 文本改变
#define NOTIFYEVENT_WINDOWINIT			_T("windowinit")		//窗口初始化事件
#define NOTIFYEVENT_RETURN					_T("return")			//edit响应回车事件
#define NOTIFYEVENT_KILLFOCUS				_T("killfocus")			//响应killfocus事件
#define NOTIFYEVENT_SETFOCUS				_T("setfocus")			//响应setfocus事件
#define NOTIFYEVENT_HEADER_CLICK			_T("headerclick")		//响应header click事件
#define NOTIFYEVENT_CHAR						_T("char")				//响应char事件
#define NOTIFYEVENT_TIMER						_T("timer")			//响应timer事件

//定时器ID
#define TIMERID_TIP_SHOW				1						//泡泡提示显示时间

//错误代号定义
#define ERRORCODE_LAYOUTFILE_IS_EMPTY				100000   //布局文件为空
#define ERRORCODE_CREATE_LOGICAL_CONTROL_FAIL		100001	 //创建逻辑控件失败

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

	//自动释放对象
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
	if (pSender == m_pCloseBtn)  //单击关闭按钮
	{
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	}
	else if (pSender == m_pMaxBtn)  //单击最大化按钮
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else if (pSender == m_pRestoreBtn)  //单击恢复按钮
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
	}
	else if (pSender == m_pMinBtn)  //单击最小化按钮
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else  //其他暂不处理
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
	if (nIDEvent <= 0xffff)  //不可使用保留的定时器ID
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

	::SetWindowText(m_hWnd, lpszString);  //设置窗口文本

	CControlUI* pCaption = m_pm.FindControl(CTRLNAME_CAPTIONLABEL);
	if (pCaption != NULL) //设置了标题标签
	{
		pCaption->SetText(lpszString);  //标题标签设置为窗口的内容
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

	if (_tcslen(szTip) == 0)  //没有提示内容
	{
		HideTip();
		return;
	}

	if (position == NULL)
	{
		return;
	}

	//将泡泡提示显示在指定控件的上方中间位置
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

	if (_tcslen(szTip) == 0)  //没有提示内容
	{
		HideTip();
		return;
	}

	//根据示例文本的内容及控件位置计算示例框显示大小及位置
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
	if (lpszTemplateName == NULL || _tcslen(lpszTemplateName) == 0) //布局文件为空
	{
		m_nLastError = ERRORCODE_LAYOUTFILE_IS_EMPTY;
		return NULL;
	}
	m_strLayoutFile = lpszTemplateName;

	HWND hWnd = CWindowWnd::Create(hwndParent, NULL, dwStyle, dwExStyle); //创建窗口
	if (hWnd == NULL)  //fail
	{
		m_nLastError = ::GetLastError();  //get error code
		return NULL;
	}
	
	if ((dwStyle & WS_CHILD) == 0)  //不是子窗口
	{
		CenterWindow();		//默认居中显示
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
	return NULL;   //默认没有回调
}

IUIWndContainerCallback* CUIDialogFrameWnd::GetWndContainerCallback()
{
	return NULL;
}

LRESULT CUIDialogFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;			//去掉标题
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	
	m_pm.Init(m_hWnd);				//关联绘画管理器
	m_pm.SetWndContainerCallback(GetWndContainerCallback());		//设置窗口容器回调

	CDialogBuilder builder;	
	IDialogBuilderCallback* pDBCallback = GetDialogBuilderCallback();
	CControlUI* pRoot = builder.Create(STRINGorID(m_strLayoutFile), (UINT)0,  pDBCallback, &m_pm); //创建逻辑控件
	ASSERT(pRoot != NULL);
	if (pRoot == NULL) //创建逻辑控件失败
	{
		m_nLastError = ERRORCODE_CREATE_LOGICAL_CONTROL_FAIL;
		return -1; //指示系统销毁窗口
	}	
	m_pm.AttachDialog(pRoot);  //逻辑控件加入到哈希表中，以便后续控件的查找
	m_pm.AddNotifier(this);   //增加为通告对象，以便接收逻辑控件的通告消息
	
	//调整逻辑控件的位置
	RECT rcClient;
	::GetClientRect(m_hWnd,&rcClient);
	pRoot->SetPos(rcClient);

	//获取按钮对象，用动态转化，确保是按钮对象
	m_pCloseBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_CLOSEBUTTON));
	m_pMaxBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_MAXBUTTON));
	m_pRestoreBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_RESTOREBUTTON));
	m_pMinBtn = dynamic_cast<CButtonUI*>(m_pm.FindControl(CTRLNAME_MINBUTTON));

	//设置窗口标题
	CControlUI* pCaption = m_pm.FindControl(CTRLNAME_CAPTIONLABEL);
	if (pCaption != NULL) //设置了标题标签
	{
		::SetWindowText(m_hWnd, pCaption->GetText().GetData());
	}

	//创建示例文本框
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

	//创建泡泡框
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

	//让子类做些初始化工作
	OnInitDialog();

	return 0;  //继续创建窗口
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
	if (wParam == 0)  //当前未激活
	{
		return TRUE;  //激活
	}
	else  //当前激活
	{
		return FALSE;   //未激活
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
	::ScreenToClient(*this, &pt);  //鼠标位置

	RECT rcClient;
	::GetClientRect(*this, &rcClient);  //客户区域

	RECT rcCaption = m_pm.GetCaptionRect();   //标题区域
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) //在标题区域内
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0  \
			&& _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 \
			&& _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )  //单击在逻辑控件上而且逻辑控件不是按钮、可选按钮、文本框
		{
			return HTCAPTION;  //指示标题栏区域
		}
	}

	return HTCLIENT;  //其他情况，指示客户区域
}

LRESULT CUIDialogFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) //设置了圆角
	{
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);   //设置圆角区域
		::DeleteObject(hRgn);
	}

	return 0;
}

LRESULT CUIDialogFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor); //获取显示器大小信息
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	//设置最大尺寸为屏幕工作区的大小
	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;
	
	return 0;
}

void CUIDialogFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//默认交给父类处理
	CWindowWnd::HandleMessage(WM_COMMAND, wParam, lParam);
}

void CUIDialogFrameWnd::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	//默认交给父类处理
	CWindowWnd::HandleMessage(WM_DRAWITEM, wParam, lParam);
}

void CUIDialogFrameWnd::OnMeasureItem(WPARAM wParam, LPARAM lParam)
{
	//默认交给父类处理
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
	BOOL bZoomed = ::IsZoomed(*this);  //标识处理前创口是否处于最大化
	BOOL bHandle = FALSE;   //标识消息是否处理
	LRESULT lRes = 0;

	if (wParam == SC_MAXIMIZE)  //最大化
	{
		bHandle = OnMaximize();
	}
	else if (wParam == SC_MINIMIZE)  //最小化
	{
		bHandle = OnMinimize();
	}
	else if (wParam == SC_CLOSE) //关闭
	{
		bHandle = OnClose();
	}

	if (!bHandle)  //尚未处理
	{
		lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam); //交给父类默认处理
	}
	
	if (::IsZoomed(*this) != bZoomed) //最大化变最小化 或 最小化变最大化
	{
		if (!bZoomed)  //原来是最大化
		{
			if (m_pMaxBtn != NULL)
			{
				m_pMaxBtn->SetVisible(false);  //隐藏最大化按钮
			}

			if (m_pRestoreBtn != NULL)
			{
				m_pRestoreBtn->SetVisible(true);  //显示恢复按钮
			}
		}
		else  //原来是最小化
		{
			if (m_pMaxBtn != NULL)
			{
				m_pMaxBtn->SetVisible(true);  //显示最大化按钮
			}

			if (m_pRestoreBtn != NULL)
			{
				m_pRestoreBtn->SetVisible(false);  //隐藏恢复按钮
			}
		}
	}

	return lRes;
}

void CUIDialogFrameWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == NOTIFYEVENT_KILLFOCUS)		//killfocus事件
	{
		ShowExample(false, NULL);
		OnKillFocus(msg.pSender);
	}
	if (msg.sType == NOTIFYEVENT_SETFOCUS)		//killfocus事件
	{
		ShowExample(true, msg.pSender);
		OnSetFocus(msg.pSender);
	}
	else if (msg.sType == NOTIFYEVENT_CLICK)   //单击事件
	{
		OnClick(msg.pSender);    //分派到单击处理函数
	}
	else if (msg.sType == NOTIFYEVENT_SELECTCHANGE)  //选择状态改变事件
	{
		OnSelectChange(msg.pSender);  //分派到选择状态改变处理函数
	}
	else if (msg.sType == NOTIFYEVENT_ITEMSELECT)  //选中项变化事件
	{
		OnItemSelect(msg.pSender, (int)msg.wParam, (int)msg.lParam);
	}
	else if (msg.sType == NOTIFYEVENT_TEXTCHANGE)  //文本改变
	{
		OnTextChange(msg.pSender);
	}	
	else if (msg.sType == NOTIFYEVENT_RETURN)		//edit回车事件
	{
		OnTextReturn(msg.pSender);
	}
	else if (msg.sType == NOTIFYEVENT_HEADER_CLICK)  //header click
	{
		OnHeaderClick(msg.pSender);
	}
	else if (msg.sType == NOTIFYEVENT_CHAR)  //char事件
	{
		OnChar(msg.pSender, (TCHAR)msg.wParam);
	}
	else if (msg.sType == NOTIFYEVENT_TIMER)  //timer事件
	{
		//如果是泡泡提示框的定时事件，隐藏定时器
		if (msg.pSender == m_pTipLabel)
		{
			TRACE(_T("SetTimer end\n"));
			HideTip();
		}

		OnTimer(msg.pSender, (DWORD)msg.wParam);
	}
	else  //其他事件目前未使用，暂不处理
	{
		//do nothing
	}
}

void CUIDialogFrameWnd::ShowExample(bool bShow, CControlUI* pControl)
{
	if (m_pExampleLabel == NULL)  //没有示例框
	{
		return;
	}

	if (!m_bShowExample) //已经禁用不显示，总是隐藏它
	{		
		m_pExampleLabel->SetVisible(false);
		return;
	}

	if (!bShow) //要求隐藏
	{
		m_pExampleLabel->SetVisible(false);
		return;
	}

	ASSERT(pControl);
	if (pControl == NULL)
	{
		return;
	}

	//目前只有edit支持该功能
	CEditUI* pEdit = (CEditUI*)pControl->GetInterface(_T("Edit"));
	if (pEdit == NULL)
	{
		return;
	}
	
	CStdString strText(pEdit->GetExampleText());
	if (strText.IsEmpty())  //没有示例文本
	{
		m_pExampleLabel->SetVisible(false);
	}
	else
	{			
		//根据示例文本的内容及控件位置计算示例框显示大小及位置
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
	//不做任何事情，为了满足框架，而定义实现该函数
	return 0;
}

LRESULT CUIDialogFrameWnd::PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	bHandled = FALSE;   //默认设置为未处理

	if (uMsg == m_nAcceptEventMessage)  //事件处理消息
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
		m_pm.MessageHandler(uMsg, wParam, lParam, lRes); //先交给绘画管理器处理
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
		bHandled = TRUE;  //框架窗口是没有边框风格，在此处理
		break;

	case WM_NCCALCSIZE:    
		lRes = OnNcCalcSize(uMsg, wParam, lParam);
		bHandled = TRUE;  //框架窗口是没有边框风格，在此处理
		break;

	case WM_NCPAINT:
		lRes = OnNcPaint(uMsg, wParam, lParam);
		bHandled = TRUE;  //框架窗口是没有边框风格，在此处理
		break;

	case WM_NCHITTEST:
		lRes = OnNcHitTest(uMsg, wParam, lParam);
		bHandled = TRUE; //默认实现，模拟标题栏，直接处理
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
		//鼠标滚动隐藏示例框和泡泡提示框
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

	default:  //其他消息不处理
		break;
	}

	return lRes;
}


LRESULT CUIDialogFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//不做任何事情，由子类重载实现自定义消息处理
	return 0;
}

LRESULT CUIDialogFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;   //默认设置为未处理

	lRes = PreTranslateMessage(uMsg, wParam, lParam, bHandled);  //预处理
	if (bHandled)   //已处理
	{
		return lRes;  //不继续处理
	}	

	// 处理自定义消息
	lRes = HandleCustomMessage(uMsg,wParam,lParam,bHandled);
	if (bHandled)   //已处理
	{
		return lRes;  //不继续处理
	}

	//交给绘画管理器处理，模拟逻辑控件消息	
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))  //已处理
	{
		return lRes; //不再往下继续处理
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);  //交给父类处理
}

}  // namespace DuiLib