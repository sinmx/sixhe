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
	//弹出界面让用户选择要导入的文件
	CFileDialog fileDlg(TRUE, _T("Ini"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Ini Files (*.ini)|*.ini||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //用户取消了
	{
		return;	
	}

	m_editX86INI.SetWindowText(fileDlg.GetPathName());	
}

void CFileListConvertDialog::OnBnClickedButtonSelectX64Ini()
{
	//弹出界面让用户选择要导入的文件
	CFileDialog fileDlg(TRUE, _T("Ini"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Ini Files (*.ini)|*.ini||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //用户取消了
	{
		return;	
	}

	m_editX64INI.SetWindowText(fileDlg.GetPathName());	
}

void CFileListConvertDialog::OnBnClickedButtonBrowse()
{
	//弹出界面让用户选择要导出的文件位置
	CFileDialog fileDlg(FALSE, _T("xml"), _T("filecopy.xml"), \
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Xml (*.xml)|*.xml||"), \
		CWnd::FromHandle(m_hWnd), sizeof(OPENFILENAME));	
	if (fileDlg.DoModal() != IDOK)  //用户取消了
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
	//读取数据
	if (!UpdateData(TRUE))
	{
		return;
	}

	//校验输入数据
	if (m_strProjectName.IsEmpty())
	{
		MessageBox(g_strLanguage[17], g_strLanguage[0], MB_OK); //项目名称不能为空
		m_editProjectName.SetFocus();
		return;
	}

	if (m_strX86INI.IsEmpty() && m_strX64INI.IsEmpty())
	{
		MessageBox(g_strLanguage[18], g_strLanguage[0], MB_OK); //未选择INI文件
		m_editX86INI.SetFocus();
		return;
	}

	if (m_strXMLSavePath.IsEmpty())
	{
		MessageBox(g_strLanguage[19], g_strLanguage[0], MB_OK); //未选择XML文件保存位置
		m_editXMLSavePath.SetFocus();
		return;
	}

	//调用默认
	OnOK();
}
