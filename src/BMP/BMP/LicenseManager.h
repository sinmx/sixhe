
//
//CLicenseManager ��Ȩ����ģ�飬�ṩ��Ȩ��صķ���
//

#pragma once

class CLicenseManager
{
protected:		//��ֹ��ʵ����
	CLicenseManager(void);
	~CLicenseManager(void);

public:
	//��ȡ��һʵ��
	static CLicenseManager* Instance();

	//��ʼ����ģ��
	void Init();

	//Ŀǰ���ð汾�������û����Ͻ��п���
	int GetUserMaxCount() const;

private:
	static CCS m_csSyn;				//����ͬ���ⲿ����	
};