
//
//CMainDlg	���������Ի���
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class CMainDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CMainDlg(void);
	~CMainDlg(void);

public:
	//��ȡͨ��ΨһIDֵ
	virtual int GetID();

	//�����¼���wParam lParam ����ֵ�����ھ����nEventCode
	virtual int AcceptEvent(int nEventCode, WPARAM wParam=0, LPARAM lParam=0);

protected:
	//���ڴ��������øú��������ظú�������ʼ������
	virtual void OnInitDialog();	

	//���ڵ����һ����Ϣ��WM_NCDESTORY,
	//����hWndָ���ô��ڵľ��ֵ����������Ч�ľ��
	virtual void OnFinalMessage(HWND hWnd);

	//����ͨ����Ϣ��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnClick(DuiLib::CControlUI* pSender);

	//ѡ��״̬�ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnSelectChange(DuiLib::CControlUI* pSender);	

	//��ϢWM_DESTROY������
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//ͨ��SendEvent���͵��¼���Ϣ�����������в���ͬSendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//�Զ�����Ϣ�������з��������ڵ��Զ�����Ϣ���ڸú����д���
	//��Ϣ�Ѿ���������bHandledΪTRUE�������ٴ���
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//��ȡ�����������ڶ���Ļص�������������ʱ����������
	virtual DuiLib::IUIWndContainerCallback* GetWndContainerCallback();

protected:  //��Ϣ������

	//�������ϵİ�ťѡ��״̬�����仯
	void OnOrderManageOptSelChange();
	void OnEatCodeOptSelChange();
	void OnRewardOptSelChange();
	void OnCustomerOptSelChange();	
	void OnSettingOptSelChange();

	void OnShowOrderPage();		//��ʾ���ӹ���ҳ��
	void OnShowCustomerPage();	//��ʾ��ׯ����ҳ��
	void OnShowEatCodePage();		//��ʾ�����ϱ�ҳ��
	void OnShowSettingPage();		//��ʾ����ҳ��

private:
	void GetLogicControlObject();		//��ȡ�߼��ؼ�����
	void InitLogicControlObject();		//��ʼ���߼��ؼ�����

private:
	DuiLib::CTabLayoutUI* m_pToolbarTab;		//������
	DuiLib::COptionUI* m_pOrderManageOpt;	//���ӹ���ť
	DuiLib::COptionUI* m_pCustomerOpt;			//��ׯ����ť
	DuiLib::COptionUI* m_pEatCodeOpt;			//�����ϱ���ť
	DuiLib::COptionUI* m_pSettingOpt;				//���ð�ť
};