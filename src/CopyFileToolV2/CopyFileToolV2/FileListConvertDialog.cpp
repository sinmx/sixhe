// FileListConvertDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CopyFileToolV2.h"
#include "FileListConvertDialog.h"
#include "LanguageManager.h"


// CFileListConvertDialog dialog

IMPLEMENT_DYNAMIC(CFileListConvertDialog, CDialog)

CFileListConvertDialog::CFileListConvertDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFileListConvertDialog::IDD, pParent)
	, m_strProjectName(_T(""))
	, m_strX86INI(_T(""))
	, m_strX64INI(_T(""))
	, m_strXMLSavePath(_T(""))
{

}

CFileListConvertDialog::~CFileListConvertDialog()
{
}

void CFileListConvertDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROJECT_NAME, m_editProjectName);
	DDX_Text(pDX, IDC_EDIT_PROJECT_NAME, m_strProjectName);
	DDV_MaxChars(pDX, m_strProjectName, 128);
	DDX_Control(pDX, IDC_EDIT_X86_INI, m_editX86INI);
	DDX_Text(pDX, IDC_EDIT_X86_INI, m_strX86INI);
	DDX_Control(pDX, IDC_EDIT_X64_INI, m_editX64INI);
	DDX_Text(pDX, IDC_EDIT_X64_INI, m_strX64INI);
	DDX_Control(pDX, IDC_EDIT_XML_SAVE_PATH, m_editXMLSavePath);
	DDV_MaxChars(pDX, m_strX86INI, 512);
	DDV_MaxChars(pDX, m_strX64INI, 512);
	DDX_Text(pDX, IDC_EDIT_XML_SAVE_PATH, m_strXMLSavePath);
	DDV_MaxChars(pDX, m_strXMLSavePath, 512);
}


BEGIN_MESSAGE_MAP(CFileListConvertDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_X86_INI, &CFileListConvertDialog::OnBnClickedButtonSelectX86Ini)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_X64_INI, &CFileListConvertDialog::OnBnClickedButtonSelectX64Ini)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CFileListConvertDialog::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDOK, &CFileListConvertDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CFileListConvertDialog message handlers

void CFileListConvertDialog::OnBnClickedButtonSelectX86Ini()
{
	//�����������û�ѡ��Ҫ������ļ�
	CFileDialog fileDlg(TRUE, _T("Ini"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Ini Files (*.ini)|*.ini||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
	{
		return;	
	}

	m_editX86INI.SetWindowText(fileDlg.GetPathName());	
}

void CFileListConvertDialog::OnBnClickedButtonSelectX64Ini()
{
	//�����������û�ѡ��Ҫ������ļ�
	CFileDialog fileDlg(TRUE, _T("Ini"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Ini Files (*.ini)|*.ini||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
	{
		return;	
	}

	m_editX64INI.SetWindowText(fileDlg.GetPathName());	
}

void CFileListConvertDialog::OnBnClickedButtonBrowse()
{
	//�����������û�ѡ��Ҫ�������ļ�λ��
	CFileDialog fileDlg(FALSE, _T("xml"), _T("filecopy.xml"), \
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Xml (*.xml)|*.xml||"), \
		CWnd::FromHandle(m_hWnd), sizeof(OPENFILENAME));	
	if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
	{
		return;
	}
	else
	{
		m_editXMLSavePath.SetWindowText(fileDlg.GetPathName());
	}	
}
void CFileListConvertDialog::OnBnClickedOk()
{
	//��ȡ����
	if (!UpdateData(TRUE))
	{
		return;
	}

	//У����������
	if (m_strProjectName.IsEmpty())
	{
		MessageBox(g_strLanguage[17], g_strLanguage[0], MB_OK); //��Ŀ���Ʋ���Ϊ��
		m_editProjectName.SetFocus();
		return;
	}

	if (m_strX86INI.IsEmpty() && m_strX64INI.IsEmpty())
	{
		MessageBox(g_strLanguage[18], g_strLanguage[0], MB_OK); //δѡ��INI�ļ�
		m_editX86INI.SetFocus();
		return;
	}

	if (m_strXMLSavePath.IsEmpty())
	{
		MessageBox(g_strLanguage[19], g_strLanguage[0], MB_OK); //δѡ��XML�ļ�����λ��
		m_editXMLSavePath.SetFocus();
		return;
	}

	//����Ĭ��
	OnOK();
}
