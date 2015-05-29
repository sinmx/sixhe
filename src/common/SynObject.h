#pragma once

class CCS
{
public:
	CCS()
	{
		InitializeCriticalSection(&m_criticalSection);
	}
	~CCS()
	{
		DeleteCriticalSection(&m_criticalSection);
	}
public:
	CRITICAL_SECTION m_criticalSection;
};

class CSynObject
{
public:
	CSynObject(CCS* pCS)
	{
		m_pCS=pCS;
		EnterCriticalSection(&m_pCS->m_criticalSection);
	}
	~CSynObject()
	{
		LeaveCriticalSection(&m_pCS->m_criticalSection);
	}
private:
	CCS* m_pCS;
};