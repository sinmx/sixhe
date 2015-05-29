
//
//COrderManageDlg	���ӹ�����
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class COrderManageDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	COrderManageDlg(void);
	~COrderManageDlg(void);

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

	//ͨ��SendEvent���͵��¼���Ϣ�����������в���ͬSendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//ѡ����ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	//nCurSel ��ǰѡ����  nOldSel ��ѡ����
	virtual void OnItemSelect(DuiLib::CControlUI* pSender, int nCurSel, int nOldSel);

	//Edit�ı��س���������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnTextReturn(DuiLib::CControlUI* pSender);

	//�ı��ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnTextChange(DuiLib::CControlUI* pSender);

	//char��������pSender����ͨ����Ϣ�Ŀؼ�����charΪ������ַ�
	virtual void OnChar(DuiLib::CControlUI* pSender, TCHAR ch);

private:
	void GetLogicControlObject();		//��ȡ�߼��ؼ�����
	void InitLogicControlObject();		//��ʼ���߼��ؼ�����
	void InitCustomer();						//��ʼ����ׯ
	void InitOrder();							//��ʼ������
	DuiLib::CControlUI* AddCode(int nCode);				//���һ������
	DuiLib::CControlUI* FindCode(int nCode);  //�������в���ָ���ĺ���
	void InitBuyLog();							//��ʼ����ע��־
	void InitGroupBuy();						//��ʼ��������ע
	void AddGroup(DuiLib::CContainerUI* pContainer, int nGroupID);		//����һ����	
	void AddBuyLog(const CString& strBuyLog);  //���ӹ�����־

private:  //�¼�����Ϣ������
	void OnNewCustomer(const CString& strCustomer);		//����һ��ׯ
	void OnDeleteCustomer(const CString& strCustomer);		//ɾ��һ��ׯ
	void OnCustomerItemChange();	  //ѡ����һ����ׯ
	void OnAddCustomerBtn();			//�����ׯ

	void OnDeleteOrderBtn();				//ɾ����ǰ�û�����
	void OnDeleteAllOrderBtn();			//ɾ�����е���	
	void OnMoneyEditReturn(DuiLib::CControlUI* pMoneyEdit);		//���������ע

	void OnSingleBuyEditReturn();		//���Ŵ���ע��ע
	void OnSingleBuyStatus(bool bShow);	//���Ŵ�����״̬
	void OnSingleBuyEditTextChange();		//���Ź��򴮷����仯

	void OnIntervalBuyEditReturn();		//����ι���
	void OnEqualBuyEditReturn();			//�ȶ��
	void OnEqualCodeEditTextChange();  //�ȶ�����
	void OnSequenceBuyEditReturn();		//˳�Ź���
	void OnGroupBuyEditReturn();			//���鹺��

	void OnOrderChange(DWORD dwChangeMask);	//�������ݷ������

	void OnCustomerChange(int nChangeMask, const CString& strCustomer);  //��ׯ�û��������

	void OnCodeGroupSelChange(DuiLib::COptionUI* pControl);  //����ѡ��״̬�仯	

	void OnDeleteAllOrder();		//ɾ�����е���

private:  //�߼��ؼ�
	DuiLib::CContainerUI* m_pOrderContainer;		//���������
	DuiLib::CComboUI* m_pCustomerCombo;		//��ׯ�б�
	DuiLib::CButtonUI* m_pAddCustomerBtn;			//�����ׯ

	DuiLib::CButtonUI* m_pDeleteOrderBtn;			//ɾ������
	DuiLib::CButtonUI* m_pDeleteAllOrderBtn;		//ɾ�����е���	

	DuiLib::CContainerUI* m_pCodeContainer;		//��������
	DuiLib::CContainerUI* m_pLastCodeContainer;		//���һ����������
	
	DuiLib::CLabelUI* m_pTotalInfoLabel;			//�ܹ���ע��
	DuiLib::CRichEditUI* m_pBuyLogRichedit;		//��ע��־

	DuiLib::CEditUI* m_pSingleBuyEdit;		//������ע	
	DuiLib::CLabelUI* m_pSingleBuyWrongLabel;  //���Ź��򴮴���״ָ̬ʾͼ

	DuiLib::CEditUI* m_pIntervalStartEdit;	//����ι���ʼ����
	DuiLib::CEditUI* m_pIntervalEndEdit;		//����ι����������
	DuiLib::CEditUI* m_pIntervalBuyEdit;		//����ι���ע��

	DuiLib::CEditUI* m_pEqualCodeEdit;		//�ȶ�����
	DuiLib::CLabelUI* m_pEqualBuyWrongLabel;  //�ȶ�򴮴���״ָ̬ʾͼ
	DuiLib::CEditUI* m_pEqualBuyEdit;		//�ȶ����

	DuiLib::CEditUI* m_pStartCodeEdit;		//��ʼ����
	DuiLib::CEditUI* m_pSequenceBuyEdit;		//˳����ע

	DuiLib::CContainerUI* m_pCodeGroupContainer;		//��������
	DuiLib::CContainerUI* m_pLastCodeGroupContainer;	//���һ��������
	DuiLib::CContainerUI* m_pRecentContainer;		//���ʹ�÷����ǿ���������
	DuiLib::CContainerUI* m_pRecentGroupContainer;	//���ʹ�÷�������
	DuiLib::CEditUI* m_pGroupBuyEdit;		//���鹺��

	DuiLib::CContainerUI* m_pNoCustomerContainer;		//û����ׯ�û�ʱ��ʾ��ҳ������
	DuiLib::CButtonUI*	m_pAddCustomerBtnWhenNoCustomer;	//û����ׯ�û�ʱ�����ׯ
};