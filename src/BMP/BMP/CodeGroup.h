
//
//CCodeGroup 号码分组，每个分组有一个ID，提供分组相关服务
//

#pragma once

#include "Order.h"

//分组ID定义
#define GROUPID_MIN				1				//分组最小值

#define GROUPID_YANG			1				//羊
#define GROUPID_MA				2				//马
#define GROUPID_SHE				3				//蛇
#define GROUPID_LONG			4				//龙
#define GROUPID_TU				5				//兔
#define GROUPID_HU				6				//虎
#define GROUPID_NIU				7				//牛
#define GROUPID_SHU				8				//鼠
#define GROUPID_ZHU				9				//猪
#define GROUPID_GOU				10				//狗
#define GROUPID_JI				11				//鸡
#define GROUPID_HOU				12				//猴


#define GROUPID_TAIL0			13				//尾0
#define GROUPID_TAIL1			14				//尾1
#define GROUPID_TAIL2			15				//尾2
#define GROUPID_TAIL3			16				//尾3
#define GROUPID_TAIL4			17				//尾4
#define GROUPID_TAIL5			18				//尾5
#define GROUPID_TAIL6			19				//尾6
#define GROUPID_TAIL7			20				//尾7
#define GROUPID_TAIL8			21				//尾8
#define GROUPID_TAIL9			22				//尾9

#define GROUPID_BIG				23				//大
#define GROUPID_SMALL		24				//小

#define GROUPID_HEAD0		25				//头0
#define GROUPID_HEAD1		26				//头1
#define GROUPID_HEAD2		27				//头2
#define GROUPID_HEAD3		28				//头3
#define GROUPID_HEAD4		29				//头4

#define GROUPID_SINGLE		30				//单
#define GROUPID_DOUBLE		31				//双

#define GROUPID_RED				32				//红
#define GROUPID_BLUE			33				//蓝
#define GROUPID_GREEN		34				//绿

#define GROUPID_JIAQIN		35				//家禽
#define GROUPID_YESHOU		36				//野兽

#define GROUPID_REDBIG		37				//红大
#define GROUPID_REDSMALL	38				//红小
#define GROUPID_BLUEBIG		39				//蓝大
#define GROUPID_BLUESMALL	40			//蓝小
#define GROUPID_GREENBIG	41				//绿大
#define GROUPID_GREENSMALL	42		//绿小

#define GROUPID_REDSINGLE	43				//红单
#define GROUPID_REDDOUBLE	44			//红双
#define GROUPID_BLUESINGLE	45			//蓝单
#define GROUPID_BLUEDOUBLE	46			//蓝双
#define GROUPID_GREENSINGLE		47		//绿单
#define GROUPID_GREENDOUBLE	48		//绿双

#define GROUPID_SUMSINGLE		49		//合单
#define GROUPID_SUMDOUBLE		50		//合双
#define GROUPID_SUMBIG				51		//合大
#define GROUPID_SUMSMALL		52		//合小

#define GROUPID_BIGSINGLE			53		//大单
#define GROUPID_BIGDOUBLE		54		//大双
#define GROUPID_SMALLSINGLE	55		//小单
#define GROUPID_SMALLDOUBLE	56		//小双

#define GROUPID_GOLD					57		//金
#define GROUPID_WOOD				58		//木
#define GROUPID_WATER				59		//水
#define GROUPID_FIRE						60		//火
#define GROUPID_EARTH					61		//土

#define GROUPID_MAX					61		//分组最大值

class CCodeGroup
{
public:
	CCodeGroup(int nID);  //nID 分组ID
	~CCodeGroup(void);

public:
	//获取分组名称
	CString Name() const;

	//获取分组ID
	int ID() const {return m_nID;}

	//获取该分组包含的号码
	vector<CCode> Code() const;

	//判断是否为五行分组
	bool IsWuHangGroup() const;

private:
	vector<CCode> ShengXiaoCode(int nID) const;		//生肖号码
	vector<CCode> TailCode(int nID) const;		//尾*
	vector<CCode> HeadCode(int nID) const;		//头*
	vector<CCode> ColorCode(int nID) const;		//颜色
	vector<CCode> WuHangCode(int nID) const;		//五行
	vector<CCode> BigCode() const;		//大码
	vector<CCode> SmallCode() const;		//小码
	vector<CCode> SingleCode() const;		//单码
	vector<CCode> DoubleCode() const;		//双码
	vector<CCode> JiaQinCode() const;		//家禽码
	vector<CCode> YeShouCode() const;		//野兽码
	vector<CCode> ColorAndBigsmallCode(int nID) const;		//红大、红小码
	vector<CCode> ColorAndSingledoubleCode(int nID) const;		//红单、红双码
	vector<CCode> SumSingleCode() const;		//合单码
	vector<CCode> SumDoubleCode() const;		//合双码
	vector<CCode> SumBigCode() const;		//合大码
	vector<CCode> SumSmallCode() const;		//合小码
	vector<CCode> BigSingleCode() const;		//大单码
	vector<CCode> BigDoubleCode() const;		//大双码
	vector<CCode> SmallSingleCode() const;		//小单码
	vector<CCode> SmallDoubleCode() const;		//小双码

public:
	//set容器需要这些操作符重载
	bool operator== (const CCodeGroup& right) const {return m_nID==right.m_nID;}
	bool operator<(const CCodeGroup& right) const {return m_nID < right.m_nID;}

private:
	int m_nID;		//分组ID
};
