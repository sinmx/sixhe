#include "StdAfx.h"
#include "StringUtil.h"

CStringUtil::CStringUtil(LPCTSTR szInput)
{
	m_strInput = szInput;
}

vector<CString> CStringUtil::Split(TCHAR chSeperate) const
{
	vector<CString> strArray;
	if( m_strInput.IsEmpty())
	{
		return strArray;
	}

	int nStart=0, nEnd;
	while( true)
	{		
		while (nStart < m_strInput.GetLength() && m_strInput[nStart] == chSeperate)
		{
			nStart++;
		}

		if (nStart >= m_strInput.GetLength())
		{
			break;
		}

		nEnd = nStart + 1;
		while (nEnd < m_strInput.GetLength() && m_strInput[nEnd] != chSeperate)
		{
			nEnd++;
		}

		if (nEnd >= m_strInput.GetLength())
		{
			strArray.push_back(m_strInput.Mid(nStart));
			break;
		}
		else
		{
			strArray.push_back(m_strInput.Mid(nStart, nEnd-nStart));
			nStart = nEnd+1;
		}
	}

	return strArray;
}

bool CStringUtil::IsAllNumber() const
{
	bool bHasUnNumber = false;
	for (int i=0; i<m_strInput.GetLength(); i++)
	{
		if (!(m_strInput[i]>=_T('0') && m_strInput[i]<=_T('9')))
		{
			bHasUnNumber = true;
			break;
		}
	}

	return !bHasUnNumber;
}

bool CStringUtil::IsNumber() const
{
	if (m_strInput.IsEmpty())
	{
		return false;
	}

	bool bNumber = true;
	for (int i=0; i<m_strInput.GetLength(); i++)
	{
		if (!(m_strInput[i]>=_T('0') && m_strInput[i]<=_T('9'))) //不是数字，判断是不是负号
		{
			if (i == 0 && m_strInput[i] == '-')
			{				
				continue;
			}
			else
			{
				bNumber = false;
				break;
			}
		}
	}

	return bNumber;
}

CString CStringUtil::IntToStr(int num)
{
	CString strTmp;
	strTmp.Format(_T("%d"), num);
	return strTmp;
}

CString CStringUtil::Int64ToStr(__int64 num)
{
	CString strTmp;
	strTmp.Format(_T("%I64d"), num);
	return strTmp;
}

int CStringUtil::CharToInt(TCHAR ch)
{
	if (ch >= _T('0') && ch <= '9')
	{
		return int(ch-_T('0'));
	}
	else
	{
		return -1;
	}
}