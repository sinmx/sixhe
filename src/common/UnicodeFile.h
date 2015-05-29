////////////////////////////////////////////////////////
//该类提供在UNICODE编程环境下读写文件功能
///////////////////////////////////////////////////////

#pragma once
#include "afx.h"

#ifndef UNICODE
	#error _T("CUnicodeFile class is used only in UNICODE environment");
#endif

class CUnicodeFile :
	public CFile
{
public:
	CUnicodeFile(void);
	~CUnicodeFile(void);

public:
	//功能: 打开文件
	//参数: 同CFile的Open函数
	BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags);

	//功能: 关闭文件
	void Close();

	//功能: 读一行文本
	//参数: rString 读出的文本内容
	BOOL ReadString(CString& rString);

	//功能: 将文本内容写入文件
	//参数: lpsz 写入的文本内容
	void WriteString(LPCTSTR lpsz);

private:
	//针对ANSI文件读取一行文本
	BOOL ReadStringForANSI(CString& rString);

	//针对UNICODE文件读取一行文本
	BOOL ReadStringForUNICODE(CString& rString);

private:
	//标识是否已经打开文件
	BOOL m_bHasOpen;

	//标识打开的文件是否为UNICODE
	BOOL m_bUnicodeFile;
};
