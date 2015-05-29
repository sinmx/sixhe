
//
//CLicenseManager 授权管理模块，提供授权相关的服务
//

#pragma once

class CLicenseManager
{
protected:		//阻止被实例化
	CLicenseManager(void);
	~CLicenseManager(void);

public:
	//获取单一实例
	static CLicenseManager* Instance();

	//初始化该模块
	void Init();

	//目前试用版本，简单在用户数上进行控制
	int GetUserMaxCount() const;

private:
	static CCS m_csSyn;				//用于同步外部调用	
};