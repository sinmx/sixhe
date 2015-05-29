
//
//CSoundManager ��Ƶ���Ź���ģ�飬�ṩ������Ƶ�ķ���
//

#pragma once

class CSoundManager
{
protected:		//��ֹ��ʵ����
	CSoundManager(void);
	~CSoundManager(void);

public:
	//��ȡ��һʵ��
	static CSoundManager* Instance();

	//������Ƶ�ļ���idΪ��ԴID
	void Play(DWORD_PTR id);

	//��ȡһ����Ƶ�ļ����ţ�����ֵ0��ʾû����Ƶ�ļ�
	DWORD_PTR GetOneSound();

	//֪ͨģ���˳�
	void Exit();

private:
	static UINT WINAPI PlayProc(LPVOID lpParameter);		//��̨�����߳�	

private:
	static CCS m_csSyn;				//����ͬ���ⲿ����
	queue<DWORD_PTR> m_soundQueue;		//��Ƶ���У��ȵ��Ȳ���
	bool m_bAlreadyInit;		//��־�Ƿ��Ѿ���ʼ��
	HANDLE m_hPlayThreadHandle;		//�����߳̾��
	HANDLE m_hHasNewSoundEvent;		//���µ����������ļ�
	HANDLE m_hQuitEvent;		//�˳��¼�
};