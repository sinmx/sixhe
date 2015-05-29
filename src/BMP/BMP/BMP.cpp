// BMP.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "BMP.h"
#include <DbgHelp.h>
#include "MainDlg.h"
#include "CustomerManager.h"
#include "OrderManager.h"
#include "OperatorManager.h"
#include "ConfManager.h"
#include "RewardManager.h"
#include "SoundManager.h"

// CBMPApp

BEGIN_MESSAGE_MAP(CBMPApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBMPApp 构造

CBMPApp::CBMPApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CBMPApp 对象

CBMPApp theApp;
CFileLog theLog;

LONG WINAPI CBMPApp::MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{	
	typedef BOOL (WINAPI * PMiniDumpWriteDump)(
		__in HANDLE hProcess,
		__in DWORD ProcessId,
		__in HANDLE hFile,
		__in MINIDUMP_TYPE DumpType,
		__in_opt PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
		__in_opt PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
		__in_opt PMINIDUMP_CALLBACK_INFORMATION CallbackParam
		);
	PMiniDumpWriteDump MiniDumpWriteDump=NULL; 

	HMODULE hModule=LoadLibrary(_T("dbghelp.dll"));
	if( hModule!=NULL)
	{
		MiniDumpWriteDump=(PMiniDumpWriteDump)GetProcAddress(hModule,"MiniDumpWriteDump");
		if( MiniDumpWriteDump==NULL)
		{
			FreeLibrary(hModule);
			hModule=NULL;

			return EXCEPTION_CONTINUE_SEARCH;
		}
	}

	CTime tm=CTime::GetCurrentTime();	
	CString strTm=tm.Format("%d-%H-%M-%S.dmp");	
	CString strFileName=CPathUtilEx::Instance()->GetImagePath()+strTm;		
	HANDLE lhDumpFile=CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);	
	if( INVALID_HANDLE_VALUE==lhDumpFile)
	{		
		FreeLibrary(hModule);
		hModule=NULL;

		return EXCEPTION_CONTINUE_SEARCH;
	}

	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
	loExceptionInfo.ExceptionPointers = ExceptionInfo;
	loExceptionInfo.ThreadId = GetCurrentThreadId();
	loExceptionInfo.ClientPointers = TRUE;
	if( !MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile,\
		(MINIDUMP_TYPE)(MINIDUMP_TYPE)(MiniDumpNormal/*|MiniDumpWithDataSegs|MiniDumpWithFullMemory|MiniDumpWithHandleData*/),&loExceptionInfo, NULL, NULL))
	{
		//
	}

	CloseHandle(lhDumpFile);

	FreeLibrary(hModule);
	hModule=NULL;
	return EXCEPTION_CONTINUE_SEARCH;
}

void CBMPApp::InitLogModule()
{
	FILELOGPROPERTY fileLogProperty;
	fileLogProperty.strFileName=CPathUtilEx::Instance()->GetImagePath()+_T("logs\\")+_T("bmp.log");
	fileLogProperty.bBaseDay=FALSE;
	fileLogProperty.dwFileMaxKB=10240;  //10M
	if (!theLog.SetFileLogProperty(fileLogProperty))
	{
		return;
	}

	//默认设置为错误级别
	if (!theLog.SetLogLevel(LOGLEVELNAME_ERROR))
	{
		return;
	}
}

BOOL CBMPApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//开启内存泄露检测
#ifdef _DEBUG 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//设置默认异常过滤器
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	//加载语言文件，目前只支持中文
	if (CLanguageManager::Instance()->LoadLanguage(CLanguageManager::LANGUAGE_CHS_SIMPLE) != CLanguageManager::Error_Language_Manager_Success)
	{
		::MessageBox(NULL, _T("语言文件加载失败。"), _T("提示"), MB_OK);
		return FALSE;
	}	

	//初始化日志模块
	InitLogModule();

	//加载配置
	CConfManager::Instance()->Init();
	CConfManager::Instance()->DBSchemeVersion(1);  //目前版本为1

	//初始化用户模块
	CCustomerManager::Instance()->Init();

	//初始化单子管理模块
	COrderManager::Instance()->Init();

	//初始化操作日志模块
	COperatorManager::Instance()->Init();

	//初始化兑奖模块
	CRewardManager::Instance()->Init();

	//初始化界面资源
	DuiLib::CPaintManagerUI::SetInstance(m_hInstance);
	DuiLib::CPaintManagerUI::SetResourcePath(CPathUtilEx::Instance()->GetImagePath());
	DuiLib::CPaintManagerUI::SetResourceZip(_T("skin.zip"));	

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr))
	{
		return FALSE;
	}

	//创建主界面
	CMainDlg* pFrame = new CMainDlg();
	if (pFrame == NULL)
	{
		return FALSE;
	}

	pFrame->SetAutoDelete(false);	
	if (pFrame->Create(_T("mainPage.xml"), NULL, WS_VISIBLE|WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW) == NULL)  //fail
	{
		//
	}
	else
	{
		pFrame->ShowModal();
	}

	delete pFrame;
	pFrame = NULL;

	::CoUninitialize();

	//停止音频播放模块，避免程序退出了，还在播放，异常
	CSoundManager::Instance()->Exit();

	return FALSE;
}
