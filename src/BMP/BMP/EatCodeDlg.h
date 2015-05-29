
//
//CEatCodeDlg	�����ϱ�����
//

#pragma once

#include "..\..\common\CommunicationManager.h"
#include "OrderManager.h"

class CEatCodeDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CEatCodeDlg(void);
	~CEatCodeDlg(void);

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

	//�ı��ı䴦������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnTextChange(DuiLib::CControlUI* pSender);

	//ͨ��SendEvent���͵��¼���Ϣ�����������в���ͬSendEvent
	virtual LRESULT OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam);

	//char��������pSender����ͨ����Ϣ�Ŀؼ�����charΪ������ַ�
	virtual void OnChar(DuiLib::CControlUI* pSender, TCHAR ch);

private:  //�¼�������
	void OnAvgDiscountEditReturn();		//ƽ����ˮ
	void OnAvgBettingOddsEditReturn();		//ƽ������
	void OnEatEditReturn(DuiLib::CControlUI* pControl);  //��������
	void OnAvgEatEditReturn();		//ƽ������
	void OnCalcEatEditReturn();		//���������	

	void OnCancelEatBtn();		//ȡ������
	void OnPrintSellBtn();		//��ӡ�ϱ�	
	void OnOrderBtn();		//�ӵ�

	void OnOrderChange();		//���ӷ����仯

private:
	void GetLogicControlObject();		//��ȡ�߼��ؼ�����
	void InitLogicControlObject();		//��ʼ���߼��ؼ�����
	void InitEat();			//��ʼ��������ؿؼ�
	void AddOneCode(DuiLib::CContainerUI* pContainer, int nCode);		//����һ����	

private:  //�߼��ؼ�
	DuiLib::CContainerUI* m_pNoOrderContainer;		//û�ж�������
	DuiLib::CButtonUI* m_pOrderBtn;		//�ӵ�

	DuiLib::CEditUI* m_pAvgDiscountEdit;		//ƽ����ˮ
	DuiLib::CEditUI* m_pAvgBettingOddsEdit;		//ƽ������

	DuiLib::CButtonUI* m_pCancelEatBtn;		//ȡ������
	DuiLib::CButtonUI* m_pPrintSellBtn;		//��ӡ�ϱ�
	DuiLib::CLabelUI* m_pMinMaxProfitLabel;  //��С�������

	DuiLib::CListUI* m_pFirstColumnList;		//��һ�к���
	DuiLib::CListUI* m_pSecondColumnList;		//�ڶ��к���
	DuiLib::CListUI* m_pThirdColumnList;		//�����к���	

	DuiLib::CLabelUI* m_pTotalInfoLabel;	//������Ϣ

	DuiLib::CLabelUI* m_pSuggestionLabel;		//����ƽ������
	DuiLib::CEditUI* m_pAvgEatEdit;		//ƽ������

	DuiLib::CEditUI* m_pCalcEatEdit;		//��������¼������
	DuiLib::CLabelUI* m_pCalcAvgEatEdit;	//����ƽ������		
};