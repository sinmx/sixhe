#include "StdAfx.h"
#include "CustomerDlg.h"
#include "CommunicationDefine.h"
#include "CustomerManager.h"
#include "OrderManager.h"
#include "LicenseManager.h"
#include "DuiMessageBox.h"

CCustomerDlg::CCustomerDlg(void) :
m_pCustomerContainer(NULL),
m_pCustomerList(NULL),
m_pCustomerAddBtn(NULL)
{
}

CCustomerDlg::~CCustomerDlg(void)
{
}

int CCustomerDlg::GetID()
{
	return COMID_CustomerDlg;
}

int CCustomerDlg::AcceptEvent(int nEventCode, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CCustomerDlg::OnInitDialog()
{
	CUIDialogFrameWnd::OnInitDialog(); //call base

	//��ӵ�ͨ�Ź�������
	CCommunicationManager::Instance()->Attach(this);

	//�������ƻ�ȡ�ؼ������ַ
	GetLogicControlObject();

	//��ʼ���߼��ؼ�����
	InitLogicControlObject();
}

void CCustomerDlg::OnFinalMessage(HWND hWnd)
{
	//��ͨ�Ź�������ȥ��
	CCommunicationManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CCustomerDlg::GetLogicControlObject()
{
	//
	//ͨ�����Ʋ��ҿؼ����󣬲���dynamic_cast���а�ȫת��	
	//	
	m_pCustomerContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("customerContainer")));
	ASSERT(m_pCustomerContainer);
	m_pCustomerList = dynamic_cast<DuiLib::CListUI*>(m_pm.FindControl(_T("customerList")));
	ASSERT(m_pCustomerList);
	m_pCustomerAddBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindSubControlByName(m_pCustomerList, _T("addBtn")));
	ASSERT(m_pCustomerAddBtn);
}

void CCustomerDlg::InitLogicControlObject()
{
	//��ʼ���û��б�
	InitCustomerList();
}

void CCustomerDlg::InitCustomerList()
{
	m_pCustomerList->RemoveAll();
	vector<CUSTOMER> customerVec = CCustomerManager::Instance()->GetCustomerList();
	for (vector<CUSTOMER>::const_iterator it=customerVec.begin(); it!=customerVec.end(); it++)
	{
		AddOneCustomer(*it, false);
	}	
}

void CCustomerDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ������������ťʶ��ͳһ�����ƱȽ�
	//

	if (pSender == m_pCustomerAddBtn)  //����û�
	{
		OnCustomerAddBtn();
	}
	else if (pSender->GetName() == _T("deleteBtn"))  //ɾ���û�
	{
		OnCustomerDeleteBtn(pSender);
	}
	else if (pSender->GetName() == _T("orderBtn"))  //�ӵ�
	{
		OnOrderBtn();
	}
	else
	{
		CUIDialogFrameWnd::OnClick(pSender);
	}
}

void CCustomerDlg::OnOrderBtn()
{
	CCommunicationManager::Instance()->Notify(COMID_MainDlg, COMEVENT_ShowOrderPage);
}

LRESULT CCustomerDlg::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//��������ʱҪ�ÿؼ�ʧȥ���㣬�����û���Ϣ�޷���������
	m_pm.SetFocus(NULL);
	return 0;
}

void CCustomerDlg::OnKillFocus(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ����������
	//

	if (pSender->GetName()==_T("nameEdit") || pSender->GetName()==_T("discountEdit") || pSender->GetName()==_T("bettingOddsEdit")) //�û����ñ��
	{
		OnCustomerItemKillFocus(pSender);
	}
	else  //call base
	{
		CUIDialogFrameWnd::OnKillFocus(pSender);
	}
}

void CCustomerDlg::OnCustomerItemKillFocus(DuiLib::CControlUI* pControl)
{
	DuiLib::CListContainerElementUI* pSelItem = dynamic_cast<DuiLib::CListContainerElementUI*>(m_pm.FindFirstParentByClass(pControl, _T("ListContainerElementUI")));
	if (pSelItem == NULL)
	{
		return;
	}

	CUSTOMER newCustomer;
	DuiLib::CControlUI* pNameCtrl = m_pm.FindSubControlByName(pSelItem, _T("nameEdit"));
	newCustomer.strName = pNameCtrl->GetText().GetData();
	if (newCustomer.strName.IsEmpty())
	{
		pNameCtrl->SetText(pNameCtrl->GetUserData());
		ShowTip(CLanguageManager::Instance()->LoadString(39), 3000, pNameCtrl);  //39=��ׯ�û�������Ϊ��
		return;
	}

	DuiLib::CControlUI* pDiscountCtrl = m_pm.FindSubControlByName(pSelItem, _T("discountEdit"));
	if (pDiscountCtrl->GetText().IsEmpty())
	{
		pDiscountCtrl->SetText(pDiscountCtrl->GetUserData());
		ShowTip(CLanguageManager::Instance()->LoadString(40), 3000, pDiscountCtrl);  //40=�����ˮ����Ϊ��
		return;
	}
	newCustomer.nDiscount = _ttoi(pDiscountCtrl->GetText());

	DuiLib::CControlUI* pBettingOddsCtrl = m_pm.FindSubControlByName(pSelItem, _T("bettingOddsEdit"));
	if (pBettingOddsCtrl->GetText().IsEmpty())
	{
		pBettingOddsCtrl->SetText(pBettingOddsCtrl->GetUserData());
		ShowTip(CLanguageManager::Instance()->LoadString(41), 3000, pBettingOddsCtrl);  //41=�������ʲ���Ϊ��
		return;
	}
	newCustomer.nBettingOdds = _ttoi(pBettingOddsCtrl->GetText());

	//����û�����账��
	if (pNameCtrl->GetText() == pNameCtrl->GetUserData() && pDiscountCtrl->GetText() == pDiscountCtrl->GetUserData() && pBettingOddsCtrl->GetText() == pBettingOddsCtrl->GetUserData())
	{
		return;
	}

	//���浽���ݿ���
	if (CCustomerManager::Instance()->UpdateCustomer((LPCTSTR)pNameCtrl->GetUserData(), newCustomer))
	{
		//�����û�������ôԭ���û��ĵ���Ҫɾ����
		if (pNameCtrl->GetText() != pNameCtrl->GetUserData())
		{
			COrderManager::Instance()->DeleteOrder(LPCTSTR(pNameCtrl->GetUserData()));
		}

		pNameCtrl->SetUserData(pNameCtrl->GetText());
		pDiscountCtrl->SetUserData(pDiscountCtrl->GetText());
		pBettingOddsCtrl->SetUserData(pBettingOddsCtrl->GetText());
		ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, pSelItem);  //37=����ɹ�
	}
	else //����ʧ�ܣ��ָ���ȥ
	{
		pNameCtrl->SetText(pNameCtrl->GetUserData());
		pDiscountCtrl->SetText(pDiscountCtrl->GetUserData());
		pBettingOddsCtrl->SetText(pBettingOddsCtrl->GetUserData());
		return;
	}
}

void CCustomerDlg::OnCustomerDeleteBtn(DuiLib::CControlUI* pDelBtn)
{
	DuiLib::CListContainerElementUI* pSelItem = dynamic_cast<DuiLib::CListContainerElementUI*>(m_pm.FindFirstParentByClass(pDelBtn, _T("ListContainerElementUI")));
	if (pSelItem == NULL)
	{
		return;
	}	

	DuiLib::CControlUI* pNameCtrl = m_pm.FindSubControlByName(pSelItem, _T("nameEdit"));
	if (CCustomerManager::Instance()->DeleteCustomer(pNameCtrl->GetText().GetData()))
	{
		//�Ѹ��û��ĵ���Ҳɾ����
		COrderManager::Instance()->DeleteOrder(pNameCtrl->GetText().GetData());

		//���û��ؼ�Ҳɾ����
		m_pCustomerContainer->SetFixedHeight(m_pCustomerContainer->GetFixedHeight()-pSelItem->GetFixedHeight());
		m_pCustomerList->Remove(pSelItem);			
	}
}

void CCustomerDlg::AddOneCustomer(const CUSTOMER& customer, bool bNeedFocus)
{
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pControl = builder.Create(_T("item_customer.xml"), NULL, NULL, &m_pm);
	if (pControl == NULL)
	{
		return;
	}

	m_pCustomerList->AddAt(pControl, 0);
	m_pCustomerContainer->SetFixedHeight(m_pCustomerContainer->GetFixedHeight()+pControl->GetFixedHeight());

	DuiLib::CControlUI* pNameCtrl = m_pm.FindSubControlByName(pControl, _T("nameEdit"));
	pNameCtrl->SetText(customer.strName);
	pNameCtrl->SetUserData(customer.strName);
	if (bNeedFocus)
	{
		pNameCtrl->SetFocus();
	}	
	DuiLib::CControlUI* pDiscountCtrl = m_pm.FindSubControlByName(pControl, _T("discountEdit"));
	pDiscountCtrl->SetText(CStringUtil::IntToStr(customer.nDiscount));
	pDiscountCtrl->SetUserData(CStringUtil::IntToStr(customer.nDiscount));
	DuiLib::CControlUI* pBettingOddsCtrl = m_pm.FindSubControlByName(pControl, _T("bettingOddsEdit"));
	pBettingOddsCtrl->SetText(CStringUtil::IntToStr(customer.nBettingOdds));
	pBettingOddsCtrl->SetUserData(CStringUtil::IntToStr(customer.nBettingOdds));	
}

void CCustomerDlg::OnCustomerAddBtn()
{
	//����Ѿ��ﵽ����û�����ʾ��
	if (CCustomerManager::Instance()->GetCustomerCount() >= CLicenseManager::Instance()->GetUserMaxCount())
	{
		//33=��ǰΪ���ð汾��ֻ�����%d����ׯ��
		CString strInfo;
		strInfo.Format(CLanguageManager::Instance()->LoadString(33), CLicenseManager::Instance()->GetUserMaxCount());
		DuiMessageBox(m_hWnd, strInfo, CLanguageManager::Instance()->LoadString(1), MB_OK);
		return;
	}

	CUSTOMER customer;
	if (CCustomerManager::Instance()->AddCustomer(customer, true))
	{
		AddOneCustomer(customer, true);
	}
}

void CCustomerDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ������������ťʶ��ͳһ�����ƱȽ�
	//	

	CUIDialogFrameWnd::OnSelectChange(pSender);	
}