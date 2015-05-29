#include "StdAfx.h"

DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{ 
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;

extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);

#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
//////////////////////////////////////////////////////////////////////////

namespace DuiLib {

CIconUI::CIconUI(void)
{
	m_hIcon = NULL;
	ZeroMemory(&m_icoPos,sizeof(RECT));
}
CIconUI::~CIconUI()
{
	if (m_hIcon!=NULL)
	{
		::DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}
}

void CIconUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("icofile")) == 0 ) {
		SetIconFile(pstrValue);			
	}
	else if( _tcscmp(pstrName, _T("icopos")) == 0 ) {	
		LPTSTR pstr = NULL;
		m_icoPos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		m_icoPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		m_icoPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		m_icoPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);   		
	}
	else {
		CButtonUI::SetAttribute(pstrName,pstrValue);
	}
}

void CIconUI::SetIconFile(LPCTSTR pstrValue)
{
	CStdString sFile = pstrValue;
	if (sFile.IsEmpty()) return;

	if (m_hIcon!=NULL)  //先释放原来的图标资源
	{
		::DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}

	bool bFullPath = (sFile.Find(':') != -1)?true:false;	// 判断是否为全路径
	if ( bFullPath ){										//如果是，不论是否指定zip，都按全路径处理
		m_hIcon = (HICON)LoadImage(NULL, sFile.GetData(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}
	else if (CPaintManagerUI::GetResourceZip().IsEmpty()){
		sFile = CPaintManagerUI::GetResourcePath() + sFile;
		m_hIcon = (HICON)LoadImage(NULL, sFile.GetData(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}
	else{
		sFile = CPaintManagerUI::GetResourcePath() + CPaintManagerUI::GetResourceZip();
		HZIP hz = NULL;
		if( CPaintManagerUI::IsCachedResourceZip() ) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
		else hz = OpenZip((void*)sFile.GetData(), 0, 2);
		if( hz == NULL ) return ;
		int i; 
		ZIPENTRY ze;
		if( FindZipItem(hz, pstrValue, true, &i, &ze) != 0 ) return ;
		TCHAR szFile[MAX_PATH] = {0};
		DWORD dwSize = GetTempPath(MAX_PATH, szFile); 
		if (dwSize == 0) return;
		CStdString snFile = szFile;
		snFile += pstrValue;
		int res = UnzipItem(hz, i, (void*)snFile.GetData(), snFile.GetLength(), 2);
		if( res != 0x00000000 && res != 0x00000600) {
			if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
			return ;
		}
		if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);

		m_hIcon = (HICON)LoadImage(NULL, snFile.GetData(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}
}

// 绘制状态图片
void CIconUI::PaintStatusImage(HDC hDC)
{		
	if (m_hIcon != NULL)
	{
		::DrawIconEx(hDC,
			m_rcItem.left + m_icoPos.left,
			m_rcItem.top + m_icoPos.top,
			m_hIcon,
			m_icoPos.right - m_icoPos.left,
			m_icoPos.bottom - m_icoPos.top,
			0,
			NULL,
			DI_NORMAL);
	}
}

}// namespace DuiLib