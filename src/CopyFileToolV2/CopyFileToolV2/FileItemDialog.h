#pragma once
#include "afxwin.h"


// CFileItemDialog dialog

class CFileItemDialog : public CDialog
{
	DECLARE_DYNAMIC(CFileItemDialog)

public:
	CFileItemDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileItemDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_FILE_ITEM_INFORMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectSourceFile();
public:
	CEdit m_editSourePath;
public:
	CString m_strName;
public:
	CString m_strSourcePath;
public:
	CString m_strDestPath;
public:
	afx_msg void OnBnClickedOk();
public:
	CEdit m_editName;
public:
	CEdit m_editDestPath;
public:
	afx_msg void OnBnClickedButtonCopyTo();
};
