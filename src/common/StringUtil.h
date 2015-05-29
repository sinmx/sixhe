//
//CStringUtil		�ַ������ߣ���չMFC CString ûʵ�ֵĹ��ܣ����ַ����ָ�
//

#pragma once

#include <vector>
using namespace std;

class CStringUtil
{
public:
	CStringUtil(LPCTSTR szInput);

public:
	//�ָ��ַ�����chSeperate�ָ��
	vector<CString> Split(TCHAR chSeperate) const;

	//�ж��Ƿ�Ϊ����
	bool IsAllNumber() const;

	//�ж��Ƿ�Ϊһ����
	bool IsNumber() const;

	//��ȡ���봮��ַ
	operator LPCTSTR () const {return (LPCTSTR)m_strInput;}
	
	//����ֱ��ת��Ϊ�ַ���
	static CString IntToStr(int num);
	static CString Int64ToStr(__int64 num);

	//�����ַ�ת��Ϊһ��������������������ַ�������-1
	static int CharToInt(TCHAR ch);

private:
	CString m_strInput;		//������ַ���
};