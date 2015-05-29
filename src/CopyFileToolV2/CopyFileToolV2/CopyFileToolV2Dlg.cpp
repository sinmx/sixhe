// CopyFileToolV2Dlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CCopyFileToolV2Dlg 对话框




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


// CCopyFileToolV2Dlg 消息处理程序

BOOL CCopyFileToolV2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//初始化界面数据
	InitUIValue();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCopyFileToolV2Dlg::InitUIValue()
{
	//设置list控件的头部
	m_listX86FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listX86FileList.InsertColumn(0, g_strLanguage[10], LVCFMT_LEFT, 100);  //名称
	m_listX86FileList.InsertColumn(1, g_strLanguage[11], LVCFMT_LEFT, 200);	//源路径
	m_listX86FileList.InsertColumn(2, g_strLanguage[12], LVCFMT_LEFT, 200);	//目的路径

	m_listX64FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listX64FileList.InsertColumn(0, g_strLanguage[10], LVCFMT_LEFT, 100);  //名称
	m_listX64FileList.InsertColumn(1, g_strLanguage[11], LVCFMT_LEFT, 200);	//源路径
	m_listX64FileList.InsertColumn(2, g_strLanguage[12], LVCFMT_LEFT, 200);	//目的路径
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCopyFileToolV2Dlg::OnPaint()
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
HCURSOR CCopyFileToolV2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCopyFileToolV2Dlg::OnBnClickedButtonSelectFileList()
{
	//弹出界面让用户选择要导入的文件
	CFileDialog fileDlg(TRUE, _T("xml"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Xml Files (*.xml)|*.xml||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //用户取消了
	{
		return;	
	}

	//设置为用户选择的文件
	m_editFileList.SetWindowText(fileDlg.GetPathName());
}

void CCopyFileToolV2Dlg::OnBnClickedButtonCopy()
{
	//校验清单文件路径是否为空
	CString strFileList;
	m_editFileList.GetWindowText(strFileList);
	if (strFileList.IsEmpty())
	{
		MessageBox(g_strLanguage[1], g_strLanguage[0], MB_OK);  //文件清单不能为空
		m_editFileList.SetFocus();
		return;
	}

	//读取拷贝列表
	CFileListXml fileList;
	if (fileList.Load(strFileList) != 0)
	{
		MessageBox(g_strLanguage[2], g_strLanguage[0], MB_OK);  //文件清单格式不对，加载失败
		return;	
	}

	//根据列表逐个拷贝
	COPY_RESULT_INFORMATION result;
	CFileCopyUtil::Copy(fileList, result);
	
	//根据拷贝结果提示用户
	CString strHint;
	CString strTemp;
	strTemp.Format(g_strLanguage[3], fileList.GetProjectName());	//项目名称
	strHint += strTemp+_T("\r\n\r\n");

	if (result.nResult == 0) //success
	{
		strHint += g_strLanguage[4]+_T("\r\n\r\n");			//拷贝成功
	}
	else //fail
	{
		strHint += g_strLanguage[5]+result.failFileItem.strName+_T("\r\n\r\n");	 //拷贝失败，失败文件为：
	}

	if (result.newFileList.size() > 0)  //有新增文件
	{
		strHint += g_strLanguage[6]+_T("\r\n");  //成功新增如下文件
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

		//转化为相对于拷贝工具的相对路径
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
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //路径无效
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
		MessageBox(g_strLanguage[13], g_strLanguage[0], MB_OK); //请选择一个删除项
		return;
	}

	for (UINT i=0; i<uSelectCount; i++)
	{
		int nItem = m_listX86FileList.GetNextItem(-1, LVNI_SELECTED);
		ASSERT(nItem != -1);
		m_listX86FileList.DeleteItem(nItem);
	}

	if (m_listX86FileList.GetItemCount() == 0)  //没有任何插件，就不允许删除
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

		//转化为相对于拷贝工具的相对路径
		//转化为相对于拷贝工具的相对路径
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
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //路径无效
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
		MessageBox(g_strLanguage[13], g_strLanguage[0], MB_OK); //请选择一个删除项
		return;
	}

	for (UINT i=0; i<uSelectCount; i++)
	{
		int nItem = m_listX64FileList.GetNextItem(-1, LVNI_SELECTED);
		ASSERT(nItem != -1);
		m_listX64FileList.DeleteItem(nItem);
	}

	if (m_listX64FileList.GetItemCount() == 0)  //没有任何插件，就不允许删除
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
	//一进来就设置为空
	m_strExportFileListPath = _T("");

	//弹出界面让用户选择要导入的文件
	CFileDialog fileDlg(TRUE, _T("xml"), NULL, \
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, _T("Xml Files (*.xml)|*.xml||"), \
		CWnd::FromHandle(m_hWnd));	
	if (fileDlg.DoModal() != IDOK)  //用户取消了
	{
		return;	
	}

	//根据用户选择的文件加载
	CFileListXml fileList;
	int nResult = fileList.Load(fileDlg.GetPathName());
	if (nResult != 0) //fail
	{
		MessageBox(g_strLanguage[2], g_strLanguage[0], MB_OK);  //文件清单格式不对，加载失败
		return;	
	}

	//更新到界面中
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

	//提示用户导入成功
	m_strExportFileListPath = fileDlg.GetPathName();
	MessageBox(g_strLanguage[14], g_strLanguage[0], MB_OK);
}

void CCopyFileToolV2Dlg::OnBnClickedButton1Save()
{
	//保存到文件
	if (SaveFileListToFile(m_strExportFileListPath))
	{
		MessageBox(g_strLanguage[16], g_strLanguage[0], MB_OK);  //保存成功
		return;
	}
}

bool CCopyFileToolV2Dlg::SaveFileListToFile(CString strFile)
{
	//校验参数
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

	//空文件名就让用户选择
	if (strFile.IsEmpty())
	{
		//弹出界面让用户选择要导出的文件位置
		CFileDialog fileDlg(FALSE, _T("xml"), _T("filecopy.xml"), \
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Xml (*.xml)|*.xml||"), \
			CWnd::FromHandle(m_hWnd), sizeof(OPENFILENAME));	
		if (fileDlg.DoModal() != IDOK)  //用户取消了
		{
			return false;
		}
		else
		{
			strFile = fileDlg.GetPathName();
		}
	}

	//读取数据
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

	//保存到文件
	int nResult = fileListXml.Save(strFile);
	if (nResult != 0)
	{
		CString strTemp;
		strTemp.Format(g_strLanguage[23], nResult);
		MessageBox(strTemp, g_strLanguage[0], MB_OK);
		return false;
	}
	else  //成功，就设置为下次导出的文件
	{
		m_strExportFileListPath = strFile;
		return true;
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonExport()
{
	//保存到文件
	if (SaveFileListToFile(_T("")))  //空文件名，内部会弹框让用户选择
	{
		MessageBox(g_strLanguage[15], g_strLanguage[0], MB_OK);  //导出成功
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
			MessageBox(g_strLanguage[20], g_strLanguage[0], MB_OK);  //INI转化为XML格式失败
		}
		else
		{
			MessageBox(g_strLanguage[21], g_strLanguage[0], MB_OK);	//INI转化为XML格式成功
		}
	}
}

void CCopyFileToolV2Dlg::OnBnClickedButtonX86Edit()
{
	UINT uSelectCount = m_listX86FileList.GetSelectedCount();
	if (0 == uSelectCount)
	{
		MessageBox(g_strLanguage[25], g_strLanguage[0], MB_OK); //请选择一个编辑项
		return;
	}

	int nItem = m_listX86FileList.GetNextItem(-1, LVNI_SELECTED);

	CFileItemDialog dlg;
	dlg.m_strName = m_listX86FileList.GetItemText(nItem, 0);
	dlg.m_strSourcePath = m_listX86FileList.GetItemText(nItem, 1);
	dlg.m_strDestPath = m_listX86FileList.GetItemText(nItem, 2);
	if (dlg.DoModal() == IDOK)
	{
		//转化为相对于拷贝工具的相对路径
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
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //路径无效
			return;
		}

		//重置该选择项
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
		MessageBox(g_strLanguage[25], g_strLanguage[0], MB_OK); //请选择一个编辑项
		return;
	}

	int nItem = m_listX64FileList.GetNextItem(-1, LVNI_SELECTED);

	CFileItemDialog dlg;
	dlg.m_strName = m_listX64FileList.GetItemText(nItem, 0);
	dlg.m_strSourcePath = m_listX64FileList.GetItemText(nItem, 1);
	dlg.m_strDestPath = m_listX64FileList.GetItemText(nItem, 2);
	if (dlg.DoModal() == IDOK)
	{
		//转化为相对于拷贝工具的相对路径
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
			MessageBox(g_strLanguage[24], g_strLanguage[0], MB_OK);  //路径无效
			return;
		}

		//重置该选择项
		m_listX64FileList.SetItemText(nItem, 0, dlg.m_strName);
		m_listX64FileList.SetItemText(nItem, 1, strSrcPath);
		m_listX64FileList.SetItemText(nItem, 2, strDestPath);
	}
}
