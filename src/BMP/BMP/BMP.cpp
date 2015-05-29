// BMP.cpp : ����Ӧ�ó��������Ϊ��
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


// CBMPApp ����

CBMPApp::CBMPApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CBMPApp ����

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

	//Ĭ������Ϊ���󼶱�
	if (!theLog.SetLogLevel(LOGLEVELNAME_ERROR))
	{
		return;
	}
}

BOOL CBMPApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//�����ڴ�й¶���
#ifdef _DEBUG 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//����Ĭ���쳣������
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	//���������ļ���Ŀǰֻ֧������
	if (CLanguageManager::Instance()->LoadLanguage(CLanguageManager::LANGUAGE_CHS_SIMPLE) != CLanguageManager::Error_Language_Manager_Success)
	{
		::MessageBox(NULL, _T("�����ļ�����ʧ�ܡ�"), _T("��ʾ"), MB_OK);
		return FALSE;
	}	

	//��ʼ����־ģ��
	InitLogModule();

	//��������
	CConfManager::Instance()->Init();
	CConfManager::Instance()->DBSchemeVersion(1);  //Ŀǰ�汾Ϊ1

	//��ʼ���û�ģ��
	CCustomerManager::Instance()->Init();

	//��ʼ�����ӹ���ģ��
	COrderManager::Instance()->Init();

	//��ʼ��������־ģ��
	COperatorManager::Instance()->Init();

	//��ʼ���ҽ�ģ��
	CRewardManager::Instance()->Init();

	//��ʼ��������Դ
	DuiLib::CPaintManagerUI::SetInstance(m_hInstance);
	DuiLib::CPaintManagerUI::SetResourcePath(CPathUtilEx::Instance()->GetImagePath());
	DuiLib::CPaintManagerUI::SetResourceZip(_T("skin.zip"));	

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr))
	{
		return FALSE;
	}

	//����������
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

	//ֹͣ��Ƶ����ģ�飬��������˳��ˣ����ڲ��ţ��쳣
	CSoundManager::Instance()->Exit();

	return FALSE;
}
