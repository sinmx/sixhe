
////////////////////////////////////////////////////////////////////
//������CUIWndContainer ����������
//���ܣ����߼�������λ���ϴ���һ���Ӵ��ڣ��Ӵ��ڵ�λ�á��Ƿ���ʾ�������������߼�����������
/////////////////////////////////////////////////////////////////////////

#pragma once

namespace DuiLib 
{
	class CUIDialogFrameWnd;
	class IUIWndContainerCallback
	{
	public:
		virtual CUIDialogFrameWnd* NewWndObject(LPCTSTR pstrWndClass) = 0;
	};

	class UILIB_API CUIWndContainer : public CContainerUI
	{
	public:
		CUIWndContainer(IUIWndContainerCallback* pCallback);
		virtual ~CUIWndContainer(void);

	public:		//�������Ե�����
		void SetPos(RECT rc);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetXMLFile(LPCTSTR pstrValue);
		void SetWndClass(LPCTSTR pstrValue);
		CUIDialogFrameWnd* GetPhysicalWndObjectPtr() {return m_pWnd;}

	protected:
		IUIWndContainerCallback* m_pCallback;	//�ص�
		CUIDialogFrameWnd* m_pWnd;			//�����Ĵ��ڶ���
		CStdString m_strXMLFile;			//�����Ĵ��ڼ��ص�xml�ļ�
		CStdString m_strWndClassName;		//����������
	};
}  // namespace DuiLib