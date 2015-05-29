
//
//CRewardManager	�ҽ��������������ṩ�ҽ���صķ���
//

#pragma once

typedef struct _tagRewardItem
{
	CString strCustomer;		//�û�
	__int64 nProfit;				//����
	__int64 nBuy;					//������
	int nDiscount;				//�ۿ�
	__int64 nWin;					//�н����
	int nBettingOdds;			//����
	bool bAlreadyReward;		//��־�Ƿ��Ѿ��Թ���

	_tagRewardItem()
	{
		nProfit = 0;
		nBuy = 0;
		nDiscount = 0;
		nWin = 0;
		nBettingOdds = 0;
		bAlreadyReward = false;
	}

}REWARD_ITEM, *PREWARD_ITEM;

class CRewardManager
{
protected:  //��ֹ��ʵ����
	CRewardManager();
	~CRewardManager();

public:
	//��ȡ��һʵ��
	static CRewardManager* Instance();

	//��ʼ��
	void Init();

	//��ȡ�ҽ���ϸ��Ϣ
	vector<REWARD_ITEM> GetRewardInfo();

	//�����Ѿ��ҽ���־
	bool SaveAlreadyReward(const CString& strCustomer, bool bAlready);

	//�����Լ�����
	__int64 CalcSelfProfit();		

	//��ӡ�ҽ���
	void PrintRewardTable();

private:
	//��ѯ�ص�������context���ڻص������ģ�recordVec��¼���ݣ�����ֵtrue��ʾ������false��ʾֹͣ
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

	bool IsAlreadyReward(const CString& strCustomer);  //�ж��Ƿ��Ѿ��ҹ���
	CString PaddingWhiteSpace(const CString& strSource, int nMaxChar);  //����nMaxChar���ַ������ո�һ����ASCII���ַ�������2���ַ�

private:
	static CCS m_synCCS;					//����ͬ���ⲿ����
	vector<REWARD_ITEM> m_alreadyRewardVec;		//�ҽ���Ϣ��Ŀǰֻ��bAlreadyReward����
};