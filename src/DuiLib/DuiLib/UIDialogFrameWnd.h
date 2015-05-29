//
//������CUIDialogFrameWnd �Ի����ܴ�����
//���ܣ��Ի����ܴ����࣬�����жԻ��򴰿ڵĸ��ࡣʵ���˶Ի���Ļ������ܣ��رա�
//		��󻯡���С������Ϣ����Ŀ��
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
	//���ܣ����ٴ���
	virtual BOOL DestroyWindow();

	//���ܣ������Ի��򣬴������Ʊ���ͨ��XML�ļ���captionLabel���������ã�����ʾ����SetWindowText������
	//������lpszTemplateName  �������   ����ģ�壬���ڲ��ֵ�XML�ļ�
	//		hwndParent      �������    �����ھ��	
	//		dwStyle			�������    ���ڷ��ȡֵ�ο�API��Ĭ��ΪWS_VISIBLE|WS_OVERLAPPEDWINDOW
	//		dwExStyle		�������    ������չ���ȡֵ�ο�API��Ĭ��ΪWS_EX_APPWINDOW
	//����ֵ�����ھ����NULL��ʾ����ʧ��
	HWND Create(LPCTSTR lpszTemplateName, HWND hwndParent, DWORD dwStyle=WS_VISIBLE|WS_OVERLAPPEDWINDOW, DWORD dwExStyle=0);

	//���ܣ���ȡ�������
	//����ֵ���������
	int GetLastError();

	//���ܣ��жϴ����Ƿ�ɼ�
	BOOL IsWindowVisible();

	//���ܣ���ȡ���
	HWND GetSafeHwnd() const;

	//���ܣ���ʾ���ش���
	//������nCmdShow  �������  ȡֵͬShowWindow API
	BOOL ShowWindow(int nCmdShow);

	//����ȡ����ʱ���� nIDEvent ������ʱ��ID��uElapseMilliSec ��ʱ���ڣ���λ����
	//0x00-0xffff֮��Ķ�ʱ��Ϊ������ʱ��ID������ʹ��
	UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT uElapseMilliSec);	
	BOOL KillTimer(UINT_PTR nIDEvent);

	//���ܣ�����XML���¼�������
	//������lpszLanguageXml ������� ��ʾ����XML�ļ�
	BOOL LoadLanguage(LPCTSTR lpszLanguageXml);

	//���ܣ����ô����ı�����
	//������lpszString �������  �ı�����
	void SetWindowText(LPCTSTR lpszString);

	//�����Ƿ��Զ��ͷű�����
	void SetAutoDelete(bool bAutoDelete) {m_bAutoDelete = bAutoDelete;}

	//�����ڷ���һ���¼������¼���ͨ����ϢͶ�ݸ��������ڵ��߳̽��д�����֤�̰߳�ȫ
	//�봦���¼������Ը���OnAcceptEvent�麯����nCode�¼����ţ�wParam lParam���¼�����ķ���ֵ
	//��������ֵҲ��OnAcceptEvent����ֵ
	int SendEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//������ʾʾ�����ܣ���ôEdit�ؼ�������exampletext�󣬻�ȡ����ʱ���ڿؼ����Ϸ���ʾʾ���ı���ʧȥ����ʱ�����ı��������ҽ�������
	void EnableExampleVisible(bool bEnable) {m_bShowExample = bEnable;}

	//��ʾ����������ʾ��������������ʱ��������Ϣ�Զ�����
	//szTip ������Ϣ����
	//nDurationMilliSec ������ʾʱ�䳤�ȣ���λ���룬-1��ʾһֱ��ʾ
	//position ������Ϣ����ʾ�ڸÿؼ��м���Ϸ�
	//nAbsLeftPos nAbsTopPos ������Ϣ��ʾ��λ�ã�����ڴ������Ͻǵ�λ��
	void ShowTip(LPCTSTR szTip, int nDurationMilliSec, CControlUI* position);
	void ShowTip(LPCTSTR szTip, int nDurationMilliSec, int nAbsLeftPos, int nAbsTopPos);
	void HideTip();

protected:
	//��ȡ��ܴ��ڵĴ���������Ĭ���� DialogFrameWnd
	virtual LPCTSTR GetWindowClassName() const;

	//��ȡ�����߼��ؼ�����ص��������Ի�ؼ�ʱ����������
	virtual IDialogBuilderCallback* GetDialogBuilderCallback();

	//��ȡ�����������ڶ���Ļص�������������ʱ����������
	virtual IUIWndContainerCallback* GetWndContainerCallback();

	//���ڵ����һ����Ϣ��WM_NCDESTORY,
	//����hWndָ���ô��ڵľ��ֵ����������Ч�ľ��
	virtual void OnFinalMessage(HWND hWnd);

	//���ڴ��������øú��������ظú�������ʼ������
	virtual void OnInitDialog();

	//�߼��ؼ�ͨ����Ϣ������
	virtual void Notify(TNotifyUI& msg);

	//����ͨ����Ϣ��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnClick(CControlUI* pSender);

	//ѡ��״̬�ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnSelectChange(CControlUI* pSender);

	//ѡ����ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	//nCurSel ��ǰѡ����  nOldSel ��ѡ����
	virtual void OnItemSelect(CControlUI* pSender, int nCurSel, int nOldSel);

	//�ı��ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnTextChange(CControlUI* pSender);

	//Edit�ı��س���������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnTextReturn(CControlUI* pSender);

	//killfocus��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnKillFocus(CControlUI* pSender);

	//setfocus��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnSetFocus(CControlUI* pSender);

	//header click��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnHeaderClick(CControlUI* pSender);

	//char��������pSender����ͨ����Ϣ�Ŀؼ�����charΪ������ַ�
	virtual void OnChar(CControlUI* pSender, TCHAR ch);

	//ϵͳ��Ϣ���Զ�����Ϣ������
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//�Զ�����Ϣ�������з��������ڵ��Զ�����Ϣ���ڸú����д���
	//��Ϣ�Ѿ���������bHandledΪTRUE�������ٴ���
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//ȫ����Ϣ�ػ����������Ϣ�����̵߳�������Ϣ����ֻ�Ǳ����ڵ���Ϣ���Ѿ���������bHandledΪTRUE�������ٴ����ú����ķ���ֵ������
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	//��Ϣ�ػ�����Ϣ�Ѿ���������bHandledΪTRUE�������ٴ���
	virtual LRESULT PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//���ڹرմ��������Ѵ�����TRUE��δ������FALSE
	virtual BOOL OnClose();	

	//������󻯴��������Ѵ�����TRUE��δ������FALSE
	virtual BOOL OnMaximize();

	//������С�����������Ѵ�����TRUE��δ������FALSE
	virtual BOOL OnMinimize();

	//��ϢWM_CREATE������
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_DESTROY������
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_SHOWWINDOW������
	virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_NCACTIVATE������
	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_NCCALCSIZE������
	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_NCPAINT������
	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_NCHITTEST������
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_SIZE������
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_GETMINMAXINFO������
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_SYSCOMMAND������
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//��ϢWM_COMMAND������
	virtual void OnCommand(WPARAM wParam, LPARAM lParam);

	//��ϢWM_DRAWITEM������
	virtual void OnDrawItem(WPARAM wParam, LPARAM lParam);

	//��ϢWM_MEASUREITEM������
	virtual void OnMeasureItem(WPARAM wParam, LPARAM lParam);

	//��ϢWM_TIMER��������pSender���ΪNULL����ʾ���͸������ڣ� �Ѵ�����TRUE��δ������FALSE
	virtual BOOL OnTimer(CControlUI* pSender, DWORD dwTimerID);

	//ͨ��SendEvent���͵��¼���Ϣ�����������в���ͬSendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

protected:
	void ShowExample(bool bShow, CControlUI* pControl);		//��ʾ����ʾ���ı���pControlΪ��ʾ���ı��Ŀؼ�������ʱpControl����

protected:
	CPaintManagerUI m_pm;     //UI�滭������
	static UINT m_nAcceptEventMessage;		//Accept Event ��Ϣ��ͨ����ϵͳע���ȡ��

protected:
	CButtonUI* m_pCloseBtn;		//�رհ�ť
	CButtonUI* m_pMaxBtn;		//��󻯰�ť
	CButtonUI* m_pMinBtn;		//��С����ť
	CButtonUI* m_pRestoreBtn;	//�ָ���ť

	CLabelUI* m_pExampleLabel;	//ʾ���ı���
	bool m_bShowExample;		//��ʶ�Ƿ���ʾʾ��

	CStdString m_strLayoutFile;		//�����ı�
	int m_nLastError;				//�������
	bool m_bAutoDelete;			//�Զ��ͷŶ���Ĭ�ϲ����Զ��ͷŶ���

	CLabelUI* m_pTipLabel;	//������ʾ��
};

}  // namespace DuiLib