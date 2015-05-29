
//
//CSettingDlg	���ô��ڶ���
//

#pragma once

#include "..\..\common\CommunicationManager.h"

class CSettingDlg : public DuiLib::CUIDialogFrameWnd , public ICommunication
{
public:
	CSettingDlg(void);
	~CSettingDlg(void);

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

	//killfocus��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnKillFocus(DuiLib::CControlUI* pSender);

	//��ϢWM_SHOWWINDOW������
	virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:  //�¼�������
	void OnGoldEditKillFocus();  //����
	void OnWoodEditKillFocus();
	void OnWaterEditKillFocus();
	void OnFireEditKillFocus();
	void OnEarthEditKillFocus();

	void OnWuHangTextChange(DuiLib::CControlUI* pEdit);  //�������ݷ����仯

	void OnSoundCheckOptSelChange();		//����У��

private:
	void GetLogicControlObject();		//��ȡ�߼��ؼ�����
	void InitLogicControlObject();		//��ʼ���߼��ؼ�����

private:  //�߼��ؼ�
	DuiLib::CEditUI* m_pGoldEdit;		//��
	DuiLib::CEditUI* m_pWoodEdit;	//ľ
	DuiLib::CEditUI* m_pWaterEdit;  //ˮ
	DuiLib::CEditUI* m_pFireEdit;  //��
	DuiLib::CEditUI* m_pEarthEdit;  //��
	DuiLib::COptionUI* m_pSoundCheckOpt;  //����У��
};