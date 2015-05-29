
//
//COrderManager 订单管理模块，提供和订单相关的服务
//

#pragma once

#include "Order.h"
#include <vector>
using namespace std;

struct ProfitItem  //利润项
{
	int nCode;	//号码	
	__int64 nProfit;	//利润

	ProfitItem()
	{
		nCode = 0;		
		nProfit = 0;
	}
};

class COrderManager
{
protected:		//阻止被实例化
	COrderManager(void);
	~COrderManager(void);

public:
	//获取单一的实例
	static COrderManager* Instance();

	//初始化订单模块
	void Init();

	//加入监听列表，新增或删除下注用户将通知监听列表中的对象
	void Attach(ICommunication* pObj);
	void Detach(ICommunication* pObj);

	//删除指定客户的订单，如果strUserName为空，删除所有订单
	bool DeleteOrder(const CString& strUserName);

	//获取指定客户的订单
	bool GetOrder(const CString& strUserName, COrder& order) const;

	//更新指定客户的订单，如果原来没有该客户的订单，新增一份订单
	bool UpdateOrder(const CString& strUserName, const COrder& order);
	bool UpdateOrder(const CString& strUserName, const vector<CCode>& buyVec);

	//加注或减注单子中的一些号码
	bool OrderAppend(const CString& strUserName, const vector<CCode>& buyVec);

	//判断是否有单子
	bool IsHasOrder();

	//判断是否全部吃了
	bool IsAllEat();

	//更新吃码表
	bool UpdateEatTable(const vector<CCode>& eatVec);	

	//获取号码吃码值
	__int64 GetEatValue(int nCode) const;
	__int64 GetEatValue() const;

	//重置吃码表
	bool RestEatTable();

	//统计号码购买数
	__int64 GetTotalBuy(int nCode) const;  //统计指定号码总的购买数
	__int64 GetTotalBuy(const CString& strCustomer) const;  //统计指定用户总的购买数
	__int64 GetTotalBuy() const;  //统计所有用户所有号码购买总额

	//获取指定号码的上报值，也就是该号码客户购买总额-自己吃码总额
	__int64 GetSelfBuy(int nCode) const;

	//获取指定用户指定号码购买值
	__int64 GetBuyValue(const CString& strCustomer, int nCode) const;

	//计算绝对利润，也就是找到一个平均吃码值，使得不管开哪个号码，都可以赚那么多钱
	//nAbsProfit 绝对利润值，nAvgEat 应该吃码多少
	void CalcAbsoluteProfit(__int64& nAbsProfit, __int64& nAvgEat);

	//计算最小利润为指定值时，应该平均吃码多少
	void CalcMinProfit(__int64 nMinProfit, __int64& nAvgEat);

	//计算指定平均吃码值时，最大和最小利润
	void CalcAvgEatProfit(__int64 nAvgEat, __int64& nMinProfit, __int64& nMaxProfit);

	//计算指定号码的利润
	__int64 CalcCodeProfit(int nCode); 

	//获取最小最大利润及对应的号码
	void GetMinMaxProfit(__int64& nMinProfit, int& nMinCode, __int64& nMaxProfit, int& nMaxCode);	

	//打印自己购买表
	void PrintSelfBuyTable();

private:
	//查询回调函数，context用于回调上下文，recordVec记录内容，返回值true表示继续，false表示停止
	static bool __stdcall  EnumOrderRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);
	static bool __stdcall  EnumEatRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

	void InitOrder();		//初始化单子
	void InitEatTable();	//初始化吃码表
	bool InsertOrder(const COrder& order);		//插入一份订单，如果已经存在替换它

	bool InsertEatTable(CCode eatCode[COrder::CODE_NUMBER+1]);		//插入吃码表，如果已经存在替换它
	bool DeleteEatTable();		//删除吃码表数据

	void AvgEat(__int64 nEat, int& nMinCode, __int64& nMinProfit, int& nMaxCode, __int64& nMaxProfit);  //计算平均吃码后，最小最大利润及对应开奖的号码
	void CalcAbsoluteProfit(__int64 nMinEat, __int64 nMaxEat, __int64& nAbsProfit, __int64& nAvgEat);	//在[min,max]吃码间计算绝对利润
	void CalcMinProfit(__int64 nMinEat, __int64 nMaxEat, __int64 nMinProfit, __int64& nAvgEat);  //在[min, max]之间计算绝对利润>=nMinProfit时，该吃多少
	
private:
	static CCS m_synCCS;		//用于同步外部调用
	vector<ICommunication*> m_listenerVec;	//监听对象列表
	vector<COrder> m_orderVec;	//订单列表
	CCode m_eatCode[COrder::CODE_NUMBER+1];		//49个号码，其中第0个不用，每个号码存放的钱数为吃码数
};