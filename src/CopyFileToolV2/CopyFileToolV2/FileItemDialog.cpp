// FileItemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CopyFileToolV2.h"
#include "FileItemDialog.h"
#include "LanguageManager.h"
#include "PathUtilEx.h"


// CFileItemDialog dialog

IMPLEMENT_DYNAMIC(CFileItemDialog, CDialog)

CFileItemDialog::CFileItemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFileItemDialog::IDD, pParent)
	, m_strName(_T(""))
	, m_strSourcePath(_T(""))
	, m_strDestPath(_T(""))
{

}

CFileItemDialog::~CFileItemDialog()
{
}

void CFileItemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SOURCE_PATH, m_editSourePath);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 256);
	DDX_Text(pDX, IDC_EDIT_SOURCE_PATH, m_strSourcePath);
	DDV_MaxChars(pDX, m_strSourcePath, 512);
	DDX_Text(pDX, IDC_EDIT_DESTINATION_PATH, m_strDestPath);
	DDV_MaxChars(pDX, m_strDestPath, 512);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_DESTINATION_PATH, m_editDestPath);
}


BEGIN_MESSAGE_MAP(CFileItemDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SOURCE_FILE, &CFileItemDialog::OnBnClickedButtonSelectSourceFile)
	ON_BN_CLICKED(IDOK, &CFileItemDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COPY_TO, &CFileItemDialog::OnBnClickedButtonCopyTo)
END_MESSAGE_MAP()


// CFileItemDialog message handlers

void CFileItemDialog::OnBnClickedButtonSelectSourceFile()
{
	//�����������û�ѡ��Ҫ������ļ�
	CFileDialog fileDlg(TRUE, NULL, NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("All Files (*.*)|*.*||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
	{
		return;	
	}

	//����Ϊ�û�ѡ����ļ�
	CString strSrcPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), fileDlg.GetPathName());
	m_editSourePath.SetWindowText(strSrcPath);
}

void CFileItemDialog::OnBnClickedOk()
{
	//��ȡ����
	if (!UpdateData(TRUE))
	{
		return;
	}

	//У������
	if (m_strName.IsEmpty())
	{
		MessageBox(g_strLanguage[7], g_strLanguage[0], MB_OK);  //���Ʋ���Ϊ��
		m_editName.SetFocus();
		return;
	}

	if (m_strSourcePath.IsEmpty())
	{
		MessageBox(g_strLanguage[8], g_strLanguage[0], MB_OK);  //Դ·������Ϊ��
		m_editSourePath.SetFocus();
		return;
	}

	if (m_strDestPath.IsEmpty())
	{
		MessageBox(g_strLanguage[9], g_strLanguage[0], MB_OK);  //Ŀ��·������Ϊ��
		m_editDestPath.SetFocus();
		return;
	}

	//����Ĭ�Ͻ���
	OnOK();
}

void CFileItemDialog::OnBnClickedButtonCopyTo()
{
	//��ȡԴ�ļ�����
	CString strSrcFile;
	m_editSourePath.GetWindowText(strSrcFile);
	int nIndex = strSrcFile.ReverseFind(_T('\\'));
	if (nIndex != -1)
	{
		strSrcFile = strSrcFile.Mid(nIndex+1);
	}	

	//�����������û�ѡ��Ҫ������ļ�
	CFileDialog fileDlg(FALSE, NULL, strSrcFile, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("All Files (*.*)|*.*||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
	{
		return;	
	}
	
	//����Ϊ�û�ѡ����ļ�
	CString strDestPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), fileDlg.GetPathName());
	m_editDestPath.SetWindowText(strDestPath);
}
