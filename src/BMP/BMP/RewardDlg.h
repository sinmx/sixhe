
//
//CRewardDlg	�ҽ�������
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class CRewardDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CRewardDlg(void);
	~CRewardDlg(void);

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

	//Edit�ı��س���������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnTextReturn(DuiLib::CControlUI* pSender);

	//ѡ����ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	//nCurSel ��ǰѡ����  nOldSel ��ѡ����
	virtual void OnItemSelect(DuiLib::CControlUI* pSender, int nCurSel, int nOldSel);

	//ͨ��SendEvent���͵��¼���Ϣ�����������в���ͬSendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

private:  //�¼�������
	void OnPrintRewardBtn();  //��ӡ�ҽ���Ϣ
	void OnOrderBtn();   //�ӵ�
	void OnDeleteAllOrderBtn();  //ɾ�����е���

	void OnWinCodeEditTextReturn();  //�н�����
	void OnSelfBuyDiscountEditTextReturn();  //�Լ������ˮ
	void OnSelfBuyBettingOddsEditTextReturn();  //�Լ���������

	void OnAlreadyRewardComboItemSelectChange(DuiLib::CComboUI* m_pControl);  //�Ƿ��Ѿ��ҽ��仯

	void OnOrderChange();		//���ӷ����仯

	void OnCustomerChange(int nChangeMask);  //�û������仯

private:
	void GetLogicControlObject();		//��ȡ�߼��ؼ�����
	void InitLogicControlObject();		//��ʼ���߼��ؼ�����
	void InitSelfProfitControlObject();		//��ʼ���Լ�����ؼ�

private:  //�߼��ؼ�����
	DuiLib::CContainerUI* m_pNoOrderContainer;		//û����ʱ�Ľ���
	DuiLib::CButtonUI* m_pOrderBtn;		//�ӵ�

	DuiLib::CEditUI* m_pWinCodeEdit;		//��������
	DuiLib::CButtonUI* m_pPrintRewardBtn;	//��ӡ�ҽ�

	DuiLib::CContainerUI* m_pConfigureWinCodeContainer;  //���öҽ���������ʾ����
	DuiLib::CContainerUI* m_pRewardContainer;	//�ҽ��б�����
	DuiLib::CListUI* m_pRewardList;			//�ҽ���ϸ��Ϣ��

	DuiLib::CContainerUI* m_pSelfProfitContainer;		//�Լ���������
	DuiLib::CContainerUI* m_pSelfBuyConfContainer;		//�Լ�������������
	DuiLib::CEditUI* m_pSelfBuyDiscountEdit;		//�Լ������ˮ
	DuiLib::CEditUI* m_pSelfBuyBettingOddsEdit;	//�Լ���������
	DuiLib::CLabelUI* m_pSelfProfitLabel;	//�Լ�����

	DuiLib::CButtonUI* m_pDeleteAllOrderBtn;		//ɾ�����е���
};