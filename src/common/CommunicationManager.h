///////////////////////////////////////////
//类名：CCommunicationManager	 通信管理器
//职责：单一实例，中介者模式，负责对象间的通信方式，简化对象间的依赖
////////////////////////////////////////////////////////

#pragma once

#include "SynObject.h"
#include <vector>
using namespace std;

class ICommunication  //可通信对象接口
{
public:
	//获取通信唯一ID值
	virtual int GetID() = 0;

	//接收事件，wParam lParam 返回值依赖于具体的nEventCode
	virtual int AcceptEvent(int nEventCode, WPARAM wParam=0, LPARAM lParam=0) = 0;
};

class CCommunicationManager
{
protected: //防止被实例化
	CCommunicationManager();
	~CCommunicationManager();

public:
	//获取单一实例对象
	static CCommunicationManager* Instance();

	//加入删除对象到通信列表中
	void Attach(ICommunication* pObj);
	void Detach(ICommunication* pObj);

	//通信接口，nID接收方的唯一ID，其他参数和返回值同接口AcceptEvent
	int Notify(int nID, int nEventCode, WPARAM wParam=0, LPARAM lParam=0);

protected:
	vector<ICommunication*> m_comObjVector;		//所有的通信对象
	static CCS m_csSyn;				//用于同步外部调用
};