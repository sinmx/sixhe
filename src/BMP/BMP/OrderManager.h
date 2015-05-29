
//
//COrderManager ��������ģ�飬�ṩ�Ͷ�����صķ���
//

#pragma once

#include "Order.h"
#include <vector>
using namespace std;

struct ProfitItem  //������
{
	int nCode;	//����	
	__int64 nProfit;	//����

	ProfitItem()
	{
		nCode = 0;		
		nProfit = 0;
	}
};

class COrderManager
{
protected:		//��ֹ��ʵ����
	COrderManager(void);
	~COrderManager(void);

public:
	//��ȡ��һ��ʵ��
	static COrderManager* Instance();

	//��ʼ������ģ��
	void Init();

	//��������б�������ɾ����ע�û���֪ͨ�����б��еĶ���
	void Attach(ICommunication* pObj);
	void Detach(ICommunication* pObj);

	//ɾ��ָ���ͻ��Ķ��������strUserNameΪ�գ�ɾ�����ж���
	bool DeleteOrder(const CString& strUserName);

	//��ȡָ���ͻ��Ķ���
	bool GetOrder(const CString& strUserName, COrder& order) const;

	//����ָ���ͻ��Ķ��������ԭ��û�иÿͻ��Ķ���������һ�ݶ���
	bool UpdateOrder(const CString& strUserName, const COrder& order);
	bool UpdateOrder(const CString& strUserName, const vector<CCode>& buyVec);

	//��ע���ע�����е�һЩ����
	bool OrderAppend(const CString& strUserName, const vector<CCode>& buyVec);

	//�ж��Ƿ��е���
	bool IsHasOrder();

	//�ж��Ƿ�ȫ������
	bool IsAllEat();

	//���³����
	bool UpdateEatTable(const vector<CCode>& eatVec);	

	//��ȡ�������ֵ
	__int64 GetEatValue(int nCode) const;
	__int64 GetEatValue() const;

	//���ó����
	bool RestEatTable();

	//ͳ�ƺ��빺����
	__int64 GetTotalBuy(int nCode) const;  //ͳ��ָ�������ܵĹ�����
	__int64 GetTotalBuy(const CString& strCustomer) const;  //ͳ��ָ���û��ܵĹ�����
	__int64 GetTotalBuy() const;  //ͳ�������û����к��빺���ܶ�

	//��ȡָ��������ϱ�ֵ��Ҳ���Ǹú���ͻ������ܶ�-�Լ������ܶ�
	__int64 GetSelfBuy(int nCode) const;

	//��ȡָ���û�ָ�����빺��ֵ
	__int64 GetBuyValue(const CString& strCustomer, int nCode) const;

	//�����������Ҳ�����ҵ�һ��ƽ������ֵ��ʹ�ò��ܿ��ĸ����룬������׬��ô��Ǯ
	//nAbsProfit ��������ֵ��nAvgEat Ӧ�ó������
	void CalcAbsoluteProfit(__int64& nAbsProfit, __int64& nAvgEat);

	//������С����Ϊָ��ֵʱ��Ӧ��ƽ���������
	void CalcMinProfit(__int64 nMinProfit, __int64& nAvgEat);

	//����ָ��ƽ������ֵʱ��������С����
	void CalcAvgEatProfit(__int64 nAvgEat, __int64& nMinProfit, __int64& nMaxProfit);

	//����ָ�����������
	__int64 CalcCodeProfit(int nCode); 

	//��ȡ��С������󼰶�Ӧ�ĺ���
	void GetMinMaxProfit(__int64& nMinProfit, int& nMinCode, __int64& nMaxProfit, int& nMaxCode);	

	//��ӡ�Լ������
	void PrintSelfBuyTable();

private:
	//��ѯ�ص�������context���ڻص������ģ�recordVec��¼���ݣ�����ֵtrue��ʾ������false��ʾֹͣ
	static bool __stdcall  EnumOrderRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);
	static bool __stdcall  EnumEatRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

	void InitOrder();		//��ʼ������
	void InitEatTable();	//��ʼ�������
	bool InsertOrder(const COrder& order);		//����һ�ݶ���������Ѿ������滻��

	bool InsertEatTable(CCode eatCode[COrder::CODE_NUMBER+1]);		//������������Ѿ������滻��
	bool DeleteEatTable();		//ɾ�����������

	void AvgEat(__int64 nEat, int& nMinCode, __int64& nMinProfit, int& nMaxCode, __int64& nMaxProfit);  //����ƽ���������С������󼰶�Ӧ�����ĺ���
	void CalcAbsoluteProfit(__int64 nMinEat, __int64 nMaxEat, __int64& nAbsProfit, __int64& nAvgEat);	//��[min,max]���������������
	void CalcMinProfit(__int64 nMinEat, __int64 nMaxEat, __int64 nMinProfit, __int64& nAvgEat);  //��[min, max]֮������������>=nMinProfitʱ���óԶ���
	
private:
	static CCS m_synCCS;		//����ͬ���ⲿ����
	vector<ICommunication*> m_listenerVec;	//���������б�
	vector<COrder> m_orderVec;	//�����б�
	CCode m_eatCode[COrder::CODE_NUMBER+1];		//49�����룬���е�0�����ã�ÿ�������ŵ�Ǯ��Ϊ������
};