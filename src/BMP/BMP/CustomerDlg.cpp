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

	//添加到通信管理器中
	CCommunicationManager::Instance()->Attach(this);

	//根据名称获取控件对象地址
	GetLogicControlObject();

	//初始化逻辑控件对象
	InitLogicControlObject();
}

void CCustomerDlg::OnFinalMessage(HWND hWnd)
{
	//从通信管理器中去除
	CCommunicationManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CCustomerDlg::GetLogicControlObject()
{
	//
	//通过名称查找控件对象，采用dynamic_cast进行安全转化	
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
	//初始化用户列表
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
	//这里每个if用一个函数处理，按钮识别统一用名称比较
	//

	if (pSender == m_pCustomerAddBtn)  //添加用户
	{
		OnCustomerAddBtn();
	}
	else if (pSender->GetName() == _T("deleteBtn"))  //删除用户
	{
		OnCustomerDeleteBtn(pSender);
	}
	else if (pSender->GetName() == _T("orderBtn"))  //接单
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
	//窗口隐藏时要让控件失去焦点，否则用户信息无法正常保存
	m_pm.SetFocus(NULL);
	return 0;
}

void CCustomerDlg::OnKillFocus(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//

	if (pSender->GetName()==_T("nameEdit") || pSender->GetName()==_T("discountEdit") || pSender->GetName()==_T("bettingOddsEdit")) //用户配置变更
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
		ShowTip(CLanguageManager::Instance()->LoadString(39), 3000, pNameCtrl);  //39=下庄用户名不能为空
		return;
	}

	DuiLib::CControlUI* pDiscountCtrl = m_pm.FindSubControlByName(pSelItem, _T("discountEdit"));
	if (pDiscountCtrl->GetText().IsEmpty())
	{
		pDiscountCtrl->SetText(pDiscountCtrl->GetUserData());
		ShowTip(CLanguageManager::Instance()->LoadString(40), 3000, pDiscountCtrl);  //40=特码回水不能为空
		return;
	}
	newCustomer.nDiscount = _ttoi(pDiscountCtrl->GetText());

	DuiLib::CControlUI* pBettingOddsCtrl = m_pm.FindSubControlByName(pSelItem, _T("bettingOddsEdit"));
	if (pBettingOddsCtrl->GetText().IsEmpty())
	{
		pBettingOddsCtrl->SetText(pBettingOddsCtrl->GetUserData());
		ShowTip(CLanguageManager::Instance()->LoadString(41), 3000, pBettingOddsCtrl);  //41=特码赔率不能为空
		return;
	}
	newCustomer.nBettingOdds = _ttoi(pBettingOddsCtrl->GetText());

	//配置没变无需处理
	if (pNameCtrl->GetText() == pNameCtrl->GetUserData() && pDiscountCtrl->GetText() == pDiscountCtrl->GetUserData() && pBettingOddsCtrl->GetText() == pBettingOddsCtrl->GetUserData())
	{
		return;
	}

	//保存到数据库中
	if (CCustomerManager::Instance()->UpdateCustomer((LPCTSTR)pNameCtrl->GetUserData(), newCustomer))
	{
		//更新用户名，那么原来用户的单子要删除掉
		if (pNameCtrl->GetText() != pNameCtrl->GetUserData())
		{
			COrderManager::Instance()->DeleteOrder(LPCTSTR(pNameCtrl->GetUserData()));
		}

		pNameCtrl->SetUserData(pNameCtrl->GetText());
		pDiscountCtrl->SetUserData(pDiscountCtrl->GetText());
		pBettingOddsCtrl->SetUserData(pBettingOddsCtrl->GetText());
		ShowTip(CLanguageManager::Instance()->LoadString(37), 1000, pSelItem);  //37=保存成功
	}
	else //保存失败，恢复回去
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
		//把该用户的单子也删除掉
		COrderManager::Instance()->DeleteOrder(pNameCtrl->GetText().GetData());

		//该用户控件也删除掉
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
	//如果已经达到最大用户数提示它
	if (CCustomerManager::Instance()->GetCustomerCount() >= CLicenseManager::Instance()->GetUserMaxCount())
	{
		//33=当前为试用版本，只能添加%d个下庄。
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
	//这里每个if用一个函数处理，按钮识别统一用名称比较
	//	

	CUIDialogFrameWnd::OnSelectChange(pSender);	
}