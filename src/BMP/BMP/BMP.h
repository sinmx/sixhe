// BMP.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CBMPApp:
// 有关此类的实现，请参阅 BMP.cpp
//

class CBMPApp : public CWinApp
{
public:
	CBMPApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

private:
	static LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);  //异常处理函数
	void InitLogModule();	//初始化日志模块

	DECLARE_MESSAGE_MAP()
};

extern CBMPApp theApp;