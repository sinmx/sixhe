// DuiPreviewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiPreview.h"
#include "DuiPreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDuiPreviewDlg 对话框




CDuiPreviewDlg::CDuiPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDuiPreviewDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDuiPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_XMLFILE, m_editXmlFile);
}

BEGIN_MESSAGE_MAP(CDuiPreviewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CDuiPreviewDlg::OnBnClickedButtonPreview)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CDuiPreviewDlg 消息处理程序

BOOL CDuiPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDuiPreviewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CDuiPreviewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDuiPreviewDlg::OnBnClickedButtonPreview()
{
	CString strXMLFile;
	m_editXmlFile.GetWindowText(strXMLFile);
	DuiLib::CUIDialogFrameWnd* pWndObj = new DuiLib::CUIDialogFrameWnd();
	if (pWndObj->Create(strXMLFile, m_hWnd, WS_VISIBLE|WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW) == NULL)  //fail
	{
		MessageBox(_T("Failed to create window."));
	}
	else
	{
		pWndObj->CenterWindow();
		pWndObj->ShowModal();
	}
}

void CDuiPreviewDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR buf[1024] = _T("");
	UINT nChars= ::DragQueryFile(hDropInfo, 0, buf, sizeof(buf)/sizeof(TCHAR));
	::DragFinish(hDropInfo );
	m_editXmlFile.SetWindowText(buf);
	OnBnClickedButtonPreview();

	CDialog::OnDropFiles(hDropInfo);
}
