// CopyFileToolV2Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCopyFileToolV2Dlg �Ի���
class CCopyFileToolV2Dlg : public CDialog
{
// ����
public:
	CCopyFileToolV2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COPYFILETOOLV2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


protected:
	void InitUIValue();			//��ʼ����������
	bool SaveFileListToFile(CString strFile);			//�����ļ��б��ļ���

protected:
	CString m_strExportFileListPath;			//������ļ��嵥·��������ʱʹ��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectFileList();
public:
	afx_msg void OnBnClickedButtonCopy();
public:
	afx_msg void OnBnClickedButtonX86Add();

public:
	afx_msg void OnBnClickedButtonX86Delete();
public:
	afx_msg void OnBnClickedButtonX86DeleteAll();
public:
	afx_msg void OnBnClickedButtonX64Add();

public:
	afx_msg void OnBnClickedButtonX64Delete();
public:
	afx_msg void OnBnClickedButtonX64DeleteAll();
public:
	afx_msg void OnBnClickedButtonImport();
public:
	afx_msg void OnBnClickedButton1Save();
public:
	afx_msg void OnBnClickedButtonExport();
public:
	afx_msg void OnBnClickedButtonIniToXml();
public:
	CEdit m_editFileList;
public:
	CListCtrl m_listX86FileList;
public:
	CListCtrl m_listX64FileList;
public:
	CButton m_btnX86Delete;
public:
	CButton m_btnX86DeleteAll;
public:
	CButton m_btnX64Delete;
public:
	CButton m_btnX64DeleteAll;
public:
	CEdit m_editProjectName;
public:
	CButton m_btnX86Edit;
public:
	CButton m_btnX64Edit;
public:
	afx_msg void OnBnClickedButtonX86Edit();
public:
	afx_msg void OnBnClickedButtonX64Edit();
};
