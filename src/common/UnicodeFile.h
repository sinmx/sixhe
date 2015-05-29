////////////////////////////////////////////////////////
//�����ṩ��UNICODE��̻����¶�д�ļ�����
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
	//����: ���ļ�
	//����: ͬCFile��Open����
	BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags);

	//����: �ر��ļ�
	void Close();

	//����: ��һ���ı�
	//����: rString �������ı�����
	BOOL ReadString(CString& rString);

	//����: ���ı�����д���ļ�
	//����: lpsz д����ı�����
	void WriteString(LPCTSTR lpsz);

private:
	//���ANSI�ļ���ȡһ���ı�
	BOOL ReadStringForANSI(CString& rString);

	//���UNICODE�ļ���ȡһ���ı�
	BOOL ReadStringForUNICODE(CString& rString);

private:
	//��ʶ�Ƿ��Ѿ����ļ�
	BOOL m_bHasOpen;

	//��ʶ�򿪵��ļ��Ƿ�ΪUNICODE
	BOOL m_bUnicodeFile;
};
