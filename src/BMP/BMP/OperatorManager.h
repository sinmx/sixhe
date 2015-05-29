//
//COperatorManager	操作日志模块，提供操作日志相关服务
//

#pragma once

class COperatorManager
{
protected:
	COperatorManager(void);
	~COperatorManager(void);

public:
	//获取单一实例
	static COperatorManager* Instance();

	//初始化模块
	void Init();

	//购买日志
	CString GetBuyLog();
	bool ClearBuyLog();
	bool AppendBuyLog(const CString& strNewLog);

private:
	//查询回调函数，context用于回调上下文，recordVec记录内容，返回值true表示继续，false表示停止
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);
	bool InsertBuyLog(const CString& strLog);		//插入下注日志到数据库中，如果已有数据，替换之
	CString AppendTime(const CString& strLog);		//日志内容前面追加时间

private:
	static CCS m_synCCs;			//用于同步外部调用
	CString m_strBuyLog;	//下注购买日志
};