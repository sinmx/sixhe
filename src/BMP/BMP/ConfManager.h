
//
//CConfManager ���ù��������ṩ������صķ���
//

#pragma once

class CConfManager
{
protected:  //��ֹ��ʵ����
	CConfManager(void);
	~CConfManager(void);

public:
	//��ȡ��һʵ������
	static CConfManager* Instance();

	//��ʼ����������
	void Init();

	//��ȡ���浱ǰ���ݿ�ģʽ�汾��
	bool DBSchemeVersion(int nVersion);
	int DBSchemeVersion() const;

	//�ж����������Ƿ�ΪĬ��ֵ
	bool IsWuHangDefaultValue() const;

	//��ȡ�������з������
	CString GoldCode() const;
	bool GoldCode(const CString& strCode);
	CString WoodCode() const;
	bool WoodCode(const CString& strCode);
	CString WaterCode() const;
	bool WaterCode(const CString& strCode);
	CString FireCode() const;
	bool FireCode(const CString& strCode);
	CString EarthCode() const;
	bool EarthCode(const CString& strCode);	

	//��ȡ�����Ƿ��������У��
	bool IsSoundCheck() const;
	bool SoundCheck(bool bEnable);

	//��ȡ����ƽ����ˮ
	int AvgDiscount() const;
	bool AvgDiscount(int value);

	//��ȡ����ƽ������
	int AvgBettingOdds() const;
	bool AvgBettingOdds(int nValue);

	//��ȡ�����н�����
	int WinCode() const;
	bool WinCode(int nCodeValue);

	//����ׯ����ʱ��ˮ
	int SelfBuyDiscount() const;
	bool SelfBuyDiscount(int nValue);

	//����ׯ����ʱ������
	int SelfBuyBettingOdds() const;
	bool SelfBuyBettingOdds(int nValue);

	//��ʾ�û���������
	bool IsNeedHintConfigureWuHang() const;
	bool NeedHintConfigureWuHang(bool bNeed);

private:
	//��ѯ�ص�������context���ڻص������ģ�recordVec��¼���ݣ�����ֵtrue��ʾ������false��ʾֹͣ
	static bool __stdcall  EnumRecord(LPARAM context, const vector<COLUMN_ITEM>& recordVec);

	bool Save(const CString& strName, const CString& strValue);   //���浽���ݿ���

private:
	static CCS m_synCCS;			//����ͬ���ⲿ����
	map<CString, CString> m_confMap;		//��������
};