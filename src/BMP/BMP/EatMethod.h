
//
//���ĵ�ʵ���˸��ֳ��뷽ʽ������ƽ�����룬���ų���
//

#pragma once

#include "Order.h"
#include <vector>
using namespace std;

class CEatMethod
{
public:
	//����ӿ�
	virtual vector<CCode> Eat() = 0;
};

class CSingleEatMethod : public CEatMethod  //���ų���
{
public:
	//strEat���봮��ʾ����0520 30500 ��ʾ��05����20 30����500��
	CSingleEatMethod(const CString& strEat);

public:
	//����
	virtual vector<CCode> Eat();

	//�жϳ��봮�Ƿ�Ϸ�
	bool IsCorrect();

private:
	bool Parse(const CString& str, vector<CCode>& codeVec);		//��һ��С���н���һ���������

private:
	CString m_strEat;			//���봮
};

class CAvgEatMethod : public CEatMethod  //ƽ������
{
public:
	CAvgEatMethod(__int64 nAvgEatValue);  //nAvgEatValueƽ������ֵ

public:
	//����
	virtual vector<CCode> Eat();

private:
	__int64 m_nAvgEatValue;  //ƽ������ֵ
};