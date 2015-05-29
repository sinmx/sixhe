#include "StdAfx.h"
#include "BuyMethod.h"

/////////////////////////////////////////////////////////////////////////////////
//  CSingleBuyMethod
//

CSingleBuyMethod::CSingleBuyMethod(const CString& strBuy)
{
	m_strBuy = strBuy;
	m_bAdd = true;
}

vector<CCode> CSingleBuyMethod::Buy()
{
	vector<CCode> codeVec;
	CStringUtil strUtil((LPCTSTR)m_strBuy);
	vector<CString> strVec = strUtil.Split(_T(' '));
	for (vector<CString>::const_iterator it=strVec.begin(); it!=strVec.end(); it++)
	{
		if (!Parse((*it), codeVec)) //非法串
		{
			codeVec.clear();
			break;
		}
	}

	return codeVec;
}

bool CSingleBuyMethod::IsCorrect()
{
	vector<CCode> codeVec = Buy();
	return codeVec.size()>0;
}

bool CSingleBuyMethod::Parse(const CString& str, vector<CCode>& codeVec)
{
	if (str.GetLength() < 3)
	{
		return false;
	}

	CString strCode = str.Left(2);
	CString strMoney = str.Mid(2);

	if (!CStringUtil(strCode).IsAllNumber() || !CStringUtil(strMoney).IsNumber())
	{
		return false;
	}

	int nCode = _ttoi(strCode);
	__int64 nMoney = _ttoi64(strMoney);
	if (nCode <=0 || nCode > COrder::CODE_NUMBER)
	{
		return false;
	}
	
	CCode code;
	code.SetCode(nCode);
	code.Buy(nMoney);

	codeVec.push_back(code);

	return true;
}

CString CSingleBuyMethod::Log()
{
	CString strTemp;
	CString strItem;
	vector<CCode> codeVec = Buy();
	for (vector<CCode>::const_iterator it=codeVec.begin(); it!=codeVec.end(); it++)
	{
		if (it != codeVec.begin())
		{
			strTemp += _T(' ');
		}
		
		if (it->Money() > 0)
		{
			if (m_bAdd)
			{
				//43=加注
				strItem.Format(_T("%02d%s%I64d"), it->GetCode(), CLanguageManager::Instance()->LoadString(43), it->Money());
			}
			else
			{
				//36=下注
				strItem.Format(_T("%02d%s%I64d"), it->GetCode(), CLanguageManager::Instance()->LoadString(36), it->Money());
			}
		}
		else
		{
			//42=减注
			strItem.Format(_T("%02d%s%I64d"), it->GetCode(), CLanguageManager::Instance()->LoadString(42), it->Money()*(-1));
		}
		strTemp += strItem;
	}

	return strTemp;
}

/////////////////////////////////////////////////////////////////////////////////
//  CIntervalBuyMethod
//

CIntervalBuyMethod::CIntervalBuyMethod(int nStartCode, int nEndCode, __int64 nBuyValue)
{
	m_nBuyValue = nBuyValue;

	if (nEndCode > nStartCode)
	{
		m_nStartCode = nStartCode;
		m_nEndCode = nEndCode;
	}
	else
	{
		m_nStartCode = nEndCode;
		m_nEndCode = nStartCode;
	}

	if (m_nStartCode > COrder::CODE_NUMBER || m_nEndCode < 1) //用户输入都大于49或都小于1，认为无效
	{
		m_nStartCode = 0;
		m_nEndCode = -1;
		return;
	}
	
	if (m_nStartCode <= 0)
	{
		m_nStartCode = 1;
	}	

	if (m_nEndCode > COrder::CODE_NUMBER)
	{
		m_nEndCode = COrder::CODE_NUMBER;
	}	
}

vector<CCode> CIntervalBuyMethod::Buy()
{	
	vector<CCode> codeVec;
	for (int i=m_nStartCode; i<=m_nEndCode; i++)
	{
		CCode code;
		code.SetCode(i);
		code.Buy(m_nBuyValue);
		codeVec.push_back(code);
	}

	return codeVec;
}

bool CIntervalBuyMethod::IsCorrect()
{
	return true;
}

CString CIntervalBuyMethod::Log()
{
	if (m_nStartCode > m_nEndCode)
	{
		return _T("");
	}
	
	CString strTemp;
	if (m_nBuyValue >= 0)
	{
		//43=加注
		strTemp.Format(_T("%02d-%02d%s%I64d"), m_nStartCode, m_nEndCode, CLanguageManager::Instance()->LoadString(43), m_nBuyValue);
	}
	else
	{
		//42=减注
		strTemp.Format(_T("%02d-%02d%s%I64d"), m_nStartCode, m_nEndCode, CLanguageManager::Instance()->LoadString(42), m_nBuyValue*(-1));
	}
	return strTemp;
}

/////////////////////////////////////////////////////////////////////////////////
//  CEqualBuyMethod
//

CEqualBuyMethod::CEqualBuyMethod(const CString& strCode, __int64 nBuyValue) : 
m_strCode(strCode), 
m_nBuyValue(nBuyValue)
{
	//
}

vector<CCode> CEqualBuyMethod::Buy()
{
	vector<CCode> codeVec;
	CStringUtil strUtil((LPCTSTR)m_strCode);
	vector<CString> strVec = strUtil.Split(_T(' '));
	for (vector<CString>::const_iterator it=strVec.begin(); it!=strVec.end(); it++)
	{
		CStringUtil codeUtil(*it);		
		if (!codeUtil.IsAllNumber())
		{
			codeVec.clear();
			return codeVec;
		}
		else
		{
			int nCode = _ttoi(*it);
			if (nCode <= 0 || nCode > COrder::CODE_NUMBER)
			{
				codeVec.clear();
				return codeVec;
			}
			else
			{
				CCode code;
				code.SetCode(nCode);
				code.Buy(m_nBuyValue);
				codeVec.push_back(code);
			}
		}
	}

	return codeVec;
}

bool CEqualBuyMethod::IsCorrect()
{
	if (Buy().empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

CString CEqualBuyMethod::Log()
{
	CString strTemp;
	CString strItem;
	vector<CCode> codeVec = Buy();
	for (vector<CCode>::const_iterator it=codeVec.begin(); it!=codeVec.end(); it++)
	{
		if (it != codeVec.begin())
		{
			strTemp += _T(' ');
		}
		
		strItem.Format(_T("%02d"), it->GetCode());
		strTemp += strItem;
	}
	
	if (m_nBuyValue >= 0)
	{
		//43=加注
		strTemp += CLanguageManager::Instance()->LoadString(43) + CStringUtil::Int64ToStr(m_nBuyValue);
	}
	else
	{
		//42=减注
		strTemp += CLanguageManager::Instance()->LoadString(42) + CStringUtil::Int64ToStr(m_nBuyValue*(-1));
	}
	
	return strTemp;
}

/////////////////////////////////////////////////////////////////////////////////
//  CGroupBuyMethod
//

CGroupBuyMethod::CGroupBuyMethod(const set<CCodeGroup> groupSet, __int64 nBuyMoney) :
m_groupSet(groupSet.begin(), groupSet.end()),
m_nBuyMoney(nBuyMoney)
{
	
}

vector<CCode> CGroupBuyMethod::Buy()
{	
	set<CCode> codeSet;
	for (set<CCodeGroup>::const_iterator it=m_groupSet.begin(); it!=m_groupSet.end(); it++)
	{
		vector<CCode> tempVec = it->Code();
		for (vector<CCode>::iterator it2=tempVec.begin(); it2!=tempVec.end(); it2++)
		{
			it2->Buy(m_nBuyMoney);
			codeSet.insert((*it2));
		}
	}

	vector<CCode> codeVec;
	codeVec.assign(codeSet.begin(), codeSet.end());
	return codeVec;
}

bool CGroupBuyMethod::IsCorrect()
{
	return true;
}

CString CGroupBuyMethod::Log()
{
	CString strGroup;
	for (set<CCodeGroup>::const_iterator it=m_groupSet.begin(); it!=m_groupSet.end(); it++)
	{
		if (it != m_groupSet.begin())  //除第一个外，添加前加分隔符
		{
			strGroup += _T('、');
		}

		strGroup += it->Name();
	}
	
	CString strTemp;
	if (m_nBuyMoney >= 0)
	{
		//7=分组(%s)加注%I64d
		strTemp.Format(CLanguageManager::Instance()->LoadString(7), strGroup, m_nBuyMoney);
	}
	else
	{
		//44=分组(%s)减注%I64d
		strTemp.Format(CLanguageManager::Instance()->LoadString(44), strGroup, m_nBuyMoney*(-1));
	}
	
	return strTemp;
}