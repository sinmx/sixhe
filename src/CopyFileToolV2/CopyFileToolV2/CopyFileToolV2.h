// CopyFileToolV2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCopyFileToolV2App:
// �йش����ʵ�֣������ CopyFileToolV2.cpp
//

class CCopyFileToolV2App : public CWinApp
{
public:
	CCopyFileToolV2App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCopyFileToolV2App theApp;