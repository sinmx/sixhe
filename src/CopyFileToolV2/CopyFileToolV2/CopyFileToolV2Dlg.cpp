// CopyFileToolV2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CopyFileToolV2.h"
#include "CopyFileToolV2Dlg.h"
#include "FileCopyUtil.h"
#include "FileItemDialog.h"
#include "FileListConvertDialog.h"
#include "LanguageManager.h"
#include "FileListUtil.h"
#include "PathUtilEx.h"
#include "shlwapi.h"

#pragma comment (lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCopyFileToolV2Dlg �Ի���




CCopyFileToolV2Dlg::CCopyFileToolV2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyFileToolV2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCopyFileToolV2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILE_LIST, m_editFileList);
	DDX_Control(pDX, IDC_LIST_X86_FILE_LIST, m_listX86FileList);
	DDX_Control(pDX, IDC_LIST_X64_FILE_LIST, m_listX64FileList);
	DDX_Control(pDX, IDC_BUTTON_X86_DELETE, m_btnX86Delete);
	DDX_Control(pDX, IDC_BUTTON_X86_DELETE_ALL, m_btnX86DeleteAll);
	DDX_Control(pDX, IDC_BUTTON_X64_DELETE, m_btnX64Delete);
	DDX_Control(pDX, IDC_BUTTON_X64_DELETE_ALL, m_btnX64DeleteAll);
	DDX_Control(pDX, IDC_EDIT_PROJECT_NAME, m_editProjectName);
	DDX_Control(pDX, IDC_BUTTON_X86_EDIT, m_btnX86Edit);
	DDX_Control(pDX, IDC_BUTTON_X64_EDIT, m_btnX64Edit);
}

BEGIN_MESSAGE_MAP(CCopyFileToolV2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FILE_LIST, &CCopyFileToolV2Dlg::OnBnClickedButtonSelectFileList)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CCopyFileToolV2Dlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_X86_ADD, &CCopyFileToolV2Dlg::OnBnClickedButtonX86Add)
	ON_BN_CLICKED(IDC_BUTTON_X86_DELETE, &CCopyFileToolV2Dlg::OnBnClickedButtonX86Delete)
	ON_BN_CLICKED(IDC_BUTTON_X86_DELETE_ALL, &CCopyFileToolV2Dlg::OnBnClickedButtonX86DeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_X64_ADD, &CCopyFileToolV2Dlg::OnBnClickedButtonX64Add)
	ON_BN_CLICKED(IDC_BUTTON_X64_DELETE, &CCopyFileToolV2Dlg::OnBnClickedButtonX64Delete)
	ON_BN_CLICKED(IDC_BUTTON_X64_DELETE_ALL, &CCopyFileToolV2Dlg::OnBnClickedButtonX64DeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CCopyFileToolV2Dlg::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON1_SAVE, &CCopyFileToolV2Dlg::OnBnClickedButton1Save)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CCopyFileToolV2Dlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_INI_TO_XML, &CCopyFileToolV2Dlg::OnBnClickedButtonIniToXml)
	ON_BN_CLICKED(IDC_BUTTON_X86_EDIT, &CCopyFileToolV2Dlg::OnBnClickedButtonX86Edit)
	ON_BN_CLICKED(IDC_BUTTON_X64_EDIT, &CCopyFileToolV2Dlg::OnBnClickedButtonX64Edit)
END_MESSAGE_MAP()


// CCopyFileToolV2Dlg ��Ϣ�������

BOOL CCopyFileToolV2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//��ʼ����������
	InitUIValue();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCopyFileToolV2Dlg::InitUIValue()
{
	//����list�ؼ���ͷ��
	m_listX86FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listX86FileList.InsertColumn(0, g_strLanguage[10], LVCFMT_LEFT, 100);  //����
	m_listX86FileList.InsertColumn(1, g_strLanguage[11], LVCFMT_LEFT, 200);	//Դ·��
	m_listX86FileList.InsertColumn(2, g_strLanguage[12], LVCFMT_LEFT, 200);	//Ŀ��·��

	m_listX64FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listX64FileList.InsertColumn(0, g_strLanguage[10], LVCFMT_LEFT, 100);  //����
	m_listX64FileList.InsertColumn(1, g_strLanguage[11], LVCFMT_LEFT, 200);	//Դ·��
	m_listX64FileList.InsertColumn(2, g_strLanguage[12], LVCFMT_LEFT, 200);	//Ŀ��·��
}

void CCopyFileToolV2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCopyFileToolV2Dlg::OnPaint()
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
HCURSOR CCopyFileToolV2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCopyFileToolV2Dlg::OnBnClickedButtonSelectFileList()
{
	//�����������û�ѡ��Ҫ������ļ�
	CFileDialog fileDlg(TRUE, _T("xml"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Xml Files (*.xml)|*.xml||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
	{
		return;	
	}

	//����Ϊ�û�ѡ����ļ�
	m_editFileList.SetWindowText(fileDlg.GetPathName());
}

void CCopyFileToolV2Dlg::OnBnClickedButtonCopy()
{
	//У���嵥�ļ�·���Ƿ�Ϊ��
	CString strFileList;
	m_editFileList.GetWindowText(strFileList);
	if (strFileList.IsEmpty())
	{
		MessageBox(g_strLanguage[1], g_strLanguage[0], MB_OK);  //�ļ��嵥����Ϊ��
		m_editFileList.SetFocus();
		return;
	}

	//��ȡ�����б�
	CFileListXml fileList;
	if (fileList.Load(strFileList) != 0)
	{
		MessageBox(g_strLanguage[2], g_strLanguage[0], MB_OK);  //�ļ��嵥��ʽ���ԣ�����ʧ��
		return;	
	}

	//�����б��������
	COPY_RESULT_INFORMATION result;
	CFileCopyUtil::Copy(fileList, result);
	
	//���ݿ��������ʾ�û�
	CString strHint;
	CString strTemp;
	strTemp.Format(g_strLanguage[3], fileList.GetProjectName());	//��Ŀ����
	strHint += strTemp+_T("\r\n\r\n");

	if (result.nResult == 0) //success
	{
		strHint += g_strLanguage[4]+_T("\r\n\r\n");			//�����ɹ�
	}
	else //fail
	{
		strHint += g_strLanguage[5]+result.failFileItem.strName+_T("\r\n\r\n");	 //����ʧ�ܣ�ʧ���ļ�Ϊ��
	}

	if (result.newFileList.size() > 0)  //�������ļ�
	{
		strHint += g_strLanguage[6]+_T("\r\n");  //�ɹ����������ļ�
		for (vector<FILE_ITEM>::const_iterator it=result.newFileList.begin(); it!=result.newFileList.end(); it++)
		{
			strHint += it->strName + _T("\r\n");
		}
	}

	MessageBox(strHint, g_strLanguage[0], MB_OK);
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX86Add()
{
	CFileItemDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		int nItemIndex = m_listX86FileList.InsertItem(0x0fffffff, dlg.m_strName);

		//ת��Ϊ����ڿ������ߵ����·��
		CString strSrcPath = dlg.m_strSourcePath;
		if (!PathIsRelative(dlg.m_strSourcePath))
		{
			strSrcPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strSourcePath);
		}

		CString strDestPath = dlg.m_strDestPath;
		if (!PathIsRelative(dlg.m_strDestPath))
		{
			strDestPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strDestPath);
		}

		if (strSrcPath.IsEmpty() || strDestPath.IsEmpty())
		{
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //·����Ч
			return;
		}
		m_listX86FileList.SetItemText(nItemIndex, 1, strSrcPath);
		m_listX86FileList.SetItemText(nItemIndex, 2, strDestPath);
		m_btnX86Delete.EnableWindow(TRUE);
		m_btnX86DeleteAll.EnableWindow(TRUE);	
		m_btnX86Edit.EnableWindow(TRUE);
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX86Delete()
{
	UINT uSelectCount = m_listX86FileList.GetSelectedCount();
	if (0 == uSelectCount)
	{
		MessageBox(g_strLanguage[13], g_strLanguage[0], MB_OK); //��ѡ��һ��ɾ����
		return;
	}

	for (UINT i=0; i<uSelectCount; i++)
	{
		int nItem = m_listX86FileList.GetNextItem(-1, LVNI_SELECTED);
		ASSERT(nItem != -1);
		m_listX86FileList.DeleteItem(nItem);
	}

	if (m_listX86FileList.GetItemCount() == 0)  //û���κβ�����Ͳ�����ɾ��
	{
		m_btnX86Delete.EnableWindow(FALSE);
		m_btnX86DeleteAll.EnableWindow(FALSE);
		m_btnX86Edit.EnableWindow(FALSE);
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX86DeleteAll()
{
	m_listX86FileList.DeleteAllItems();
	m_btnX86Delete.EnableWindow(FALSE);
	m_btnX86DeleteAll.EnableWindow(FALSE);
	m_btnX86Edit.EnableWindow(FALSE);
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX64Add()
{
	CFileItemDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		int nItemIndex = m_listX64FileList.InsertItem(0x0fffffff, dlg.m_strName);

		//ת��Ϊ����ڿ������ߵ����·��
		//ת��Ϊ����ڿ������ߵ����·��
		CString strSrcPath = dlg.m_strSourcePath;
		if (!PathIsRelative(dlg.m_strSourcePath))
		{
			strSrcPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strSourcePath);
		}

		CString strDestPath = dlg.m_strDestPath;
		if (!PathIsRelative(dlg.m_strDestPath))
		{
			strDestPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strDestPath);
		}

		if (strSrcPath.IsEmpty() || strDestPath.IsEmpty())
		{
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //·����Ч
			return;
		}
		m_listX64FileList.SetItemText(nItemIndex, 1, strSrcPath);
		m_listX64FileList.SetItemText(nItemIndex, 2, strDestPath);
		m_btnX64Delete.EnableWindow(TRUE);
		m_btnX64DeleteAll.EnableWindow(TRUE);
		m_btnX64Edit.EnableWindow(TRUE);
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX64Delete()
{
	UINT uSelectCount = m_listX64FileList.GetSelectedCount();
	if (0 == uSelectCount)
	{
		MessageBox(g_strLanguage[13], g_strLanguage[0], MB_OK); //��ѡ��һ��ɾ����
		return;
	}

	for (UINT i=0; i<uSelectCount; i++)
	{
		int nItem = m_listX64FileList.GetNextItem(-1, LVNI_SELECTED);
		ASSERT(nItem != -1);
		m_listX64FileList.DeleteItem(nItem);
	}

	if (m_listX64FileList.GetItemCount() == 0)  //û���κβ�����Ͳ�����ɾ��
	{
		m_btnX64Delete.EnableWindow(FALSE);
		m_btnX64DeleteAll.EnableWindow(FALSE);
		m_btnX64Edit.EnableWindow(FALSE);
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX64DeleteAll()
{
	m_listX64FileList.DeleteAllItems();
	m_btnX64Delete.EnableWindow(FALSE);
	m_btnX64DeleteAll.EnableWindow(FALSE);
	m_btnX64Edit.EnableWindow(FALSE);
}

void CCopyFileToolV2Dlg::OnBnClickedButtonImport()
{
	//һ����������Ϊ��
	m_strExportFileListPath = _T("");

	//�����������û�ѡ��Ҫ������ļ�
	CFileDialog fileDlg(TRUE, _T("xml"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Xml Files (*.xml)|*.xml||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
	{
		return;	
	}

	//�����û�ѡ����ļ�����
	CFileListXml fileList;
	int nResult = fileList.Load(fileDlg.GetPathName());
	if (nResult != 0) //fail
	{
		MessageBox(g_strLanguage[2], g_strLanguage[0], MB_OK);  //�ļ��嵥��ʽ���ԣ�����ʧ��
		return;	
	}

	//���µ�������
	m_editProjectName.SetWindowText(fileList.GetProjectName());

	m_listX86FileList.DeleteAllItems();
	vector<FILE_ITEM>& vectorX86FileItem = fileList.GetX86FileList();
	for (vector<FILE_ITEM>::const_iterator it=vectorX86FileItem.begin(); it!=vectorX86FileItem.end(); it++)
	{
		int nItemIndex = m_listX86FileList.InsertItem(0x0fffffff, it->strName);
		m_listX86FileList.SetItemText(nItemIndex, 1, it->strSrcPath);
		m_listX86FileList.SetItemText(nItemIndex, 2, it->strDestPath);		
	}

	if (m_listX86FileList.GetItemCount() > 0)
	{
		m_btnX86Delete.EnableWindow(TRUE);
		m_btnX86DeleteAll.EnableWindow(TRUE);	
		m_btnX86Edit.EnableWindow(TRUE);
	}
	else
	{
		m_btnX86Delete.EnableWindow(FALSE);
		m_btnX86DeleteAll.EnableWindow(FALSE);	
		m_btnX86Edit.EnableWindow(FALSE);
	}

	m_listX64FileList.DeleteAllItems();
	vector<FILE_ITEM>& vectorX64FileItem = fileList.GetX64FileList();
	for (vector<FILE_ITEM>::const_iterator it=vectorX64FileItem.begin(); it!=vectorX64FileItem.end(); it++)
	{
		int nItemIndex = m_listX64FileList.InsertItem(0x0fffffff, it->strName);
		m_listX64FileList.SetItemText(nItemIndex, 1, it->strSrcPath);
		m_listX64FileList.SetItemText(nItemIndex, 2, it->strDestPath);
	}

	if (m_listX64FileList.GetItemCount() > 0)
	{
		m_btnX64Delete.EnableWindow(TRUE);
		m_btnX64DeleteAll.EnableWindow(TRUE);	
		m_btnX64Edit.EnableWindow(TRUE);
	}
	else
	{
		m_btnX64Delete.EnableWindow(FALSE);
		m_btnX64DeleteAll.EnableWindow(FALSE);	
		m_btnX64Edit.EnableWindow(FALSE);
	}

	//��ʾ�û�����ɹ�
	m_strExportFileListPath = fileDlg.GetPathName();
	MessageBox(g_strLanguage[14], g_strLanguage[0], MB_OK);
}

void CCopyFileToolV2Dlg::OnBnClickedButton1Save()
{
	//���浽�ļ�
	if (SaveFileListToFile(m_strExportFileListPath))
	{
		MessageBox(g_strLanguage[16], g_strLanguage[0], MB_OK);  //����ɹ�
		return;
	}
}

bool CCopyFileToolV2Dlg::SaveFileListToFile(CString strFile)
{
	//У�����
	CString strProjectName;
	m_editProjectName.GetWindowText(strProjectName);
	if (strProjectName.IsEmpty())
	{
		MessageBox(g_strLanguage[17], g_strLanguage[0], MB_OK);
		m_editProjectName.SetFocus();
		return false;
	}

	if (m_listX86FileList.GetItemCount() == 0 && m_listX64FileList.GetItemCount() == 0)
	{
		MessageBox(g_strLanguage[22], g_strLanguage[0], MB_OK);
		return false;
	}

	//���ļ��������û�ѡ��
	if (strFile.IsEmpty())
	{
		//�����������û�ѡ��Ҫ�������ļ�λ��
		CFileDialog fileDlg(FALSE, _T("xml"), _T("filecopy.xml"), \
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Xml (*.xml)|*.xml||"), \
			CWnd::FromHandle(m_hWnd), sizeof(OPENFILENAME));	
		if (fileDlg.DoModal() != IDOK)  //�û�ȡ����
		{
			return false;
		}
		else
		{
			strFile = fileDlg.GetPathName();
		}
	}

	//��ȡ����
	CFileListXml fileListXml;
	fileListXml.SetProjectName(strProjectName);

	for (int nItem=0; nItem<m_listX86FileList.GetItemCount(); nItem++)
	{
		FILE_ITEM item;
		item.strName = m_listX86FileList.GetItemText(nItem, 0);
		item.strSrcPath = m_listX86FileList.GetItemText(nItem, 1);
		item.strDestPath = m_listX86FileList.GetItemText(nItem, 2);

		fileListXml.GetX86FileList().push_back(item);
	}

	for (int nItem=0; nItem<m_listX64FileList.GetItemCount(); nItem++)
	{
		FILE_ITEM item;
		item.strName = m_listX64FileList.GetItemText(nItem, 0);
		item.strSrcPath = m_listX64FileList.GetItemText(nItem, 1);
		item.strDestPath = m_listX64FileList.GetItemText(nItem, 2);

		fileListXml.GetX64FileList().push_back(item);
	}

	//���浽�ļ�
	int nResult = fileListXml.Save(strFile);
	if (nResult != 0)
	{
		CString strTemp;
		strTemp.Format(g_strLanguage[23], nResult);
		MessageBox(strTemp, g_strLanguage[0], MB_OK);
		return false;
	}
	else  //�ɹ���������Ϊ�´ε������ļ�
	{
		m_strExportFileListPath = strFile;
		return true;
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonExport()
{
	//���浽�ļ�
	if (SaveFileListToFile(_T("")))  //���ļ������ڲ��ᵯ�����û�ѡ��
	{
		MessageBox(g_strLanguage[15], g_strLanguage[0], MB_OK);  //�����ɹ�
		return;
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonIniToXml()
{
	CFileListConvertDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		V1ToV2_Information info;
		info.strProjectName = dlg.m_strProjectName;
		info.strX86IniFile = dlg.m_strX86INI;
		info.strX64IniFile = dlg.m_strX64INI;
		info.strXmlFile = dlg.m_strXMLSavePath;

		if (CFileListUtil::ConvertIniToXml(info) != 0)
		{
			MessageBox(g_strLanguage[20], g_strLanguage[0], MB_OK);  //INIת��ΪXML��ʽʧ��
		}
		else
		{
			MessageBox(g_strLanguage[21], g_strLanguage[0], MB_OK);	//INIת��ΪXML��ʽ�ɹ�
		}
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX86Edit()
{
	UINT uSelectCount = m_listX86FileList.GetSelectedCount();
	if (0 == uSelectCount)
	{
		MessageBox(g_strLanguage[25], g_strLanguage[0], MB_OK); //��ѡ��һ���༭��
		return;
	}

	int nItem = m_listX86FileList.GetNextItem(-1, LVNI_SELECTED);

	CFileItemDialog dlg;
	dlg.m_strName = m_listX86FileList.GetItemText(nItem, 0);
	dlg.m_strSourcePath = m_listX86FileList.GetItemText(nItem, 1);
	dlg.m_strDestPath = m_listX86FileList.GetItemText(nItem, 2);
	if (dlg.DoModal() == IDOK)
	{
		//ת��Ϊ����ڿ������ߵ����·��
		CString strSrcPath = dlg.m_strSourcePath;
		if (!PathIsRelative(dlg.m_strSourcePath))
		{
			strSrcPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strSourcePath);
		}

		CString strDestPath = dlg.m_strDestPath;
		if (!PathIsRelative(dlg.m_strDestPath))
		{
			strDestPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strDestPath);
		}

		if (strSrcPath.IsEmpty() || strDestPath.IsEmpty())
		{
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //·����Ч
			return;
		}

		//���ø�ѡ����
		m_listX86FileList.SetItemText(nItem, 0, dlg.m_strName);
		m_listX86FileList.SetItemText(nItem, 1, strSrcPath);
		m_listX86FileList.SetItemText(nItem, 2, strDestPath);
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX64Edit()
{
	UINT uSelectCount = m_listX64FileList.GetSelectedCount();
	if (0 == uSelectCount)
	{
		MessageBox(g_strLanguage[25], g_strLanguage[0], MB_OK); //��ѡ��һ���༭��
		return;
	}

	int nItem = m_listX64FileList.GetNextItem(-1, LVNI_SELECTED);

	CFileItemDialog dlg;
	dlg.m_strName = m_listX64FileList.GetItemText(nItem, 0);
	dlg.m_strSourcePath = m_listX64FileList.GetItemText(nItem, 1);
	dlg.m_strDestPath = m_listX64FileList.GetItemText(nItem, 2);
	if (dlg.DoModal() == IDOK)
	{
		//ת��Ϊ����ڿ������ߵ����·��
		CString strSrcPath = dlg.m_strSourcePath;
		if (!PathIsRelative(dlg.m_strSourcePath))
		{
			strSrcPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strSourcePath);
		}

		CString strDestPath = dlg.m_strDestPath;
		if (!PathIsRelative(dlg.m_strDestPath))
		{
			strDestPath = CPathUtilEx::GetRelativePath(CPathUtilEx::Instance()->GetImageFile(), dlg.m_strDestPath);
		}

		if (strSrcPath.IsEmpty() || strDestPath.IsEmpty())
		{
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //·����Ч
			return;
		}

		//���ø�ѡ����
		m_listX64FileList.SetItemText(nItem, 0, dlg.m_strName);
		m_listX64FileList.SetItemText(nItem, 1, strSrcPath);
		m_listX64FileList.SetItemText(nItem, 2, strDestPath);
	}
}
