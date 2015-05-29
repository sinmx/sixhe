
//
//CCustomerManager ��ׯ�û�����ģ�飬������ׯ�û���Ϣ��ȡ���棬�ṩ����ׯ�û���ط���
//

#pragma once

#include "..\..\common\SynObject.h"
#include "..\..\common\DBUtil.h"
#include <vector>
using namespace std;

typedef struct _tagCustomer  //��ׯ�û�
{
	CString strName;		//�û���
	int nDiscount;			//��ˮǧ�ֵ�
	int nBettingOdds;		//����

	_tagCustomer()
	{
		nDiscount = 100;
		nBettingOdds = 41;
	}

	bool IsValid() const
	{
		if (strName.IsEmpty() || nDiscount<0 || nDiscount>1000 || nBettingOdds<0)
		{
			return false;
		}

		return true;
	}
} CUSTOMER, *PCUSTOMER;

class CCustomerManager
{
protected:		//��ֹ��ʵ����
	CCustomerManager(void);
	~CCustomerManager(void);

public:
	//��ȡ��һʵ��
	static CCustomerManager* Instance();

	//��ʼ����ģ��
	void Init();

	//����һ���û������bNeedAutoGenerateΪtrue��ʾ�ڲ��Զ�����һ���û������浽���ݿ��У�customer��Ÿ��û���Ϣ�����ظ�������
	//���bNeedAutoGenerateΪfalse����customerָ�����û���Ϣ���浽���ݿ���
	bool AddCustomer(CUSTOMER& customer, bool bNeedAutoGenerate);

	//ɾ��һ����ׯ�û�
	bool DeleteCustomer(const CString& strName);

	//����һ���û���strOldName����ǰ�û������ƣ�customer���º��û�����Ϣ
	//������µ��ֶ����û�������ôԭ�����û����ȱ�ɾ����Ȼ���������û�
	bool UpdateCustomer(const CString& strOldName, const CUSTOMER& customer);

	//��ȡ�û��б�
	vector<CUSTOMER> GetCustomerList();

	//ѯ�ʵ�ǰ�ж����û�
	int GetCustomerCount() const;

	//ѯ���Ƿ����û�
	bool IsHasCustomer();

	//��������б�������ɾ����ע�û���֪ͨ�����б��еĶ���
	void Attach(ICommunication* pObj);
	void Detach(ICommunication* pObj); 

private:
	//��ѯ�ص�������context���ڻص������ģ�recordVec��¼���ݣ�����ֵtrue��ʾ������false��ʾֹͣ
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);
	CString GenerateOneCustomerName();		//�Զ�����һ���û���
	bool IsValidName(const CString& strName);		//�ж��Ƿ�Ϊ��Ч�û������û����Ƿ��ͻ
	bool InsertCustomer(const CUSTOMER& customer);		//�����û�

private:
	static CCS m_csSyn;				//����ͬ���ⲿ����
	vector<CUSTOMER> m_customerVec;		//��ׯ�û��б�
	vector<ICommunication*> m_listenerVec;	//���������б�
};