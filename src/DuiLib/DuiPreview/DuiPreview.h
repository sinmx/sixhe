// DuiPreview.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDuiPreviewApp:
// �йش����ʵ�֣������ DuiPreview.cpp
//

class CDuiPreviewApp : public CWinApp
{
public:
	CDuiPreviewApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDuiPreviewApp theApp;