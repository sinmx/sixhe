
//
//本文档实现了各种吃码方式，包括平均吃码，单号吃码
//

#pragma once

#include "Order.h"
#include <vector>
using namespace std;

class CEatMethod
{
public:
	//吃码接口
	virtual vector<CCode> Eat() = 0;
};

class CSingleEatMethod : public CEatMethod  //单号吃码
{
public:
	//strEat吃码串，示例：0520 30500 表示：05吃码20 30吃码500。
	CSingleEatMethod(const CString& strEat);

public:
	//吃码
	virtual vector<CCode> Eat();

	//判断吃码串是否合法
	bool IsCorrect();

private:
	bool Parse(const CString& str, vector<CCode>& codeVec);		//从一个小串中解析一个号码吃码

private:
	CString m_strEat;			//吃码串
};

class CAvgEatMethod : public CEatMethod  //平均吃码
{
public:
	CAvgEatMethod(__int64 nAvgEatValue);  //nAvgEatValue平均吃码值

public:
	//吃码
	virtual vector<CCode> Eat();

private:
	__int64 m_nAvgEatValue;  //平均吃码值
};