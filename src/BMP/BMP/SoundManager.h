
//
//CSoundManager 音频播放管理模块，提供播放音频的服务
//

#pragma once

class CSoundManager
{
protected:		//阻止被实例化
	CSoundManager(void);
	~CSoundManager(void);

public:
	//获取单一实例
	static CSoundManager* Instance();

	//播放音频文件，id为资源ID
	void Play(DWORD_PTR id);

	//获取一个音频文件播放，返回值0表示没有音频文件
	DWORD_PTR GetOneSound();

	//通知模块退出
	void Exit();

private:
	static UINT WINAPI PlayProc(LPVOID lpParameter);		//后台播放线程	

private:
	static CCS m_csSyn;				//用于同步外部调用
	queue<DWORD_PTR> m_soundQueue;		//音频队列，先到先播放
	bool m_bAlreadyInit;		//标志是否已经初始化
	HANDLE m_hPlayThreadHandle;		//播放线程句柄
	HANDLE m_hHasNewSoundEvent;		//有新的声音播放文件
	HANDLE m_hQuitEvent;		//退出事件
};