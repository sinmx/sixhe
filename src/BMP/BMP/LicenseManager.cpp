#include "StdAfx.h"
#include "LicenseManager.h"

CCS CLicenseManager::m_csSyn;
CLicenseManager::CLicenseManager(void)
{
}

CLicenseManager::~CLicenseManager(void)
{
}

CLicenseManager* CLicenseManager::Instance()
{
	CSynObject synObj(&m_csSyn);

	static CLicenseManager thisObj;
	return &thisObj;
}

void CLicenseManager::Init()
{
	CSynObject synObj(&m_csSyn);

	//do nothing now
}

int CLicenseManager::GetUserMaxCount() const
{
	CSynObject synObj(&m_csSyn);

	return 10;
}