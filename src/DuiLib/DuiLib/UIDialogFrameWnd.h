//
//类名：CUIDialogFrameWnd 对话框框架窗口类
//功能：对话框框架窗口类，是所有对话框窗口的父类。实现了对话框的基本功能：关闭、
//		最大化、最小化、消息处理的框架
//

#pragma once

namespace DuiLib {

class IDialogBuilderCallback;
class UILIB_API CUIDialogFrameWnd :	public CWindowWnd, public INotifyUI, IMessageFilterUI
{
public:
	CUIDialogFrameWnd(void);
	virtual ~CUIDialogFrameWnd(void);

public:
	//功能：销毁窗口
	virtual BOOL DestroyWindow();

	//功能：创建对话框，窗口名称必须通过XML文件中captionLabel名称来设置，或显示调用SetWindowText来设置
	//参数：lpszTemplateName  输入参数   窗口模板，窗口布局的XML文件
	//		hwndParent      输入参数    父窗口句柄	
	//		dwStyle			输入参数    窗口风格，取值参考API，默认为WS_VISIBLE|WS_OVERLAPPEDWINDOW
	//		dwExStyle		输入参数    窗口扩展风格，取值参考API，默认为WS_EX_APPWINDOW
	//返回值：窗口句柄，NULL表示创建失败
	HWND Create(LPCTSTR lpszTemplateName, HWND hwndParent, DWORD dwStyle=WS_VISIBLE|WS_OVERLAPPEDWINDOW, DWORD dwExStyle=0);

	//功能：获取错误代号
	//返回值：错误代号
	int GetLastError();

	//功能：判断窗口是否可见
	BOOL IsWindowVisible();

	//功能：获取句柄
	HWND GetSafeHwnd() const;

	//功能：显示隐藏窗口
	//参数：nCmdShow  输入参数  取值同ShowWindow API
	BOOL ShowWindow(int nCmdShow);

	//设置取消定时器， nIDEvent 期望定时器ID，uElapseMilliSec 定时周期，单位毫秒
	//0x00-0xffff之间的定时器为保留定时器ID，不可使用
	UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT uElapseMilliSec);	
	BOOL KillTimer(UINT_PTR nIDEvent);

	//功能：根据XML重新加载语言
	//参数：lpszLanguageXml 输入参数 表示语言XML文件
	BOOL LoadLanguage(LPCTSTR lpszLanguageXml);

	//功能：设置窗口文本内容
	//参数：lpszString 输入参数  文本内容
	void SetWindowText(LPCTSTR lpszString);

	//设置是否自动释放本对象
	void SetAutoDelete(bool bAutoDelete) {m_bAutoDelete = bAutoDelete;}

	//给窗口发送一个事件，该事件将通过消息投递给窗口所在的线程进行处理，保证线程安全
	//想处理事件，可以覆盖OnAcceptEvent虚函数，nCode事件代号，wParam lParam该事件管理的返回值
	//函数返回值也是OnAcceptEvent返回值
	int SendEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//启用显示示例功能，那么Edit控件设置了exampletext后，获取焦点时将在控件的上方显示示例文本，失去焦点时隐藏文本，起到自我解释作用
	void EnableExampleVisible(bool bEnable) {m_bShowExample = bEnable;}

	//显示隐藏泡泡提示，单击或滚动鼠标时，泡泡消息自动隐藏
	//szTip 泡泡消息内容
	//nDurationMilliSec 泡泡显示时间长度，单位毫秒，-1表示一直显示
	//position 泡泡消息将显示在该控件中间的上方
	//nAbsLeftPos nAbsTopPos 泡泡消息显示的位置，相对于窗口左上角的位置
	void ShowTip(LPCTSTR szTip, int nDurationMilliSec, CControlUI* position);
	void ShowTip(LPCTSTR szTip, int nDurationMilliSec, int nAbsLeftPos, int nAbsTopPos);
	void HideTip();

protected:
	//获取框架窗口的窗口类名，默认是 DialogFrameWnd
	virtual LPCTSTR GetWindowClassName() const;

	//获取生成逻辑控件对象回调，当有自绘控件时，进行重载
	virtual IDialogBuilderCallback* GetDialogBuilderCallback();

	//获取生成容器窗口对象的回调，有容器窗口时，进行重载
	virtual IUIWndContainerCallback* GetWndContainerCallback();

	//窗口的最后一条消息即WM_NCDESTORY,
	//参数hWnd指明该窗口的句柄值，但已是无效的句柄
	virtual void OnFinalMessage(HWND hWnd);

	//窗口创建后会调用该函数，重载该函数做初始化工作
	virtual void OnInitDialog();

	//逻辑控件通告消息处理函数
	virtual void Notify(TNotifyUI& msg);

	//单击通告消息处理函数，pSender发送通告消息的控件对象
	virtual void OnClick(CControlUI* pSender);

	//选择状态改变处理函数，pSender发送通告消息的控件对象
	virtual void OnSelectChange(CControlUI* pSender);

	//选中项改变处理函数，pSender发送通告消息的控件对象
	//nCurSel 当前选中项  nOldSel 旧选中项
	virtual void OnItemSelect(CControlUI* pSender, int nCurSel, int nOldSel);

	//文本改变处理函数，pSender发送通告消息的控件对象
	virtual void OnTextChange(CControlUI* pSender);

	//Edit文本回车处理函数，pSender发送通告消息的控件对象
	virtual void OnTextReturn(CControlUI* pSender);

	//killfocus处理函数，pSender发送通告消息的控件对象
	virtual void OnKillFocus(CControlUI* pSender);

	//setfocus处理函数，pSender发送通告消息的控件对象
	virtual void OnSetFocus(CControlUI* pSender);

	//header click处理函数，pSender发送通告消息的控件对象
	virtual void OnHeaderClick(CControlUI* pSender);

	//char处理函数，pSender发送通告消息的控件对象，char为输入的字符
	virtual void OnChar(CControlUI* pSender, TCHAR ch);

	//系统消息或自定义消息处理函数
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//自定义消息处理，所有发到本窗口的自定义消息都在该函数中处理，
	//消息已经处理设置bHandled为TRUE，将不再处理
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//全局消息截获处理，这里的消息包括线程的所有消息，不只是本窗口的消息，已经处理设置bHandled为TRUE，将不再处理，该函数的返回值被忽略
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	//消息截获处理，消息已经处理设置bHandled为TRUE，将不再处理，
	virtual LRESULT PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//窗口关闭处理函数，已处理返回TRUE，未处理返回FALSE
	virtual BOOL OnClose();	

	//窗口最大化处理函数，已处理返回TRUE，未处理返回FALSE
	virtual BOOL OnMaximize();

	//窗口最小化处理函数，已处理返回TRUE，未处理返回FALSE
	virtual BOOL OnMinimize();

	//消息WM_CREATE处理函数
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_DESTROY处理函数
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_SHOWWINDOW处理函数
	virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_NCACTIVATE处理函数
	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_NCCALCSIZE处理函数
	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_NCPAINT处理函数
	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_NCHITTEST处理函数
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_SIZE处理函数
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_GETMINMAXINFO处理函数
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_SYSCOMMAND处理函数
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//消息WM_COMMAND处理函数
	virtual void OnCommand(WPARAM wParam, LPARAM lParam);

	//消息WM_DRAWITEM处理函数
	virtual void OnDrawItem(WPARAM wParam, LPARAM lParam);

	//消息WM_MEASUREITEM处理函数
	virtual void OnMeasureItem(WPARAM wParam, LPARAM lParam);

	//消息WM_TIMER处理函数，pSender如果为NULL，表示发送给物理窗口， 已处理返回TRUE，未处理返回FALSE
	virtual BOOL OnTimer(CControlUI* pSender, DWORD dwTimerID);

	//通过SendEvent发送的事件消息处理函数，其中参数同SendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

protected:
	void ShowExample(bool bShow, CControlUI* pControl);		//显示隐藏示例文本，pControl为有示例文本的控件，隐藏时pControl忽略

protected:
	CPaintManagerUI m_pm;     //UI绘画管理器
	static UINT m_nAcceptEventMessage;		//Accept Event 消息，通过向系统注册获取的

protected:
	CButtonUI* m_pCloseBtn;		//关闭按钮
	CButtonUI* m_pMaxBtn;		//最大化按钮
	CButtonUI* m_pMinBtn;		//最小化按钮
	CButtonUI* m_pRestoreBtn;	//恢复按钮

	CLabelUI* m_pExampleLabel;	//示例文本框
	bool m_bShowExample;		//标识是否显示示例

	CStdString m_strLayoutFile;		//窗口文本
	int m_nLastError;				//错误代号
	bool m_bAutoDelete;			//自动释放对象，默认不会自动释放对象

	CLabelUI* m_pTipLabel;	//泡泡提示框
};

}  // namespace DuiLib