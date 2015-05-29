#include "stdafx.h"
#include "CharSetConversionUtil.h"

char* CCharSetConversionUtil::WideCharToMultiByteEx(const unsigned char* pUnicodeBuf, const int nBufLen)
{
	int nNeedLen=WideCharToMultiByte(CP_ACP,0,reinterpret_cast<LPCWSTR>(pUnicodeBuf),nBufLen/sizeof(WCHAR),NULL,0,NULL,NULL);
	if (nNeedLen == 0)
	{
		return NULL;
	}

	char* lpszBuf=new char[nNeedLen+1];
	if (lpszBuf == 0)
	{
		return NULL;
	}

	memset(lpszBuf, 0, nNeedLen+1);
	if (WideCharToMultiByte(CP_ACP,0,reinterpret_cast<LPCWSTR>(pUnicodeBuf),nBufLen/sizeof(WCHAR),lpszBuf,nNeedLen,NULL,NULL)!=nNeedLen) //error
	{
		delete []lpszBuf;
		return NULL;
	}
	else //sucess
	{
		return lpszBuf;
	}
}

wchar_t* CCharSetConversionUtil::MultiByteToWideCharEx(const char* pMBBuf, const int nBufLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, pMBBuf, nBufLen, NULL, 0);
	if (nSize == 0)
	{
		return NULL;
	}

	wchar_t* pBuffer = new wchar_t[nSize+1];
	if (pBuffer == 0)
	{
		return NULL;
	}
	memset(pBuffer, 0 , sizeof(wchar_t)*(nSize+1));
	if (MultiByteToWideChar(CP_ACP, 0, pMBBuf, nBufLen, pBuffer, nSize)!=nSize)
	{
		delete []pBuffer;
		return NULL;
	}

	return pBuffer;
}

char* CCharSetConversionUtil::Utf8ToMultiByte(const char* pUtf8Buf, const int nBufLen)
{
	//convert utf8 to unicode
	int nSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUtf8Buf, nBufLen, NULL, 0);
	if (nSize == 0)
	{
		return NULL;
	}
	wchar_t* pWideChar = new wchar_t[nSize];
	if (pWideChar == 0)
	{
		return NULL;
	}
	if (MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUtf8Buf, nBufLen, pWideChar, nSize)!=nSize)
	{
		delete []pWideChar;
		return NULL;
	}

	//convert unicode to multibyte
	char* pMBBuf = WideCharToMultiByteEx((unsigned char*)pWideChar, sizeof(wchar_t)*nSize);
	delete [] pWideChar;
	return pMBBuf;
}

char* CCharSetConversionUtil::MultiByteToUtf8(const char* pMBBuf, const int nBufLen)
{
	wchar_t* pBuffer = MultiByteToWideCharEx(pMBBuf, nBufLen);
	if (pBuffer == 0)
	{
		return NULL;
	}

	int nNeedLen = WideCharToMultiByte(CP_UTF8, 0, pBuffer, -1, NULL, 0, NULL, NULL);
	if (nNeedLen == 0)
	{
		delete []pBuffer;
		return NULL;
	}
	char* pUtf8Buf = new char[nNeedLen];
	if (pUtf8Buf == 0)
	{
		delete []pBuffer;
		return NULL;
	}
	memset(pUtf8Buf, 0, nNeedLen);
	if (WideCharToMultiByte(CP_UTF8, 0, pBuffer, -1, pUtf8Buf, nNeedLen, NULL, NULL)!=nNeedLen)
	{
		delete []pBuffer;
		delete []pUtf8Buf;
		return NULL;
	}

	delete []pBuffer;
	return pUtf8Buf;
}

char* CCharSetConversionUtil::WideCharToUtf8(const unsigned char* pUnicodeBuf, const int nBufLen)
{
	int nNeedLen=WideCharToMultiByte(CP_UTF8,0,reinterpret_cast<LPCWSTR>(pUnicodeBuf),nBufLen/sizeof(WCHAR),NULL,0,NULL,NULL);
	if (nNeedLen == 0)
	{
		return NULL;
	}

	char* lpszBuf=new char[nNeedLen+1];
	if (lpszBuf == 0)
	{
		return NULL;
	}

	memset(lpszBuf, 0, nNeedLen+1);
	if (WideCharToMultiByte(CP_UTF8,0,reinterpret_cast<LPCWSTR>(pUnicodeBuf),nBufLen/sizeof(WCHAR),lpszBuf,nNeedLen,NULL,NULL)!=nNeedLen) //error
	{
		delete []lpszBuf;
		return NULL;
	}
	else //sucess
	{
		return lpszBuf;
	}
}

wchar_t* CCharSetConversionUtil::Utf8ToWideChar(const char* pUtf8Buffer, const int nBufLen)
{
	int nSize = MultiByteToWideChar(CP_UTF8, 0, pUtf8Buffer, nBufLen, NULL, 0);
	if (nSize == 0)
	{
		return NULL;
	}

	wchar_t* pBuffer = new wchar_t[nSize+1];
	if (pBuffer == 0)
	{
		return NULL;
	}
	memset(pBuffer, 0 , sizeof(wchar_t)*(nSize+1));
	if (MultiByteToWideChar(CP_UTF8, 0, pUtf8Buffer, nBufLen, pBuffer, nSize)!=nSize)
	{
		delete []pBuffer;
		return NULL;
	}

	return pBuffer;
}