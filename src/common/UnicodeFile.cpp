#include "StdAfx.h"
#include "UnicodeFile.h"

CUnicodeFile::CUnicodeFile(void)
{
	m_bHasOpen=FALSE;
	m_bUnicodeFile=FALSE;
}

CUnicodeFile::~CUnicodeFile(void)
{
	if( m_bHasOpen)
	{
		Close();
		m_bHasOpen=FALSE;
	}
}

BOOL CUnicodeFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	//打开文件
	if( !CFile::Open(lpszFileName,nOpenFlags))
	{
		return FALSE;
	}	

	//判断是否为UNICODE文件
	if( GetLength()<2)
	{
		m_bUnicodeFile=FALSE;
	}
	else
	{
		BYTE bBOM[2];
		if( Read(bBOM,2)!=2)
		{
			return FALSE;
		}
		else
		{
			if( bBOM[0]==0xFF&&bBOM[1]==0xFE)
			{
				m_bUnicodeFile=TRUE;
			}
		}
		SeekToBegin();
	}

	//标识已经打开
	m_bHasOpen=TRUE;

	return TRUE;
}

void CUnicodeFile::Close()
{
	if( m_bHasOpen)
	{
		CFile::Close();
		m_bHasOpen=FALSE;
	}
}

BOOL CUnicodeFile::ReadString(CString& rString)
{
	if( !m_bHasOpen)
	{
		return FALSE;
	}
	if (GetPosition() >= GetLength())
	{
		return FALSE;
	}
	rString=_T("");	
	if( m_bUnicodeFile)
	{
		ReadStringForUNICODE(rString);
	}
	else
	{
		ReadStringForANSI(rString);
	}

	//去除最后\r\n
	if( rString.GetLength()>=1)
	{
		if( rString[rString.GetLength()-1]==_T('\n'))
		{
			rString=rString.Left(rString.GetLength()-1);			
		}
	}
	if( rString.GetLength()>=1)
	{
		if( rString[rString.GetLength()-1]==_T('\r'))
		{
			rString=rString.Left(rString.GetLength()-1);
		}
	}

	//去除UNICODE的BOM头
	if(m_bUnicodeFile )
	{
		if( rString.GetLength()>=1)
		{
			if( (USHORT)rString[0]==0xfeff)
			{
				rString=rString.Right(rString.GetLength()-1);
			}
		}
	}
	return TRUE;
}

void CUnicodeFile::WriteString(LPCTSTR lpsz)
{
	if( !m_bHasOpen)
	{
		return;
	}

	//内容为空
	if( _tcslen(lpsz)==0)
	{
		return;
	}

	if( m_bUnicodeFile)
	{
		if( GetPosition()==0) //开头位置
		{
			//写入UNICODE BOM标识
			USHORT unicodeFlag=0xfeff;
			CFile::Write(&unicodeFlag,sizeof(USHORT));
		}
		CFile::Write(lpsz,(UINT)_tcslen(lpsz)*sizeof(TCHAR));
	}
	else
	{
		//转化为ANSI编码
		int nNeedLen=WideCharToMultiByte(CP_ACP,0,lpsz,(UINT)_tcslen(lpsz),NULL,0,NULL,NULL);
		if( nNeedLen!=0)
		{
			char* lpszBuf=new char[nNeedLen];			
			if( lpszBuf!=NULL)
			{
				memset(lpszBuf,0,nNeedLen);
				if( WideCharToMultiByte(CP_ACP,0,(LPCWSTR)lpsz,(UINT)_tcslen(lpsz),lpszBuf,nNeedLen,NULL,NULL))
				{
					CFile::Write(lpszBuf,nNeedLen);
				}
				delete []lpszBuf;
				lpszBuf=NULL;
			}
		}
	}
}

BOOL CUnicodeFile::ReadStringForANSI(CString& rString)
{	
	rString=_T("");

	int nReadCount=0;  //读取字节数
	const int nAllocateSize=1024;
	int nBufLen=nAllocateSize;	//缓冲区长度
	BYTE* pBuf=new BYTE[nBufLen];
	if( pBuf==NULL)
	{
		return FALSE;
	}

	while(TRUE)
	{	
		if( nReadCount>=nBufLen)
		{
			//重新分配内存
			nBufLen=nReadCount+nAllocateSize;
			BYTE* pBufTemp=new BYTE[nBufLen];
			if( pBufTemp==NULL)
			{
				delete[] pBuf;
				pBuf=NULL;
				return FALSE;
			}
			memcpy_s(pBufTemp,nBufLen,pBuf,nReadCount);
			delete[] pBuf;
			pBuf=pBufTemp;			
		}

		//一个一个字节读取
		if( CFile::Read(&pBuf[nReadCount],1)==0)  //读取到文件的末尾
		{
			break;
		}
		else
		{
			if( pBuf[nReadCount]==0x0a)  //换行
			{
				break;
			}
		}

		nReadCount++;
	}

	//已经达到文件末尾
	if( nReadCount<=0)
	{
		delete[] pBuf;
		pBuf=NULL;
		return FALSE;
	}

	//将ANSI转化为UNICODE编码
	int nNeedLen=MultiByteToWideChar(CP_ACP,0,(LPCSTR)pBuf,nReadCount,NULL,0);
	if( nNeedLen!=0)
	{
		WCHAR* lpszBuf=new WCHAR[nNeedLen+1];		
		if( lpszBuf!=NULL)
		{
			memset(lpszBuf,0,(nNeedLen+1)*sizeof(WCHAR));
			if(MultiByteToWideChar(CP_ACP,0,(LPCSTR)pBuf,nReadCount,lpszBuf,nNeedLen))
			{
				rString=lpszBuf;
			}
			delete []lpszBuf;
			lpszBuf=NULL;
		}
	}

	if( pBuf!=NULL)
	{
		delete[] pBuf;
		pBuf=NULL;
	}

	if( rString.IsEmpty())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CUnicodeFile::ReadStringForUNICODE(CString& rString)
{
	rString=_T("");

	int nReadCount=0;  //读取字节数
	const int nAllocateSize=1024;
	int nBufLen=nAllocateSize;	//缓冲区长度
	BYTE* pBuf=new BYTE[nBufLen];
	if( pBuf==NULL)
	{
		return FALSE;
	}

	while(TRUE)
	{	
		if( nReadCount>=nBufLen)
		{
			//重新分配内存
			nBufLen=nReadCount+nAllocateSize;
			BYTE* pBufTemp=new BYTE[nBufLen];
			if( pBufTemp==NULL)
			{
				delete[] pBuf;
				pBuf=NULL;
				return FALSE;
			}
			memcpy_s(pBufTemp,nBufLen,pBuf,nReadCount);
			delete[] pBuf;
			pBuf=pBufTemp;			
		}

		//每次两个字节读取
		if( CFile::Read(&pBuf[nReadCount],2)<2)  //读取到文件的末尾
		{
			break;
		}
		else
		{
			if( pBuf[nReadCount]==0x0a && pBuf[nReadCount+1]==0x00)  //换行
			{
				break;
			}
		}

		nReadCount+=2;
	}

	//已经达到文件末尾
	if( nReadCount<2)
	{
		delete[] pBuf;
		pBuf=NULL;
		return FALSE;
	}

	//拷贝到字符串
	BYTE* pStrBuf=new BYTE[nReadCount+2];
	if( pStrBuf==NULL)
	{
		delete[] pBuf;
		pBuf=NULL;
		return FALSE;
	}
	ZeroMemory(pStrBuf,nReadCount+2);
	memcpy_s(pStrBuf,nReadCount+2,pBuf,nReadCount);
	rString=(wchar_t*)pStrBuf;

	if( pBuf!=NULL)
	{
		delete[] pBuf;
		pBuf=NULL;
	}
	if( pStrBuf!=NULL)
	{
		delete[] pStrBuf;
		pStrBuf=NULL;
	}	

	return TRUE;
}