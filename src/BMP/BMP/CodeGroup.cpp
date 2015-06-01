#include "StdAfx.h"
#include "CodeGroup.h"
#include "ConfManager.h"

CCodeGroup::CCodeGroup(int nID) 
{
	m_nID = nID;
	if (m_nID<GROUPID_MIN || m_nID>GROUPID_MAX)
	{
		m_nID = 0;
	}
}

CCodeGroup::~CCodeGroup(void)
{
}

CString CCodeGroup::Name() const
{
	static CString strName[] = {
		_T(""),
		_T("羊"),_T("马"),_T("蛇"), _T("龙"), _T("兔"), _T("虎"), _T("牛"), _T("鼠"), _T("猪"), _T("狗"), _T("鸡"), _T("猴"),  
		_T("尾0"), _T("尾1"), _T("尾2"), _T("尾3"), _T("尾4"), _T("尾5"), _T("尾6"), _T("尾7"), _T("尾8"), _T("尾9"), _T("大"), _T("小"),
		_T("头0"), _T("头1"), _T("头2"), _T("头3"), _T("头4"), _T("单"), _T("双"), _T("红"), _T("蓝"), _T("绿"), _T("家禽"), _T("野兽"),
		_T("红大"), _T("红小"), _T("蓝大"), _T("蓝小"), _T("绿大"), _T("绿小"), _T("红单"), _T("红双"), _T("蓝单"), _T("蓝双"), _T("绿单"), _T("绿双"),
		_T("合单"), _T("合双"), _T("合大"), _T("合小"), _T("大单"), _T("大双"), _T("小单"), _T("小双"), _T("金"), _T("木"), _T("水"), _T("火"),
		_T("土")
	};

	return strName[m_nID];
}

vector<CCode> CCodeGroup::ShengXiaoCode(int nID) const
{
	vector<CCode> result;
	for (int i=0; i<=4; i++)
	{
		int nCode = 12*i+(nID-GROUPID_YANG+1);
		if (nCode <= COrder::CODE_NUMBER)
		{
			CCode code;
			code.SetCode(nCode);
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::TailCode(int nID) const
{
	vector<CCode> result;
	for (int i=0; i<=4; i++)
	{
		int nCode = i*10+(nID-GROUPID_TAIL0);
		if (nCode>0 && nCode<=COrder::CODE_NUMBER)
		{
			CCode code;
			code.SetCode(nCode);
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::HeadCode(int nID) const
{
	vector<CCode> result;
	for (int i=0; i<10; i++)
	{
		int nCode = (nID-GROUPID_HEAD0)*10 + i;
		if (nCode>0 && nCode<=COrder::CODE_NUMBER)
		{
			CCode code;
			code.SetCode(nCode);
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::ColorCode(int nID) const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (nID == GROUPID_RED && code.IsRed())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_BLUE && code.IsBlue())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_GREEN && code.IsGreen())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::WuHangCode(int nID) const
{
	vector<CCode> result;

	CString strCode;
	if (nID == GROUPID_GOLD)
	{
		strCode = CConfManager::Instance()->GoldCode();
	}
	else if (nID == GROUPID_WOOD)
	{
		strCode = CConfManager::Instance()->WoodCode();
	}
	else if (nID == GROUPID_WATER)
	{
		strCode = CConfManager::Instance()->WaterCode();
	}
	else if (nID == GROUPID_FIRE)
	{
		strCode = CConfManager::Instance()->FireCode();
	}
	else if (nID == GROUPID_EARTH)
	{
		strCode = CConfManager::Instance()->EarthCode();
	}

	CStringUtil strUtil(strCode);
	vector<CString> strVec = strUtil.Split(_T(' '));
	for (vector<CString>::const_iterator it=strVec.begin(); it!=strVec.end(); it++)
	{
		int nCode = _ttoi(*it);
		if (nCode > 0 && nCode <= COrder::CODE_NUMBER)
		{
			CCode code;
			code.SetCode(nCode);
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::JiaQinCode() const
{
	//家禽码为牛、猪、狗、鸡、羊、马码
	vector<CCode> result = ShengXiaoCode(GROUPID_NIU);
	vector<CCode> result2 = ShengXiaoCode(GROUPID_ZHU);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_GOU);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_JI);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_YANG);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_MA);
	result.insert(result.end(), result2.begin(), result2.end());

	return result;
}

vector<CCode> CCodeGroup::YeShouCode() const
{
	//家禽码为蛇、龙、虎、猴、兔、鼠码
	vector<CCode> result = ShengXiaoCode(GROUPID_SHE);
	vector<CCode> result2 = ShengXiaoCode(GROUPID_LONG);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_HU);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_HOU);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_TU);
	result.insert(result.end(), result2.begin(), result2.end());
	result2 = ShengXiaoCode(GROUPID_SHU);
	result.insert(result.end(), result2.begin(), result2.end());

	return result;
}

vector<CCode> CCodeGroup::ColorAndBigsmallCode(int nID) const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (nID == GROUPID_REDBIG && code.IsRed() && code.IsBig())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_REDSMALL && code.IsRed() && code.IsSmall())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_BLUEBIG && code.IsBlue() && code.IsBig())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_BLUESMALL && code.IsBlue() && code.IsSmall())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_GREENBIG && code.IsGreen() && code.IsBig())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_GREENSMALL && code.IsGreen() && code.IsSmall())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::ColorAndSingledoubleCode(int nID) const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (nID == GROUPID_REDSINGLE && code.IsRed() && code.IsSingle())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_REDDOUBLE && code.IsRed() && code.IsDouble())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_BLUESINGLE && code.IsBlue() && code.IsSingle())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_BLUEDOUBLE && code.IsBlue() && code.IsDouble())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_GREENSINGLE && code.IsGreen() && code.IsSingle())
		{
			result.push_back(code);
		}
		else if (nID == GROUPID_GREENDOUBLE && code.IsGreen() && code.IsDouble())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::SumSingleCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSumSingle())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::SumDoubleCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSumDouble())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::SumBigCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSumBig())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::SumSmallCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSumSmall())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::BigSingleCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsBig() && code.IsSingle())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::BigDoubleCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsBig() && code.IsDouble())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::SmallSingleCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSmall() && code.IsSingle())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::SmallDoubleCode() const
{
	vector<CCode> result;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSmall() && code.IsDouble())
		{
			result.push_back(code);
		}
	}

	return result;
}

vector<CCode> CCodeGroup::BigCode() const
{
	vector<CCode> codeVec;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsBig())
		{
			codeVec.push_back(code);
		}
	}

	return codeVec;
}

vector<CCode> CCodeGroup::SmallCode() const
{
	vector<CCode> codeVec;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSmall())
		{
			codeVec.push_back(code);
		}		
	}

	return codeVec;
}

vector<CCode> CCodeGroup::SingleCode() const
{
	vector<CCode> codeVec;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsSingle())
		{			
			codeVec.push_back(code);
		}		
	}

	return codeVec;
}

vector<CCode> CCodeGroup::DoubleCode() const
{
	vector<CCode> codeVec;
	for (int i=1; i<=COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		if (code.IsDouble())
		{			
			codeVec.push_back(code);
		}		
	}

	return codeVec;
}

bool CCodeGroup::IsWuHangGroup() const
{
	if (m_nID >= GROUPID_GOLD && m_nID <= GROUPID_EARTH)
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<CCode> CCodeGroup::Code() const
{
	vector<CCode> result;
	
	if (m_nID>=GROUPID_YANG && m_nID<=GROUPID_HOU)  //生肖
	{
		result = ShengXiaoCode(m_nID);
	}
	else if (m_nID>=GROUPID_TAIL0 && m_nID<=GROUPID_TAIL9)  //尾0-尾9
	{
		result = TailCode(m_nID);
	}
	else if (m_nID>=GROUPID_HEAD0 && m_nID<=GROUPID_HEAD4)  //头0-头4
	{
		result = HeadCode(m_nID);
	}
	else if (m_nID>=GROUPID_RED && m_nID<=GROUPID_GREEN) //红蓝绿
	{
		result = ColorCode(m_nID);
	}
	else if (m_nID>=GROUPID_GOLD && m_nID<=GROUPID_EARTH) //五行
	{
		result = WuHangCode(m_nID);
	}
	else if (m_nID == GROUPID_BIG)
	{
		result = BigCode();
	}
	else if (m_nID == GROUPID_SMALL)
	{
		result = SmallCode();
	}
	else if (m_nID == GROUPID_SINGLE)
	{
		result = SingleCode();
	}
	else if (m_nID == GROUPID_DOUBLE)
	{
		result = DoubleCode();
	}
	else if (m_nID == GROUPID_JIAQIN)
	{
		result = JiaQinCode();
	}
	else if (m_nID == GROUPID_YESHOU)
	{
		result = YeShouCode();
	}
	else if (m_nID >= GROUPID_REDBIG && m_nID <= GROUPID_GREENSMALL)  //颜色和大小组合
	{
		result = ColorAndBigsmallCode(m_nID);
	}
	else if (m_nID >= GROUPID_REDSINGLE && m_nID <= GROUPID_GREENDOUBLE)  //颜色和单双组合
	{
		result = ColorAndSingledoubleCode(m_nID);
	}
	else if (m_nID == GROUPID_SUMSINGLE)
	{
		result = SumSingleCode();
	}
	else if (m_nID == GROUPID_SUMDOUBLE)
	{
		result = SumDoubleCode();
	}
	else if (m_nID == GROUPID_SUMBIG)
	{
		result = SumBigCode();
	}
	else if (m_nID == GROUPID_SUMSMALL)
	{
		result = SumSmallCode();
	}
	else if (m_nID == GROUPID_BIGSINGLE)
	{
		result = BigSingleCode();
	}
	else if (m_nID == GROUPID_BIGDOUBLE)
	{
		result = BigDoubleCode();
	}
	else if (m_nID == GROUPID_SMALLSINGLE)
	{
		result = SmallSingleCode();
	}
	else if (m_nID == GROUPID_SMALLDOUBLE)
	{
		result = SmallDoubleCode();
	}

	return result;
}