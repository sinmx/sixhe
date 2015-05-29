//
//CCode 表示一个号码，提供号码相关的服务如是什么生肖，是大还是小，什么颜色的
//COrder 存放一张订单的内容，提供和订单有关的服务
//

#pragma once

class CCode		//号码
{
public:
	CCode();
	~CCode();

public:
	//购买及获取
	void Buy(__int64 nMoney) {m_nMoney = nMoney;}
	__int64 Money() const {return m_nMoney;}

	//设置获取代号值
	void SetCode(int nValue) {m_nValue = nValue;}
	int GetCode() const {return m_nValue;}

	//获取生肖的名称
	CString GetShengXiao() const;

	//判断颜色
	bool IsRed() const;
	bool IsBlue() const;
	bool IsGreen() const;

	//判断是大还是小
	bool IsBig() const;
	bool IsSmall() const;

	//判断是单还是双
	bool IsDouble() const;
	bool IsSingle() const;

	//号码两位相加是单还是双
	bool IsSumSingle() const;
	bool IsSumDouble() const;

	//号码两位相加是大还是小
	bool IsSumBig() const;
	bool IsSumSmall() const;

public:
	//set容器需要这些操作符重载
	bool operator== (const CCode& right) const {return m_nValue==right.m_nValue;}
	bool operator<(const CCode& right) const {return m_nValue < right.m_nValue;}

private:
	int m_nValue;		//号码值
	__int64 m_nMoney;		//购买的钱数
};

class COrder
{
public:
	COrder(void);
	~COrder(void);

	enum
	{
		CODE_NUMBER = 49,		//号码个数
	};

public:
	//获取所有购买金额
	__int64 GetTotalBuyMoney() const;

	//购买指定的号码
	void Buy(int nIndex, __int64 nMoney);

	//获取指定号码购买金额
	__int64 Money(int nIndex) const;

	//设置获取用户名
	void Name(const CString& strName) {m_strCustomerName = strName;}
	CString Name() const {return m_strCustomerName;}

private:
	CString m_strCustomerName;		//用户名
	CCode m_code[CODE_NUMBER+1];		//49个号码，其中第0个不用
};