
//
//CCodeGroup ������飬ÿ��������һ��ID���ṩ������ط���
//

#pragma once

#include "Order.h"

//����ID����
#define GROUPID_MIN				1				//������Сֵ

#define GROUPID_YANG			1				//��
#define GROUPID_MA				2				//��
#define GROUPID_SHE				3				//��
#define GROUPID_LONG			4				//��
#define GROUPID_TU				5				//��
#define GROUPID_HU				6				//��
#define GROUPID_NIU				7				//ţ
#define GROUPID_SHU				8				//��
#define GROUPID_ZHU				9				//��
#define GROUPID_GOU				10				//��
#define GROUPID_JI				11				//��
#define GROUPID_HOU				12				//��


#define GROUPID_TAIL0			13				//β0
#define GROUPID_TAIL1			14				//β1
#define GROUPID_TAIL2			15				//β2
#define GROUPID_TAIL3			16				//β3
#define GROUPID_TAIL4			17				//β4
#define GROUPID_TAIL5			18				//β5
#define GROUPID_TAIL6			19				//β6
#define GROUPID_TAIL7			20				//β7
#define GROUPID_TAIL8			21				//β8
#define GROUPID_TAIL9			22				//β9

#define GROUPID_BIG				23				//��
#define GROUPID_SMALL		24				//С

#define GROUPID_HEAD0		25				//ͷ0
#define GROUPID_HEAD1		26				//ͷ1
#define GROUPID_HEAD2		27				//ͷ2
#define GROUPID_HEAD3		28				//ͷ3
#define GROUPID_HEAD4		29				//ͷ4

#define GROUPID_SINGLE		30				//��
#define GROUPID_DOUBLE		31				//˫

#define GROUPID_RED				32				//��
#define GROUPID_BLUE			33				//��
#define GROUPID_GREEN		34				//��

#define GROUPID_JIAQIN		35				//����
#define GROUPID_YESHOU		36				//Ұ��

#define GROUPID_REDBIG		37				//���
#define GROUPID_REDSMALL	38				//��С
#define GROUPID_BLUEBIG		39				//����
#define GROUPID_BLUESMALL	40			//��С
#define GROUPID_GREENBIG	41				//�̴�
#define GROUPID_GREENSMALL	42		//��С

#define GROUPID_REDSINGLE	43				//�쵥
#define GROUPID_REDDOUBLE	44			//��˫
#define GROUPID_BLUESINGLE	45			//����
#define GROUPID_BLUEDOUBLE	46			//��˫
#define GROUPID_GREENSINGLE		47		//�̵�
#define GROUPID_GREENDOUBLE	48		//��˫

#define GROUPID_SUMSINGLE		49		//�ϵ�
#define GROUPID_SUMDOUBLE		50		//��˫
#define GROUPID_SUMBIG				51		//�ϴ�
#define GROUPID_SUMSMALL		52		//��С

#define GROUPID_BIGSINGLE			53		//��
#define GROUPID_BIGDOUBLE		54		//��˫
#define GROUPID_SMALLSINGLE	55		//С��
#define GROUPID_SMALLDOUBLE	56		//С˫

#define GROUPID_GOLD					57		//��
#define GROUPID_WOOD				58		//ľ
#define GROUPID_WATER				59		//ˮ
#define GROUPID_FIRE						60		//��
#define GROUPID_EARTH					61		//��

#define GROUPID_MAX					61		//�������ֵ

class CCodeGroup
{
public:
	CCodeGroup(int nID);  //nID ����ID
	~CCodeGroup(void);

public:
	//��ȡ��������
	CString Name() const;

	//��ȡ����ID
	int ID() const {return m_nID;}

	//��ȡ�÷�������ĺ���
	vector<CCode> Code() const;

	//�ж��Ƿ�Ϊ���з���
	bool IsWuHangGroup() const;

private:
	vector<CCode> ShengXiaoCode(int nID) const;		//��Ф����
	vector<CCode> TailCode(int nID) const;		//β*
	vector<CCode> HeadCode(int nID) const;		//ͷ*
	vector<CCode> ColorCode(int nID) const;		//��ɫ
	vector<CCode> WuHangCode(int nID) const;		//����
	vector<CCode> BigCode() const;		//����
	vector<CCode> SmallCode() const;		//С��
	vector<CCode> SingleCode() const;		//����
	vector<CCode> DoubleCode() const;		//˫��
	vector<CCode> JiaQinCode() const;		//������
	vector<CCode> YeShouCode() const;		//Ұ����
	vector<CCode> ColorAndBigsmallCode(int nID) const;		//��󡢺�С��
	vector<CCode> ColorAndSingledoubleCode(int nID) const;		//�쵥����˫��
	vector<CCode> SumSingleCode() const;		//�ϵ���
	vector<CCode> SumDoubleCode() const;		//��˫��
	vector<CCode> SumBigCode() const;		//�ϴ���
	vector<CCode> SumSmallCode() const;		//��С��
	vector<CCode> BigSingleCode() const;		//����
	vector<CCode> BigDoubleCode() const;		//��˫��
	vector<CCode> SmallSingleCode() const;		//С����
	vector<CCode> SmallDoubleCode() const;		//С˫��

public:
	//set������Ҫ��Щ����������
	bool operator== (const CCodeGroup& right) const {return m_nID==right.m_nID;}
	bool operator<(const CCodeGroup& right) const {return m_nID < right.m_nID;}

private:
	int m_nID;		//����ID
};
