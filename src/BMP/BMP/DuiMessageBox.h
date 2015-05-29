#pragma once

//ȷ������ʱ����IDOK��ȡ�����ʱ����IDCANCEL
UINT DuiMessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType );

class CDuiMessageBoxDlg :	public DuiLib::CUIDialogFrameWnd
{
public:
	CDuiMessageBoxDlg(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
	~CDuiMessageBoxDlg(void);

protected:
	//���ڴ��������øú��������ظú�������ʼ������
	virtual void OnInitDialog();

	//����ͨ����Ϣ��������pSender����ͨ����Ϣ�Ŀؼ�����
	virtual void OnClick(DuiLib::CControlUI* pSender);

private:
	void GetLoginControlObject();		//��ȡ�߼��ؼ�����
	int GetTextHeightByWidth(DuiLib::CStdString strText,int nWidth,int iFont);
	void InitMessageType(UINT uType);

private:  //�߼��ؼ�
	DuiLib::CTextUI* m_pMsgText;
	DuiLib::CButtonUI* m_pOKBtn;
	DuiLib::CButtonUI* m_pCancelBtn;
	DuiLib::CControlUI* m_pSpaceCtrl;
	DuiLib::CControlUI* m_pIconCtrl;

private:
	DuiLib::CStdString m_strText;
	DuiLib::CStdString m_strCaption;
	UINT m_uType;
	bool m_bIsChineseOS;		//��־�Ƿ�Ϊ���Ĳ���ϵͳ
};
