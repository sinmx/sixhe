///////////////////////////////////////////
//������CCommunicationManager	 ͨ�Ź�����
//ְ�𣺵�һʵ�����н���ģʽ�����������ͨ�ŷ�ʽ���򻯶���������
////////////////////////////////////////////////////////

#pragma once

#include "SynObject.h"
#include <vector>
using namespace std;

class ICommunication  //��ͨ�Ŷ���ӿ�
{
public:
	//��ȡͨ��ΨһIDֵ
	virtual int GetID() = 0;

	//�����¼���wParam lParam ����ֵ�����ھ����nEventCode
	virtual int AcceptEvent(int nEventCode, WPARAM wParam=0, LPARAM lParam=0) = 0;
};

class CCommunicationManager
{
protected: //��ֹ��ʵ����
	CCommunicationManager();
	~CCommunicationManager();

public:
	//��ȡ��һʵ������
	static CCommunicationManager* Instance();

	//����ɾ������ͨ���б���
	void Attach(ICommunication* pObj);
	void Detach(ICommunication* pObj);

	//ͨ�Žӿڣ�nID���շ���ΨһID�����������ͷ���ֵͬ�ӿ�AcceptEvent
	int Notify(int nID, int nEventCode, WPARAM wParam=0, LPARAM lParam=0);

protected:
	vector<ICommunication*> m_comObjVector;		//���е�ͨ�Ŷ���
	static CCS m_csSyn;				//����ͬ���ⲿ����
};