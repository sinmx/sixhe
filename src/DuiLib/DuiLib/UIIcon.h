
//
//������CIconUI  ��ʾICO��
//���ܣ���ʾICO�ļ�
//
#pragma once

namespace DuiLib {
class UILIB_API CIconUI :	public CButtonUI
{
public:
	CIconUI(void);
	~CIconUI();
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	// ����icoͼƬ
	virtual void PaintStatusImage(HDC hDC);	

	// ����icoͼƬ
	void SetIconFile(LPCTSTR pstrValue);
	HICON m_hIcon;			// icon���
	RECT m_icoPos;			// icon �ߴ�
};

}  // namespace DuiLib
