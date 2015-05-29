
//
//本文档实现了各种购买下注方式，包括单号下注，区间段下注，分组下注，等额下注
//

#pragma once

#include "Order.h"
#include "CodeGroup.h"
#include <vector>
using namespace std;

class CBuyMethod
{
public:
	//购买下注接口
	virtual vector<CCode> Buy() = 0;

	//下注日志接口
	virtual CString Log() = 0;

	//判断用户购买输入是否正确
	virtual bool IsCorrect() = 0;
};

class CSingleBuyMethod : public CBuyMethod  //单号购买
{
public:
	//strBuy购买串，示例：2030 05-123 表示号码20加注30，号码05减注123
	CSingleBuyMethod(const CString& strBuy);

public:
	//购买下注
	virtual vector<CCode> Buy();

	//判断用户购买输入是否正确
	virtual bool IsCorrect();

	//下注日志
	virtual CString Log();

	//设置是否为加注，不是加注就是下注
	void SetAddFlag(bool flag) {m_bAdd = flag;}

private:
	bool Parse(const CString& str, vector<CCode>& codeVec);		//从一个小串中解析一个号码购买

private:
	CString m_strBuy;			//购买串
	bool m_bAdd;				//标识是否为加注
};

class CEqualBuyMethod : public CBuyMethod  //等额购买
{
public:
	//strCode 号码串，号码之间使用空格分开
	CEqualBuyMethod(const CString& strCode, __int64 nBuyValue);

public:
	//购买下注
	virtual vector<CCode> Buy();

	//判断用户购买输入是否正确
	virtual bool IsCorrect();

	//下注日志
	virtual CString Log();

private:
	CString m_strCode;			//号码串
	__int64 m_nBuyValue;		//购买值
};


class CIntervalBuyMethod : public CBuyMethod  //特段购买
{
public:	
	CIntervalBuyMethod(int nStartCode, int nEndCode, __int64 nBuyValue);

public:
	//购买下注
	virtual vector<CCode> Buy();

	//判断用户购买输入是否正确
	virtual bool IsCorrect();

	//下注日志
	virtual CString Log();

private:
	int m_nStartCode;
	int m_nEndCode;
	__int64 m_nBuyValue;
};

class CGroupBuyMethod : public CBuyMethod  //分组购买
{
public:
	//groupVec可以同时购买多个分组，取号码并集
	CGroupBuyMethod(const set<CCodeGroup> groupSet, __int64 nBuyMoney);

public:
	//购买下注
	virtual vector<CCode> Buy();

	//判断用户购买输入是否正确
	virtual bool IsCorrect();

	//下注日志
	virtual CString Log();

private:
	set<CCodeGroup> m_groupSet;		//购买分组
	__int64 m_nBuyMoney;		//购买金额
};