//
//CCode ��ʾһ�����룬�ṩ������صķ�������ʲô��Ф���Ǵ���С��ʲô��ɫ��
//COrder ���һ�Ŷ��������ݣ��ṩ�Ͷ����йصķ���
//

#pragma once

class CCode		//����
{
public:
	CCode();
	~CCode();

public:
	//���򼰻�ȡ
	void Buy(__int64 nMoney) {m_nMoney = nMoney;}
	__int64 Money() const {return m_nMoney;}

	//���û�ȡ����ֵ
	void SetCode(int nValue) {m_nValue = nValue;}
	int GetCode() const {return m_nValue;}

	//��ȡ��Ф������
	CString GetShengXiao() const;

	//�ж���ɫ
	bool IsRed() const;
	bool IsBlue() const;
	bool IsGreen() const;

	//�ж��Ǵ���С
	bool IsBig() const;
	bool IsSmall() const;

	//�ж��ǵ�����˫
	bool IsDouble() const;
	bool IsSingle() const;

	//������λ����ǵ�����˫
	bool IsSumSingle() const;
	bool IsSumDouble() const;

	//������λ����Ǵ���С
	bool IsSumBig() const;
	bool IsSumSmall() const;

public:
	//set������Ҫ��Щ����������
	bool operator== (const CCode& right) const {return m_nValue==right.m_nValue;}
	bool operator<(const CCode& right) const {return m_nValue < right.m_nValue;}

private:
	int m_nValue;		//����ֵ
	__int64 m_nMoney;		//�����Ǯ��
};

class COrder
{
public:
	COrder(void);
	~COrder(void);

	enum
	{
		CODE_NUMBER = 49,		//�������
	};

public:
	//��ȡ���й�����
	__int64 GetTotalBuyMoney() const;

	//����ָ���ĺ���
	void Buy(int nIndex, __int64 nMoney);

	//��ȡָ�����빺����
	__int64 Money(int nIndex) const;

	//���û�ȡ�û���
	void Name(const CString& strName) {m_strCustomerName = strName;}
	CString Name() const {return m_strCustomerName;}

private:
	CString m_strCustomerName;		//�û���
	CCode m_code[CODE_NUMBER+1];		//49�����룬���е�0������
};