#include "StdAfx.h"
#include "Order.h"

///////////////////////////////////////////////////////////////////////////////////////////
//  CCode
//

CCode::CCode() :
m_nValue(0),
m_nMoney(0)
{
	//
}

CCode::~CCode()
{
	//
}

CString CCode::GetShengXiao() const
{
	static CString strShengXiao[] = {_T("∫Ô"), _T("—Ú"), _T("¬Ì"), _T("…ﬂ"), _T("¡˙"), _T("Õ√"), _T("ª¢"), _T("≈£"), _T(" Û"), _T("÷Ì"), _T("π∑"), _T("º¶")};
	return strShengXiao[m_nValue%12];
}

bool CCode::IsRed() const
{
	static int nRedCode[] = { 1, 2, 7, 8, 12, 13, 18, 19, 23, 24, 29, 30, 34, 35, 40, 45,46 };
	for (int i=0; i<sizeof(nRedCode)/sizeof(int); i++)
	{
		if (m_nValue == nRedCode[i])
		{
			return true;
		}
	}

	return false;
}

bool CCode::IsBlue() const
{
	static int nBlueCode[] = { 3, 4, 9, 10, 14, 15, 20, 25, 26, 31, 36, 37, 41, 42, 47, 48 };
	for (int i=0; i<sizeof(nBlueCode)/sizeof(int); i++)
	{
		if (m_nValue == nBlueCode[i])
		{
			return true;
		}
	}

	return false;
}

bool CCode::IsGreen() const
{
	static int nGreenCode[] = { 5, 6, 11, 16, 17, 21, 22, 27, 28, 32, 33, 38, 39, 43, 44, 49 };
	for (int i=0; i<sizeof(nGreenCode)/sizeof(int); i++)
	{
		if (m_nValue == nGreenCode[i])
		{
			return true;
		}
	}

	return false;
}

bool CCode::IsBig() const
{
	return m_nValue>=25;
}

bool CCode::IsSmall() const
{
	return m_nValue<25;
}

bool CCode::IsDouble() const
{
	return (m_nValue%2)==0;
}

bool CCode::IsSingle() const
{
	return (m_nValue%2)!=0;
}

bool CCode::IsSumSingle() const
{
	return !(IsSumDouble());
}

bool CCode::IsSumDouble() const
{
	int nSum = m_nValue/10 + m_nValue%10;
	return nSum%2 ? false:true;
}

bool CCode::IsSumBig() const
{
	int nSum = m_nValue/10 + m_nValue%10;
	return nSum>=7;
}

bool CCode::IsSumSmall() const
{
	int nSum = m_nValue/10 + m_nValue%10;
	return nSum<7;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  COrder
//

COrder::COrder(void)
{
	for (int i=1; i<=CODE_NUMBER; i++)
	{
		m_code[i].SetCode(i);
	}
}

COrder::~COrder(void)
{
}

__int64 COrder::GetTotalBuyMoney() const
{
	__int64 nSum = 0;
	for (int i=1; i<=CODE_NUMBER; i++)
	{
		nSum += m_code[i].Money();
	}

	return nSum;
}

void COrder::Buy(int nIndex, __int64 nMoney)
{
	if (nIndex <= 0 || nIndex > COrder::CODE_NUMBER)
	{
		return;
	}

	m_code[nIndex].Buy(nMoney);
}

__int64 COrder::Money(int nIndex) const
{
	if (nIndex <= 0 || nIndex > COrder::CODE_NUMBER)
	{
		return 0;
	}

	return m_code[nIndex].Money();
}