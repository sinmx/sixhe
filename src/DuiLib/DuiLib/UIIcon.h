
//
//类名：CIconUI  显示ICO类
//功能：显示ICO文件
//
#pragma once

namespace DuiLib {
class UILIB_API CIconUI :	public CButtonUI
{
public:
	CIconUI(void);
	~CIconUI();
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	// 绘制ico图片
	virtual void PaintStatusImage(HDC hDC);	

	// 设置ico图片
	void SetIconFile(LPCTSTR pstrValue);
	HICON m_hIcon;			// icon句柄
	RECT m_icoPos;			// icon 尺寸
};

}  // namespace DuiLib
