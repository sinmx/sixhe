//
//CStringUtil		字符串工具，扩展MFC CString 没实现的功能，如字符串分割
//

#pragma once

#include <vector>
using namespace std;

class CStringUtil
{
public:
	CStringUtil(LPCTSTR szInput);

public:
	//分割字符串，chSeperate分割符
	vector<CString> Split(TCHAR chSeperate) const;

	//判断是否都为数字
	bool IsAllNumber() const;

	//判断是否为一个数
	bool IsNumber() const;

	//获取输入串地址
	operator LPCTSTR () const {return (LPCTSTR)m_strInput;}
	
	//整数直接转化为字符串
	static CString IntToStr(int num);
	static CString Int64ToStr(__int64 num);

	//数字字符转化为一个整数，如果不是数字字符，返回-1
	static int CharToInt(TCHAR ch);

private:
	CString m_strInput;		//输入的字符串
};