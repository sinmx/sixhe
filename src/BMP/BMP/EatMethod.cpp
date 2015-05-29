#include "StdAfx.h"
#include "EatMethod.h"

/////////////////////////////////////////////////////////////////////////////////
//  CSingleEatMethod
//

CSingleEatMethod::CSingleEatMethod(const CString& strEat) :
m_strEat(strEat)
{
	//
}

vector<CCode> CSingleEatMethod::Eat()
{
	vector<CCode> codeVec;
	CStringUtil strUtil((LPCTSTR)m_strEat);
	vector<CString> strVec = strUtil.Split(_T(' '));
	for (vector<CString>::const_iterator it=strVec.begin(); it!=strVec.end(); it++)
	{
		if (!Parse((*it), codeVec)) //·Ç·¨´®
		{
			codeVec.clear();
			break;
		}
	}

	return codeVec;
}

bool CSingleEatMethod::IsCorrect()
{
	vector<CCode> codeVec = Eat();
	return codeVec.size()>0;
}

bool CSingleEatMethod::Parse(const CString& str, vector<CCode>& codeVec)
{
	if (str.GetLength() < 3)
	{
		return false;
	}

	CString strCode = str.Left(2);
	CString strMoney = str.Mid(2);

	if (!CStringUtil(strCode).IsAllNumber() || !CStringUtil(strMoney).IsAllNumber())
	{
		return false;
	}

	int nCode = _ttoi(strCode);
	__int64 nMoney = _ttoi64(strMoney);
	if (nCode <= 0 || nCode > COrder::CODE_NUMBER)
	{
		return false;
	}

	CCode code;
	code.SetCode(nCode);
	code.Buy(nMoney);	

	codeVec.push_back(code);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////
//  CAvgEatMethod
//

CAvgEatMethod::CAvgEatMethod(__int64 nAvgEatValue)
{
	m_nAvgEatValue = nAvgEatValue;	
}

vector<CCode> CAvgEatMethod::Eat()
{
	vector<CCode> codeVec;
	for (int i=1; i<= COrder::CODE_NUMBER; i++)
	{
		CCode code;
		code.SetCode(i);
		code.Buy(m_nAvgEatValue);
		
		codeVec.push_back(code);
	}

	return codeVec;
}