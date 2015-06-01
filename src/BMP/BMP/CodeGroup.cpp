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
		_T("��"),_T("��"),_T("��"), _T("��"), _T("��"), _T("��"), _T("ţ"), _T("��"), _T("��"), _T("��"), _T("��"), _T("��"),  
		_T("β0"), _T("β1"), _T("β2"), _T("β3"), _T("β4"), _T("β5"), _T("β6"), _T("β7"), _T("β8"), _T("β9"), _T("��"), _T("С"),
		_T("ͷ0"), _T("ͷ1"), _T("ͷ2"), _T("ͷ3"), _T("ͷ4"), _T("��"), _T("˫"), _T("��"), _T("��"), _T("��"), _T("����"), _T("Ұ��"),
		_T("���"), _T("��С"), _T("����"), _T("��С"), _T("�̴�"), _T("��С"), _T("�쵥"), _T("��˫"), _T("����"), _T("��˫"), _T("�̵�"), _T("��˫"),
		_T("�ϵ�"), _T("��˫"), _T("�ϴ�"), _T("��С"), _T("��"), _T("��˫"), _T("С��"), _T("С˫"), _T("��"), _T("ľ"), _T("ˮ"), _T("��"),
		_T("��")
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
	//������Ϊţ������������������
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
	//������Ϊ�ߡ�����������á�����
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
	
	if (m_nID>=GROUPID_YANG && m_nID<=GROUPID_HOU)  //��Ф
	{
		result = ShengXiaoCode(m_nID);
	}
	else if (m_nID>=GROUPID_TAIL0 && m_nID<=GROUPID_TAIL9)  //β0-β9
	{
		result = TailCode(m_nID);
	}
	else if (m_nID>=GROUPID_HEAD0 && m_nID<=GROUPID_HEAD4)  //ͷ0-ͷ4
	{
		result = HeadCode(m_nID);
	}
	else if (m_nID>=GROUPID_RED && m_nID<=GROUPID_GREEN) //������
	{
		result = ColorCode(m_nID);
	}
	else if (m_nID>=GROUPID_GOLD && m_nID<=GROUPID_EARTH) //����
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
	else if (m_nID >= GROUPID_REDBIG && m_nID <= GROUPID_GREENSMALL)  //��ɫ�ʹ�С���
	{
		result = ColorAndBigsmallCode(m_nID);
	}
	else if (m_nID >= GROUPID_REDSINGLE && m_nID <= GROUPID_GREENDOUBLE)  //��ɫ�͵�˫���
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