// BMP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CBMPApp:
// �йش����ʵ�֣������ BMP.cpp
//

class CBMPApp : public CWinApp
{
public:
	CBMPApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

private:
	static LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);  //�쳣������
	void InitLogModule();	//��ʼ����־ģ��

	DECLARE_MESSAGE_MAP()
};

extern CBMPApp theApp;