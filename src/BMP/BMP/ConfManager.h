
//
//CConfManager 配置管理器，提供配置相关的服务
//

#pragma once

class CConfManager
{
protected:  //阻止被实例化
	CConfManager(void);
	~CConfManager(void);

public:
	//获取单一实例对象
	static CConfManager* Instance();

	//初始化加载配置
	void Init();

	//获取保存当前数据库模式版本号
	bool DBSchemeVersion(int nVersion);
	int DBSchemeVersion() const;

	//判断五行配置是否为默认值
	bool IsWuHangDefaultValue() const;

	//获取保存五行分组号码
	CString GoldCode() const;
	bool GoldCode(const CString& strCode);
	CString WoodCode() const;
	bool WoodCode(const CString& strCode);
	CString WaterCode() const;
	bool WaterCode(const CString& strCode);
	CString FireCode() const;
	bool FireCode(const CString& strCode);
	CString EarthCode() const;
	bool EarthCode(const CString& strCode);	

	//获取保存是否进行声音校对
	bool IsSoundCheck() const;
	bool SoundCheck(bool bEnable);

	//获取保存平均回水
	int AvgDiscount() const;
	bool AvgDiscount(int value);

	//获取保存平均赔率
	int AvgBettingOdds() const;
	bool AvgBettingOdds(int nValue);

	//获取保存中奖号码
	int WinCode() const;
	bool WinCode(int nCodeValue);

	//向上庄购买时回水
	int SelfBuyDiscount() const;
	bool SelfBuyDiscount(int nValue);

	//向上庄购买时的赔率
	int SelfBuyBettingOdds() const;
	bool SelfBuyBettingOdds(int nValue);

	//提示用户配置五行
	bool IsNeedHintConfigureWuHang() const;
	bool NeedHintConfigureWuHang(bool bNeed);

private:
	//查询回调函数，context用于回调上下文，recordVec记录内容，返回值true表示继续，false表示停止
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

	bool Save(const CString& strName, const CString& strValue);   //保存到数据库中

private:
	static CCS m_synCCS;			//用于同步外部调用
	map<CString, CString> m_confMap;		//配置数据
};