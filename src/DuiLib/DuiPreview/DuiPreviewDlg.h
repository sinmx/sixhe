// DuiPreviewDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CDuiPreviewDlg �Ի���
class CDuiPreviewDlg : public CDialog
{
// ����
public:
	CDuiPreviewDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DUIPREVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editXmlFile;
public:
	afx_msg void OnBnClickedButtonPreview();
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
