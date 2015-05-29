#include "stdafx.h"
#include "CommunicationManager.h"

CCS CCommunicationManager::m_csSyn;

CCommunicationManager::CCommunicationManager()
{

}

CCommunicationManager::~CCommunicationManager()
{
	//
}

CCommunicationManager* CCommunicationManager::Instance()
{
	//同步外部调用
	CSynObject synObj(&m_csSyn);

	static CCommunicationManager thisObj;
	return &thisObj;
}

void CCommunicationManager::Attach(ICommunication* pObj)
{
	//同步外部调用
	CSynObject synObj(&m_csSyn);

	for (vector<ICommunication*>::const_iterator it=m_comObjVector.begin(); it!=m_comObjVector.end(); it++)
	{
		if ((*it) == pObj)
		{
			return;
		}
	}

	//到此说明是新对象
	m_comObjVector.push_back(pObj);
}

void CCommunicationManager::Detach(ICommunication* pObj)
{
	//同步外部调用
	CSynObject synObj(&m_csSyn);

	for (vector<ICommunication*>::iterator it=m_comObjVector.begin(); it!=m_comObjVector.end(); it++)
	{
		if ((*it) == pObj)
		{
			m_comObjVector.erase(it);
		}
	}
}

int CCommunicationManager::Notify(int nID, int nEventCode, WPARAM wParam, LPARAM lParam)
{
	//同步外部调用
	CSynObject synObj(&m_csSyn);

	for (vector<ICommunication*>::const_iterator it=m_comObjVector.begin(); it!=m_comObjVector.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it)->AcceptEvent(nEventCode, wParam, lParam);
		}
	}

	return 0;
}