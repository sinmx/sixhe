
//
//CRewardManager	兑奖管理器，负责提供兑奖相关的服务
//

#pragma once

typedef struct _tagRewardItem
{
	CString strCustomer;		//用户
	__int64 nProfit;				//利润
	__int64 nBuy;					//购买金额
	int nDiscount;				//折扣
	__int64 nWin;					//中奖金额
	int nBettingOdds;			//赔率
	bool bAlreadyReward;		//标志是否已经对过奖

	_tagRewardItem()
	{
		nProfit = 0;
		nBuy = 0;
		nDiscount = 0;
		nWin = 0;
		nBettingOdds = 0;
		bAlreadyReward = false;
	}

}REWARD_ITEM, *PREWARD_ITEM;

class CRewardManager
{
protected:  //阻止被实例化
	CRewardManager();
	~CRewardManager();

public:
	//获取单一实例
	static CRewardManager* Instance();

	//初始化
	void Init();

	//获取兑奖详细信息
	vector<REWARD_ITEM> GetRewardInfo();

	//保存已经兑奖标志
	bool SaveAlreadyReward(const CString& strCustomer, bool bAlready);

	//计算自己利润
	__int64 CalcSelfProfit();		

	//打印兑奖表
	void PrintRewardTable();

private:
	//查询回调函数，context用于回调上下文，recordVec记录内容，返回值true表示继续，false表示停止
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

	bool IsAlreadyReward(const CString& strCustomer);  //判断是否已经兑过奖
	CString PaddingWhiteSpace(const CString& strSource, int nMaxChar);  //不足nMaxChar的字符，填充空格，一个非ASCII码字符被当作2个字符

private:
	static CCS m_synCCS;					//用于同步外部调用
	vector<REWARD_ITEM> m_alreadyRewardVec;		//兑奖信息，目前只有bAlreadyReward有用
};