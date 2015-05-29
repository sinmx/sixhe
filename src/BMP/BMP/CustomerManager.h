
//
//CCustomerManager 下庄用户管理模块，负责下庄用户信息读取保存，提供和下庄用户相关服务
//

#pragma once

#include "..\..\common\SynObject.h"
#include "..\..\common\DBUtil.h"
#include <vector>
using namespace std;

typedef struct _tagCustomer  //下庄用户
{
	CString strName;		//用户名
	int nDiscount;			//回水千分点
	int nBettingOdds;		//赔率

	_tagCustomer()
	{
		nDiscount = 100;
		nBettingOdds = 41;
	}

	bool IsValid() const
	{
		if (strName.IsEmpty() || nDiscount<0 || nDiscount>1000 || nBettingOdds<0)
		{
			return false;
		}

		return true;
	}
} CUSTOMER, *PCUSTOMER;

class CCustomerManager
{
protected:		//阻止被实例化
	CCustomerManager(void);
	~CCustomerManager(void);

public:
	//获取单一实例
	static CCustomerManager* Instance();

	//初始化该模块
	void Init();

	//增加一个用户，如果bNeedAutoGenerate为true表示内部自动生成一个用户并保存到数据库中，customer存放该用户信息，返回给调用者
	//如果bNeedAutoGenerate为false，将customer指定的用户信息保存到数据库中
	bool AddCustomer(CUSTOMER& customer, bool bNeedAutoGenerate);

	//删除一个下庄用户
	bool DeleteCustomer(const CString& strName);

	//更新一个用户，strOldName更新前用户的名称，customer更新后用户的信息
	//如果更新的字段是用户名，那么原来的用户会先被删除，然后增加新用户
	bool UpdateCustomer(const CString& strOldName, const CUSTOMER& customer);

	//获取用户列表
	vector<CUSTOMER> GetCustomerList();

	//询问当前有多少用户
	int GetCustomerCount() const;

	//询问是否有用户
	bool IsHasCustomer();

	//加入监听列表，新增或删除下注用户将通知监听列表中的对象
	void Attach(ICommunication* pObj);
	void Detach(ICommunication* pObj); 

private:
	//查询回调函数，context用于回调上下文，recordVec记录内容，返回值true表示继续，false表示停止
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);
	CString GenerateOneCustomerName();		//自动生成一个用户名
	bool IsValidName(const CString& strName);		//判断是否为有效用户名，用户名是否冲突
	bool InsertCustomer(const CUSTOMER& customer);		//插入用户

private:
	static CCS m_csSyn;				//用于同步外部调用
	vector<CUSTOMER> m_customerVec;		//下庄用户列表
	vector<ICommunication*> m_listenerVec;	//监听对象列表
};