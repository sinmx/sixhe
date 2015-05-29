
//
//���ĵ�ʵ���˸��ֹ�����ע��ʽ������������ע���������ע��������ע���ȶ���ע
//

#pragma once

#include "Order.h"
#include "CodeGroup.h"
#include <vector>
using namespace std;

class CBuyMethod
{
public:
	//������ע�ӿ�
	virtual vector<CCode> Buy() = 0;

	//��ע��־�ӿ�
	virtual CString Log() = 0;

	//�ж��û����������Ƿ���ȷ
	virtual bool IsCorrect() = 0;
};

class CSingleBuyMethod : public CBuyMethod  //���Ź���
{
public:
	//strBuy���򴮣�ʾ����2030 05-123 ��ʾ����20��ע30������05��ע123
	CSingleBuyMethod(const CString& strBuy);

public:
	//������ע
	virtual vector<CCode> Buy();

	//�ж��û����������Ƿ���ȷ
	virtual bool IsCorrect();

	//��ע��־
	virtual CString Log();

	//�����Ƿ�Ϊ��ע�����Ǽ�ע������ע
	void SetAddFlag(bool flag) {m_bAdd = flag;}

private:
	bool Parse(const CString& str, vector<CCode>& codeVec);		//��һ��С���н���һ�����빺��

private:
	CString m_strBuy;			//����
	bool m_bAdd;				//��ʶ�Ƿ�Ϊ��ע
};

class CEqualBuyMethod : public CBuyMethod  //�ȶ��
{
public:
	//strCode ���봮������֮��ʹ�ÿո�ֿ�
	CEqualBuyMethod(const CString& strCode, __int64 nBuyValue);

public:
	//������ע
	virtual vector<CCode> Buy();

	//�ж��û����������Ƿ���ȷ
	virtual bool IsCorrect();

	//��ע��־
	virtual CString Log();

private:
	CString m_strCode;			//���봮
	__int64 m_nBuyValue;		//����ֵ
};


class CIntervalBuyMethod : public CBuyMethod  //�ضι���
{
public:	
	CIntervalBuyMethod(int nStartCode, int nEndCode, __int64 nBuyValue);

public:
	//������ע
	virtual vector<CCode> Buy();

	//�ж��û����������Ƿ���ȷ
	virtual bool IsCorrect();

	//��ע��־
	virtual CString Log();

private:
	int m_nStartCode;
	int m_nEndCode;
	__int64 m_nBuyValue;
};

class CGroupBuyMethod : public CBuyMethod  //���鹺��
{
public:
	//groupVec����ͬʱ���������飬ȡ���벢��
	CGroupBuyMethod(const set<CCodeGroup> groupSet, __int64 nBuyMoney);

public:
	//������ע
	virtual vector<CCode> Buy();

	//�ж��û����������Ƿ���ȷ
	virtual bool IsCorrect();

	//��ע��־
	virtual CString Log();

private:
	set<CCodeGroup> m_groupSet;		//�������
	__int64 m_nBuyMoney;		//������
};