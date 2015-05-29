#include "StdAfx.h"
#include "SoundManager.h"
#include <process.h>
#include <Mmsystem.h>

#pragma comment(lib, "Winmm.lib")

CCS CSoundManager::m_csSyn;
CSoundManager::CSoundManager(void) :
m_bAlreadyInit(false),
m_hPlayThreadHandle(NULL),
m_hHasNewSoundEvent(NULL),
m_hQuitEvent(NULL)
{
}

CSoundManager::~CSoundManager(void)
{
}

CSoundManager* CSoundManager::Instance()
{
	CSynObject synObj(&m_csSyn);

	static CSoundManager thisObj;
	return &thisObj;
}

void CSoundManager::Play(DWORD_PTR id)
{
	CSynObject synObj(&m_csSyn);

	m_soundQueue.push(id);

	if (m_bAlreadyInit)
	{
		::SetEvent(m_hHasNewSoundEvent);
	}
	else
	{
		//创建事件
		m_hHasNewSoundEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
		ASSERT(m_hHasNewSoundEvent);
		if (m_hHasNewSoundEvent == NULL)
		{
			theLog.LogError(_T("Failed to create the event(HasNewSoundEvent), error=%d."), GetLastError());
			return;
		}

		m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_hQuitEvent == NULL)
		{
			theLog.LogError(_T("Failed to create the event(QuitEvent), error=%d."), GetLastError());
			return;
		}

		//启动打包线程
		m_hPlayThreadHandle = (HANDLE)_beginthreadex(NULL, 0, PlayProc, NULL, 0, NULL);
		if (m_hPlayThreadHandle == 0)
		{
			theLog.LogError(_T("Failed to create play sound thread."));
			return;
		}

		m_bAlreadyInit = true;
	}	
}

DWORD_PTR CSoundManager::GetOneSound()
{
	CSynObject synObj(&m_csSyn);

	if (m_soundQueue.empty())
	{
		return 0;
	}
	else
	{
		DWORD_PTR id = m_soundQueue.front();
		m_soundQueue.pop();
		return id;
	}	
}

void CSoundManager::Exit()
{
	//不能同步，否则会发生死锁 CSynObject synObj(&m_csSyn);
	::SetEvent(m_hQuitEvent);
	WaitForSingleObject(m_hPlayThreadHandle, 3000);
}

UINT WINAPI CSoundManager::PlayProc(LPVOID lpParameter)
{
	while (true)
	{
		DWORD_PTR id = CSoundManager::Instance()->GetOneSound();
		if (id == 0)  //没有音频文件可播放
		{
			HANDLE hEvent[2] = {CSoundManager::Instance()->m_hQuitEvent, CSoundManager::Instance()->m_hHasNewSoundEvent};
			DWORD dwResult = WaitForMultipleObjects(2, hEvent, FALSE, INFINITE);
			if (dwResult == WAIT_OBJECT_0) //退出
			{
				return 1;
			}
			else  //有音频文件
			{
				continue;
			}
		}
		else
		{
			//检测是否退出
			if (WaitForSingleObject(CSoundManager::Instance()->m_hQuitEvent, 0) == WAIT_OBJECT_0)
			{
				return 1;
			}
			else
			{
				PlaySound(MAKEINTRESOURCE(id), 	GetModuleHandle(NULL),  SND_RESOURCE|SND_NODEFAULT|SND_SYNC);
			}			
		}
	}	

	return 0;
}