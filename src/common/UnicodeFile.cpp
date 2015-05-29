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
	//���ļ�
	if( !CFile::Open(lpszFileName,nOpenFlags))
	{
		return FALSE;
	}	

	//�ж��Ƿ�ΪUNICODE�ļ�
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

	//��ʶ�Ѿ���
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

	//ȥ�����\r\n
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

	//ȥ��UNICODE��BOMͷ
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

	//����Ϊ��
	if( _tcslen(lpsz)==0)
	{
		return;
	}

	if( m_bUnicodeFile)
	{
		if( GetPosition()==0) //��ͷλ��
		{
			//д��UNICODE BOM��ʶ
			USHORT unicodeFlag=0xfeff;
			CFile::Write(&unicodeFlag,sizeof(USHORT));
		}
		CFile::Write(lpsz,(UINT)_tcslen(lpsz)*sizeof(TCHAR));
	}
	else
	{
		//ת��ΪANSI����
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

	int nReadCount=0;  //��ȡ�ֽ���
	const int nAllocateSize=1024;
	int nBufLen=nAllocateSize;	//����������
	BYTE* pBuf=new BYTE[nBufLen];
	if( pBuf==NULL)
	{
		return FALSE;
	}

	while(TRUE)
	{	
		if( nReadCount>=nBufLen)
		{
			//���·����ڴ�
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

		//һ��һ���ֽڶ�ȡ
		if( CFile::Read(&pBuf[nReadCount],1)==0)  //��ȡ���ļ���ĩβ
		{
			break;
		}
		else
		{
			if( pBuf[nReadCount]==0x0a)  //����
			{
				break;
			}
		}

		nReadCount++;
	}

	//�Ѿ��ﵽ�ļ�ĩβ
	if( nReadCount<=0)
	{
		delete[] pBuf;
		pBuf=NULL;
		return FALSE;
	}

	//��ANSIת��ΪUNICODE����
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

	int nReadCount=0;  //��ȡ�ֽ���
	const int nAllocateSize=1024;
	int nBufLen=nAllocateSize;	//����������
	BYTE* pBuf=new BYTE[nBufLen];
	if( pBuf==NULL)
	{
		return FALSE;
	}

	while(TRUE)
	{	
		if( nReadCount>=nBufLen)
		{
			//���·����ڴ�
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

		//ÿ�������ֽڶ�ȡ
		if( CFile::Read(&pBuf[nReadCount],2)<2)  //��ȡ���ļ���ĩβ
		{
			break;
		}
		else
		{
			if( pBuf[nReadCount]==0x0a && pBuf[nReadCount+1]==0x00)  //����
			{
				break;
			}
		}

		nReadCount+=2;
	}

	//�Ѿ��ﵽ�ļ�ĩβ
	if( nReadCount<2)
	{
		delete[] pBuf;
		pBuf=NULL;
		return FALSE;
	}

	//�������ַ���
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