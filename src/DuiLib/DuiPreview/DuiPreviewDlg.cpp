// DuiPreviewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuiPreview.h"
#include "DuiPreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDuiPreviewDlg �Ի���




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


// CDuiPreviewDlg ��Ϣ�������

BOOL CDuiPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDuiPreviewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
