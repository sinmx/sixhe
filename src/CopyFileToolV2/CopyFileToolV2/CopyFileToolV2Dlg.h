// CopyFileToolV2Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCopyFileToolV2Dlg 对话框
class CCopyFileToolV2Dlg : public CDialog
{
// 构造
public:
	CCopyFileToolV2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COPYFILETOOLV2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


protected:
	void InitUIValue();			//初始化界面数据
	bool SaveFileListToFile(CString strFile);			//保存文件列表到文件中

protected:
	CString m_strExportFileListPath;			//导入的文件清单路径，保存时使用


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
