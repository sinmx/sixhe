
//
//CCustomerDlg	��ׯ�û�������
//

#pragma once

#include "CustomerManager.h"

class CCustomerDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CCustomerDlg(void);
	~CCustomerDlg(void);

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

	//killfocus��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnKillFocus(DuiLib::CControlUI* pSender);

	//��ϢWM_SHOWWINDOW������
	virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

private: //�¼�������
	void OnCustomerAddBtn();			//����û�
	void OnCustomerDeleteBtn(DuiLib::CControlUI* pDelBtn);  //ɾ���û�
	void OnCustomerItemKillFocus(DuiLib::CControlUI* pControl);		//�û������ñ��
	void OnOrderBtn();		//�ӵ�

private:
	void GetLogicControlObject();		//��ȡ�߼��ؼ�����
	void InitLogicControlObject();		//��ʼ���߼��ؼ�����
	void InitCustomerList();					//��ʼ���û��б�

private:
	void AddOneCustomer(const CUSTOMER& customer, bool bNeedFocus);			//���һ���û����û��б�ؼ���
	
private: //�߼��ؼ�����
	DuiLib::CContainerUI* m_pCustomerContainer;	
	DuiLib::CListUI* m_pCustomerList;
	DuiLib::CButtonUI* m_pCustomerAddBtn;
};