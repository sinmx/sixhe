#pragma once
#include "afxwin.h"


// CFileListConvertDialog dialog

class CFileListConvertDialog : public CDialog
{
	DECLARE_DYNAMIC(CFileListConvertDialog)

public:
	CFileListConvertDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileListConvertDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_FILE_LIST_CONVERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectX86Ini();
public:
	CEdit m_editProjectName;
public:
	CString m_strProjectName;
public:
	CEdit m_editX86INI;
public:
	CString m_strX86INI;
public:
	CEdit m_editX64INI;
public:
	CString m_strX64INI;
public:
	CEdit m_editXMLSavePath;
public:
	CString m_strXMLSavePath;
public:
	afx_msg void OnBnClickedButtonSelectX64Ini();
public:
	afx_msg void OnBnClickedButtonBrowse();
public:
	afx_msg void OnBnClickedOk();
};
