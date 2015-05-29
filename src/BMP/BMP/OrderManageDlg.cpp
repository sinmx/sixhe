#include "StdAfx.h"
#include "OrderManageDlg.h"
#include "CommunicationDefine.h"
#include "CodeGroup.h"
#include "CustomerManager.h"
#include "OrderManager.h"
#include "OperatorManager.h"
#include "BuyMethod.h"
#include "ConfManager.h"
#include "DuiMessageBox.h"
#include "SoundManager.h"
#include "resource.h"

//自定义消息
#define UM_ACCEPT_EVENT			WM_USER+100			//接收事件, wParam为事件code, lParam为_tagEventParam指针


COrderManageDlg::COrderManageDlg(void) :
m_pOrderContainer(NULL),
m_pCustomerCombo(NULL),
m_pAddCustomerBtn(NULL),

m_pDeleteOrderBtn(NULL),
m_pDeleteAllOrderBtn(NULL),

m_pCodeContainer(NULL),
m_pLastCodeContainer(NULL),

m_pTotalInfoLabel(NULL),

m_pBuyLogRichedit(NULL),

m_pSingleBuyEdit(NULL),
m_pSingleBuyWrongLabel(NULL),

m_pIntervalStartEdit(NULL),
m_pIntervalEndEdit(NULL),
m_pIntervalBuyEdit(NULL),

m_pEqualCodeEdit(NULL),
m_pEqualBuyWrongLabel(NULL),
m_pEqualBuyEdit(NULL),

m_pStartCodeEdit(NULL),
m_pSequenceBuyEdit(NULL),

m_pCodeGroupContainer(NULL),
m_pLastCodeGroupContainer(NULL),
m_pRecentContainer(NULL),
m_pRecentGroupContainer(NULL),
m_pGroupBuyEdit(NULL),

m_pNoCustomerContainer(NULL),
m_pAddCustomerBtnWhenNoCustomer(NULL)
{
}

COrderManageDlg::~COrderManageDlg(void)
{
}

int COrderManageDlg::GetID()
{
	return COMID_OrderManageDlg;
}

int COrderManageDlg::AcceptEvent(int nEventCode, WPARAM wParam, LPARAM lParam)
{
	return SendEvent(nEventCode, wParam, lParam);	
}

void COrderManageDlg::OnInitDialog()
{
	CUIDialogFrameWnd::OnInitDialog(); //call base

	//添加到通信管理器中
	CCommunicationManager::Instance()->Attach(this);

	//监听下庄用户的变化
	CCustomerManager::Instance()->Attach(this);

	//监听单子数据的变化
	COrderManager::Instance()->Attach(this);

	//根据名称获取控件对象地址
	GetLogicControlObject();

	//初始化逻辑控件对象
	InitLogicControlObject();		
}

void COrderManageDlg::OnFinalMessage(HWND hWnd)
{
	//从通信管理器中去除
	CCommunicationManager::Instance()->Detach(this);

	//不再监听下庄用户变化
	CCustomerManager::Instance()->Detach(this);

	//从单子管理器监听列表中去除
	COrderManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void COrderManageDlg::GetLogicControlObject()
{
	//
	//通过名称查找控件对象，采用dynamic_cast进行安全转化	
	//

	m_pOrderContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("orderContainer")));
	ASSERT(m_pOrderContainer);
	m_pCustomerCombo = dynamic_cast<DuiLib::CComboUI*>(m_pm.FindControl(_T("customerCombo")));
	ASSERT(m_pCustomerCombo);
	m_pAddCustomerBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindSubControlByName(m_pOrderContainer, _T("addCustomerBtn")));
	ASSERT(m_pAddCustomerBtn);
	m_pDeleteOrderBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("deleteOrderBtn")));
	ASSERT(m_pDeleteOrderBtn);
	m_pDeleteAllOrderBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("deleteAllOrderBtn")));
	ASSERT(m_pDeleteAllOrderBtn);	
	m_pCodeContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("codeContainer")));
	ASSERT(m_pCodeContainer);
	m_pLastCodeContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("lastCodeContainer")));
	ASSERT(m_pLastCodeContainer);
	
	m_pTotalInfoLabel = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("totalInfoLabel")));
	ASSERT(m_pTotalInfoLabel);

	m_pBuyLogRichedit = dynamic_cast<DuiLib::CRichEditUI*>(m_pm.FindControl(_T("buyLogRichEdit")));
	ASSERT(m_pBuyLogRichedit);

	m_pSingleBuyEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("singleBuyEdit")));
	ASSERT(m_pSingleBuyEdit);	
	m_pSingleBuyWrongLabel = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("singleBuyWrongLabel")));
	ASSERT(m_pSingleBuyWrongLabel);	

	m_pIntervalStartEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("intervalStartEdit")));
	ASSERT(m_pIntervalStartEdit);
	m_pIntervalEndEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("intervalEndEdit")));
	ASSERT(m_pIntervalEndEdit);
	m_pIntervalBuyEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("intervalBuyEdit")));
	ASSERT(m_pIntervalBuyEdit);

	m_pEqualCodeEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("equalCodeEdit")));
	ASSERT(m_pEqualCodeEdit);
	m_pEqualBuyWrongLabel = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("equalBuyWrongLabel")));
	ASSERT(m_pEqualBuyWrongLabel);
	m_pEqualBuyEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("equalBuyEdit")));
	ASSERT(m_pEqualBuyEdit);

	m_pStartCodeEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("startCodeEdit")));
	ASSERT(m_pStartCodeEdit);
	m_pSequenceBuyEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("sequenceBuyEdit")));
	ASSERT(m_pSequenceBuyEdit);

	m_pCodeGroupContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("codeGroupContainer")));
	ASSERT(m_pCodeGroupContainer);
	m_pLastCodeGroupContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("lastGroupContainer")));
	ASSERT(m_pLastCodeGroupContainer);
	m_pRecentContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("recentContainer")));
	ASSERT(m_pRecentContainer);
	m_pRecentGroupContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("recentGroupContainer")));
	ASSERT(m_pRecentGroupContainer);
	m_pGroupBuyEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("groupBuyEdit")));
	ASSERT(m_pGroupBuyEdit);

	m_pNoCustomerContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("noCustomerContainer")));
	ASSERT(m_pNoCustomerContainer); 
	m_pAddCustomerBtnWhenNoCustomer = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindSubControlByName(m_pNoCustomerContainer, _T("addCustomer")));
	ASSERT(m_pAddCustomerBtnWhenNoCustomer); 
}

void COrderManageDlg::InitCustomer()
{
	m_pCustomerCombo->RemoveAll();
	vector<CUSTOMER> customerVec = CCustomerManager::Instance()->GetCustomerList();
	for (vector<CUSTOMER>::const_iterator it=customerVec.begin(); it!=customerVec.end(); it++)
	{
		DuiLib::CDialogBuilder builder;
		DuiLib::CControlUI* pControl = builder.Create(_T("item_list_label_element.xml"));
		if (pControl == NULL)
		{
			continue;
		}

		pControl->SetText(it->strName);
		m_pCustomerCombo->Add(pControl);
	}

	m_pCustomerCombo->SelectItem(0);
	m_pNoCustomerContainer->SetVisible(customerVec.size()<=0);
}

DuiLib::CControlUI* COrderManageDlg::AddCode(int nCode)
{
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pControl = builder.Create(_T("item_code.xml"), NULL, NULL, &m_pm);
	if (pControl == NULL)
	{
		return NULL;
	}

	if (nCode < 49)
	{
		m_pCodeContainer->Add(pControl);
	}
	else  //号码49放在最后一排
	{
		m_pLastCodeContainer->Add(pControl);
	}		

	//号码显示名称
	CCode code;
	code.SetCode(nCode);		
	CString strName;
	strName.Format(_T("%02d%s"), nCode, code.GetShengXiao());

	//设置号码名称、颜色及值
	DuiLib::CLabelUI* pCodeName = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pControl, _T("nameLabel"));
	DuiLib::CEditUI* pCodeMoney = (DuiLib::CEditUI*)m_pm.FindSubControlByName(pControl, _T("moneyEdit"));
	pCodeName->SetText(strName);		
	if (code.IsRed())
	{
		pCodeName->SetTextColor(0xffff0000);
	}
	else if (code.IsGreen())
	{
		pCodeName->SetTextColor(0xff6e9c08);
	}
	else if (code.IsBlue())
	{
		pCodeName->SetTextColor(0xff0000ff);
	}

	pCodeMoney->SetTag(nCode);  //号码储存在此
	
	return pCodeMoney;
}

DuiLib::CControlUI* COrderManageDlg::FindCode(int nCode)
{
	DuiLib::CControlUI* pParentControl[2] = {m_pCodeContainer, m_pLastCodeContainer};
	for (int j=0; j<2; j++)
	{
		DuiLib::CStdPtrArray* pControlArray = m_pm.FindSubControlsByClass(pParentControl[j], _T("EditUI"));
		for (int i=0; i<pControlArray->GetSize(); i++)
		{
			DuiLib::CControlUI* pControl = (DuiLib::CControlUI*)(pControlArray->GetAt(i));
			if (pControl->GetName() == _T("moneyEdit") && pControl->GetTag() == nCode)
			{
				return pControl;
			}
		}
	}

	return NULL;
}

void COrderManageDlg::InitOrder()
{
	//当前用户单子
	bool bHasOrder = false;
	COrder order;
	CString strCustomer = m_pCustomerCombo->GetText();
	if (!strCustomer.IsEmpty() && COrderManager::Instance()->GetOrder(strCustomer, order))
	{
		bHasOrder = true;
	}

	//添加号码	
	for (int i=1; i<=12; i++)	
	{
		for (int j=0; j<=4; j++)
		{
			int nCode = i+j*12;
			if (nCode > COrder::CODE_NUMBER)
			{
				continue;
			}

			DuiLib::CControlUI* pCodeMoney = FindCode(nCode);
			if (pCodeMoney == NULL)  //还没有添加一个
			{
				pCodeMoney = AddCode(nCode);
				if (pCodeMoney == NULL)
				{
					continue;
				}
			}

			if (bHasOrder && order.Money(nCode)>0)
			{
				pCodeMoney->SetText(CStringUtil::Int64ToStr(order.Money(nCode)));
			}
			else
			{
				pCodeMoney->SetText(_T(""));
			}			
		}
	}		

	//总共下注数 16=%s总共下注%I64d 
	CString strTotal;
	strTotal.Format(CLanguageManager::Instance()->LoadString(16), strCustomer, bHasOrder?order.GetTotalBuyMoney():0);
	m_pTotalInfoLabel->SetText(strTotal);

	//显示删除单子按钮 和 删除所有单子按钮
	m_pDeleteOrderBtn->SetVisible(bHasOrder);
	m_pDeleteAllOrderBtn->SetVisible(COrderManager::Instance()->IsHasOrder());	
}

void COrderManageDlg::InitBuyLog()
{
	CString strLog = COperatorManager::Instance()->GetBuyLog();
	if (strLog.IsEmpty())
	{
		m_pBuyLogRichedit->SetText(CLanguageManager::Instance()->LoadString(3));  //3=还没有下注日志
	}
	else
	{
		m_pBuyLogRichedit->SetText(strLog);
	}
	m_pBuyLogRichedit->EndDown();
}

void COrderManageDlg::AddGroup(DuiLib::CContainerUI* pContainer, int nGroupID)
{
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pControl = builder.Create(_T("item_group.xml"), NULL, NULL, &m_pm);
	if (pControl == NULL)
	{
		return;
	}

	CCodeGroup group(nGroupID);
	pControl->SetText(group.Name());
	pControl->SetTag(nGroupID);  //group id保存在此
	pContainer->Add(pControl);	
}

void COrderManageDlg::InitGroupBuy()
{
	m_pCodeGroupContainer->RemoveAll();
	m_pLastCodeGroupContainer->RemoveAll();
	for (int id=GROUPID_MIN; id<=GROUPID_MAX; id++)
	{
		if (id <= GROUPID_GREENDOUBLE)
		{
			AddGroup(m_pCodeGroupContainer, id);			
		}
		else
		{
			AddGroup(m_pLastCodeGroupContainer, id);			
		}	
	}
}

void COrderManageDlg::InitLogicControlObject()
{
	//初始化下庄
	InitCustomer();

	//初始化单子
	InitOrder();

	//初始化下注日志
	InitBuyLog();

	//初始化分组
	InitGroupBuy();
}

void COrderManageDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理，按钮识别统一用名称比较
	//

	if (pSender == m_pAddCustomerBtn || pSender == m_pAddCustomerBtnWhenNoCustomer)  //添加下庄
	{
		OnAddCustomerBtn();
	}
	else if (pSender == m_pDeleteOrderBtn)	//删除当前下庄单子
	{
		OnDeleteOrderBtn();
	}
	else if (pSender == m_pDeleteAllOrderBtn)  //删除所有单子
	{
		OnDeleteAllOrderBtn();
	}	
	else  //call base
	{
		CUIDialogFrameWnd::OnClick(pSender);
	}
}

void COrderManageDlg::OnDeleteOrderBtn()
{
	CString strCustomer = m_pCustomerCombo->GetText().GetData();
	if (!strCustomer.IsEmpty())
	{
		//让用户再次确认是否删除
		CString strInfo;
		strInfo.Format(CLanguageManager::Instance()->LoadString(13), strCustomer);  //13=您确定删除%s的单子？
		if (DuiMessageBox(m_hWnd, strInfo, CLanguageManager::Instance()->LoadString(1), MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}

		COrderManager::Instance()->DeleteOrder(strCustomer);
	}
}

void COrderManageDlg::OnDeleteAllOrder()
{
	COrderManager::Instance()->DeleteOrder(_T(""));
	COperatorManager::Instance()->ClearBuyLog();
	m_pBuyLogRichedit->SetText(CLanguageManager::Instance()->LoadString(3));  //3=还没有下注日志
}

void COrderManageDlg::OnDeleteAllOrderBtn()
{
	//让用户再次确认 14=删除所有单子后，下注日志也将被清除，是否继续?
	if (DuiMessageBox(m_hWnd, CLanguageManager::Instance()->LoadString(14), CLanguageManager::Instance()->LoadString(1), MB_YESNO) == IDCANCEL)
	{
		return;
	}

	OnDeleteAllOrder();
}

void COrderManageDlg::OnMoneyEditReturn(DuiLib::CControlUI* pMoneyEdit)
{
	if (pMoneyEdit->GetText().IsEmpty())
	{
		return;
	}

	CString strBuy;
	strBuy.Format(_T("%02d%s"), (int)pMoneyEdit->GetTag(), pMoneyEdit->GetText().GetData());
	CSingleBuyMethod buyMethod(strBuy);	
	if (!COrderManager::Instance()->UpdateOrder(m_pCustomerCombo->GetText().GetData(), buyMethod.Buy()))
	{
		return;		
	}

	//记日志
	AddBuyLog(buyMethod.Log());	

	//去除提示
	static bool bFirst = true;
	if (bFirst)
	{
		bFirst = false;

		DuiLib::CStdPtrArray* controlArray = m_pm.FindSubControlsByClass(m_pCodeContainer, _T("EditUI"));
		for (int i=0; i<controlArray->GetSize(); i++)
		{
			((DuiLib::CEditUI*)(controlArray->GetAt(i)))->SetExampleText(_T(""));
		}

		controlArray = m_pm.FindSubControlsByClass(m_pLastCodeGroupContainer, _T("EditUI"));
		for (int i=0; i<controlArray->GetSize(); i++)
		{
			((DuiLib::CEditUI*)(controlArray->GetAt(i)))->SetExampleText(_T(""));
		}
	}	
}

void COrderManageDlg::OnAddCustomerBtn()
{
	CCommunicationManager::Instance()->Notify(COMID_MainDlg, COMEVENT_ShowCustomerPage);
}

void COrderManageDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//这里每个if用一个函数处理
	//	

	if (pSender->GetName() == _T("codeGroupOpt"))
	{
		OnCodeGroupSelChange(dynamic_cast<DuiLib::COptionUI*>(pSender));
	}
	else  //call base
	{
		CUIDialogFrameWnd::OnSelectChange(pSender);	
	}
}

LRESULT COrderManageDlg::OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	//每个事件用一个函数
	LRESULT result = 0;
	if (nCode == COMEVENT_CustomerChange)  //下庄用户列表变化
	{
		OnCustomerChange((int)wParam, (LPCTSTR)lParam);		
	}	
	else if (nCode == COMEVENT_OrderChange)  //单子变化
	{
		OnOrderChange((DWORD)wParam);
	}
	else if (nCode == COMEVENT_DeleteAllOrder)  //删除所有单子
	{
		OnDeleteAllOrder();
	}

	return result;
}

void COrderManageDlg::OnOrderChange(DWORD dwChangeMask)
{
	if ((dwChangeMask|ORDER_CHANGE_MASK_ORDER) == 0)
	{
		return;
	}

	//重新初始化单子
	InitOrder();
}

void COrderManageDlg::OnCustomerChange(int nChangeMask, const CString& strCustomer)
{
	if (nChangeMask == CUSTOMER_CHANGE_NEW_CUSTOMER)  //新增一个用户
	{
		OnNewCustomer(strCustomer);
	}
	else if (nChangeMask == CUSTOMER_CHANGE_DELETE_CUSTOMER)  //删除一个用户
	{
		OnDeleteCustomer(strCustomer);
	}
}

void COrderManageDlg::OnCodeGroupSelChange(DuiLib::COptionUI* pControl)
{
	if (pControl == NULL)
	{
		return;
	}

	//如果勾选五行分组，第一次提示它使用五行默认配置，是否需要修改
	if (pControl->IsSelected())
	{
		CCodeGroup group((int)pControl->GetTag());
		if (group.IsWuHangGroup() && CConfManager::Instance()->IsNeedHintConfigureWuHang())
		{
			CConfManager::Instance()->NeedHintConfigureWuHang(false);

			//12=五行默认号码为：金(%s)、木(%s)、水(%s)、火(%s)、土(%s)。是否需要修改？
			CString strGoldCode = CConfManager::Instance()->GoldCode();
			CString strWoodCode = CConfManager::Instance()->WoodCode();
			CString strWaterCode = CConfManager::Instance()->WaterCode();
			CString strFireCode = CConfManager::Instance()->FireCode();
			CString strEarthCode = CConfManager::Instance()->EarthCode();
			CString strInfo;
			strInfo.Format(CLanguageManager::Instance()->LoadString(12), strGoldCode, strWoodCode, strWaterCode, strFireCode, strEarthCode);
			if (DuiMessageBox(m_hWnd, strInfo, CLanguageManager::Instance()->LoadString(1), MB_YESNO) == IDOK)
			{
				//切换到配置页面
				CCommunicationManager::Instance()->Notify(COMID_MainDlg, COMEVENT_ShowSettingPage);
			}
		}
	}
}

void COrderManageDlg::OnItemSelect(DuiLib::CControlUI* pSender, int nCurSel, int nOldSel)
{
	//每个用一个函数处理
	if (pSender == m_pCustomerCombo)  //选择另一个下庄
	{
		OnCustomerItemChange();
	}
	else //call base
	{
		CUIDialogFrameWnd::OnItemSelect(pSender, nCurSel, nOldSel);
	}
}

void COrderManageDlg::OnTextReturn(DuiLib::CControlUI* pSender)
{
	//每个用一个函数处理
	if (pSender->GetName() == _T("moneyEdit"))  //逐个号码购买
	{
		OnMoneyEditReturn(pSender);
	}
	else if (pSender == m_pSingleBuyEdit)  //单号串购买
	{
		OnSingleBuyEditReturn();
	}
	else if (pSender == m_pIntervalBuyEdit)  //区间段购买
	{
		OnIntervalBuyEditReturn();
	}
	else if (pSender == m_pEqualBuyEdit)  //等额购买
	{
		OnEqualBuyEditReturn();
	}
	else if (pSender == m_pSequenceBuyEdit)  //顺号购买
	{
		OnSequenceBuyEditReturn();
	}
	else if (pSender == m_pGroupBuyEdit)  //分组购买
	{
		OnGroupBuyEditReturn();
	}
	else
	{
		CUIDialogFrameWnd::OnTextReturn(pSender);
	}
}

void COrderManageDlg::OnTextChange(DuiLib::CControlUI* pSender)
{
	//每个用一个函数处理
	if (pSender == m_pSingleBuyEdit)  //单号串购买
	{
		OnSingleBuyEditTextChange();
	}
	else if (pSender == m_pEqualCodeEdit)  //等额购买号码
	{
		OnEqualCodeEditTextChange();
	}
	else
	{
		CUIDialogFrameWnd::OnTextChange(pSender);
	}
}

void COrderManageDlg::OnChar(DuiLib::CControlUI* pSender, TCHAR ch)
{
	//如果开启声音校验，对数字输入播放音频
	if (CConfManager::Instance()->IsSoundCheck())
	{
		int num = CStringUtil::CharToInt(ch);
		if (num != -1)
		{
			CSoundManager::Instance()->Play(IDR_WAVE1+num);
		}
	}	
}

void COrderManageDlg::AddBuyLog(const CString& strLog)
{
	if (strLog.IsEmpty())
	{
		return;
	}

	CString strBuyLog = m_pCustomerCombo->GetText().GetData();
	strBuyLog += _T(' ');	
	strBuyLog += strLog;
	COperatorManager::Instance()->AppendBuyLog(strBuyLog);
	m_pBuyLogRichedit->SetText(COperatorManager::Instance()->GetBuyLog());
	m_pBuyLogRichedit->EndDown();
}

void COrderManageDlg::OnSingleBuyEditReturn()
{
	CSingleBuyMethod buyMethod(m_pSingleBuyEdit->GetText().GetData());
	vector<CCode> codeVec = buyMethod.Buy();
	if (codeVec.size() <= 0)  //错误串
	{
		return;
	}

	if (!COrderManager::Instance()->OrderAppend(m_pCustomerCombo->GetText().GetData(), codeVec)) //失败
	{
		return;
	}
	
	//记日志
	AddBuyLog(buyMethod.Log());	

	//清空内容及状态图
	m_pSingleBuyEdit->SetText(_T(""));	
}

void COrderManageDlg::OnIntervalBuyEditReturn()
{
	if (m_pIntervalStartEdit->GetText().IsEmpty() || m_pIntervalEndEdit->GetText().IsEmpty() || m_pIntervalBuyEdit->GetText().IsEmpty())
	{
		return;
	}

	//不是一个数，不继续
	CStringUtil strUtil(m_pIntervalBuyEdit->GetText().GetData());
	if (!strUtil.IsNumber())
	{
		return;
	}

	int nStartCode = _ttoi(m_pIntervalStartEdit->GetText().GetData());
	int nEndCode = _ttoi(m_pIntervalEndEdit->GetText().GetData());
	__int64 nBuyValue = _ttoi64(m_pIntervalBuyEdit->GetText().GetData());
	CIntervalBuyMethod buyMethod(nStartCode, nEndCode, nBuyValue);
	if (!COrderManager::Instance()->OrderAppend(m_pCustomerCombo->GetText().GetData(), buyMethod.Buy()))
	{
		return;
	}

	//记日志
	AddBuyLog(buyMethod.Log());		

	//清空内容
	m_pIntervalStartEdit->SetText(_T(""));
	m_pIntervalStartEdit->SetFocus();
	m_pIntervalEndEdit->SetText(_T(""));
	m_pIntervalBuyEdit->SetText(_T(""));

	//清除提示
	//m_pIntervalBuyEdit->SetExampleText(_T(""));
}

void COrderManageDlg::OnEqualBuyEditReturn()
{
	if (m_pEqualBuyEdit->GetText().IsEmpty() || m_pEqualCodeEdit->GetText().IsEmpty())
	{
		return;
	}

	//不是一个数，不继续
	CStringUtil strUtil(m_pEqualBuyEdit->GetText().GetData());
	if (!strUtil.IsNumber())
	{
		return;
	}

	CEqualBuyMethod buyMethod(m_pEqualCodeEdit->GetText().GetData(), _ttoi64(m_pEqualBuyEdit->GetText().GetData()));
	if (!buyMethod.IsCorrect())
	{
		return;
	}

	if (!COrderManager::Instance()->OrderAppend(m_pCustomerCombo->GetText().GetData(), buyMethod.Buy()))
	{
		return;
	}

	//记日志
	AddBuyLog(buyMethod.Log());

	//清空内容
	m_pEqualCodeEdit->SetText(_T(""));
	m_pEqualCodeEdit->SetFocus();
	m_pEqualBuyEdit->SetText(_T(""));

	//清除提示
	//m_pEqualBuyEdit->SetExampleText(_T(""));
}

void COrderManageDlg::OnEqualCodeEditTextChange()
{
	//用户输入错误指示
	if (m_pEqualCodeEdit->GetText().IsEmpty())
	{
		m_pEqualBuyWrongLabel->SetVisible(false);
	}
	else
	{
		CEqualBuyMethod buyMethod(m_pEqualCodeEdit->GetText().GetData(), _ttoi64(m_pEqualBuyEdit->GetText().GetData()));
		if (!buyMethod.IsCorrect())
		{
			m_pEqualBuyWrongLabel->SetVisible(true);
		}
		else
		{
			m_pEqualBuyWrongLabel->SetVisible(false);
		}
	}

	//智能给用户加上空格
	CString strTemp = m_pEqualCodeEdit->GetText().GetData();
	int nLength = strTemp.GetLength();
	int nLastLength = (int)m_pEqualCodeEdit->GetTag();  //上次长度
	if (nLength > nLastLength)  //字符是增加的
	{
		if (nLength >= 2 && strTemp[nLength-1] != _T(' ') && strTemp[nLength-2] != _T(' '))
		{
			strTemp += _T(' ');
			m_pEqualCodeEdit->SetText(strTemp);
		}
	}

	m_pEqualCodeEdit->SetTag(nLength);  //当前字符串长度保存在此
}

void COrderManageDlg::OnSingleBuyEditTextChange()
{
	if (m_pSingleBuyEdit->GetText().IsEmpty())
	{
		OnSingleBuyStatus(false);
	}
	else
	{
		CSingleBuyMethod buyMethod(m_pSingleBuyEdit->GetText().GetData());
		if (buyMethod.IsCorrect())
		{
			OnSingleBuyStatus(false);
		}
		else
		{
			OnSingleBuyStatus(true);
		}
	}
}

void COrderManageDlg::OnSequenceBuyEditReturn()
{
	if (m_pStartCodeEdit->GetText().IsEmpty() || m_pSequenceBuyEdit->GetText().IsEmpty())
	{
		return;
	}

	//不是一个数，不继续
	CStringUtil strUtil(m_pSequenceBuyEdit->GetText().GetData());
	if (!strUtil.IsNumber())
	{
		return;
	}

	int nCode = _ttoi(m_pStartCodeEdit->GetText().GetData());
	CString strCode;
	strCode.Format(_T("%02d"), nCode);
	CSingleBuyMethod buyMethod(strCode+m_pSequenceBuyEdit->GetText().GetData());
	if (!buyMethod.IsCorrect()) //用户输错号码，清空让他重新输入
	{
		m_pStartCodeEdit->SetText(_T(""));
		m_pSequenceBuyEdit->SetText(_T(""));	
		return;
	}

	if (!COrderManager::Instance()->OrderAppend(m_pCustomerCombo->GetText().GetData(), buyMethod.Buy()))
	{
		return;		
	}	

	//记日志
	AddBuyLog(buyMethod.Log());		

	//计算下一个号码
	int nNextCode = _ttoi(m_pStartCodeEdit->GetText().GetData())+1;
	if (nNextCode > COrder::CODE_NUMBER)
	{
		nNextCode = 1;
	}

	//设置为下一个号码，清空购买金额
	m_pStartCodeEdit->SetText(CStringUtil::IntToStr(nNextCode));
	m_pSequenceBuyEdit->SetText(_T(""));	
	//m_pSequenceBuyEdit->SetExampleText(_T(""));  //去除提示
}

void COrderManageDlg::OnGroupBuyEditReturn()
{
	//获取选中的分组
	set<CCodeGroup> groupSet;
	DuiLib::CContainerUI* pGroupContainer[]= {m_pCodeGroupContainer, m_pLastCodeGroupContainer, m_pRecentGroupContainer};
	for (int j=0; j<sizeof(pGroupContainer)/sizeof(DuiLib::CContainerUI*); j++)
	{
		for (int i=0; i<pGroupContainer[j]->GetCount(); i++)
		{		
			DuiLib::COptionUI* pOption = dynamic_cast<DuiLib::COptionUI*>(pGroupContainer[j]->GetItemAt(i));
			if (pOption && pOption->IsSelected())
			{
				CCodeGroup group((int)(pOption->GetTag()));  //group id
				groupSet.insert(group);
				pOption->Selected(false, false);  //现在就取消选中状态，避免后面还要查找控件遍历取消
			}		
		}
	}

	if (groupSet.size() <= 0)
	{
		return;
	}

	//不是一个数，不继续
	CStringUtil strUtil(m_pGroupBuyEdit->GetText().GetData());
	if (!strUtil.IsNumber())
	{
		return;
	}

	//购买失败，不处理
	__int64 nBuyMoney = _ttoi64(m_pGroupBuyEdit->GetText().GetData());	
	CGroupBuyMethod buyMethod(groupSet, nBuyMoney);
	vector<CCode> buyVec = buyMethod.Buy();
	if (!COrderManager::Instance()->OrderAppend(m_pCustomerCombo->GetText().GetData(), buyVec))
	{
		return;
	}

	//记日志
	AddBuyLog(buyMethod.Log());		

	//初始化最近使用分组
	m_pRecentContainer->SetVisible(true);
	vector<CCodeGroup> groupVec(groupSet.begin(), groupSet.end());  //set会进行排序，所以用vector存储
	for (int i=0; i<m_pRecentGroupContainer->GetCount(); i++)  //将当前最近使用列表也认为是刚购买的
	{		
		DuiLib::COptionUI* pOption = dynamic_cast<DuiLib::COptionUI*>(m_pRecentGroupContainer->GetItemAt(i));
		if (pOption)
		{
			bool bHasInVector =false;
			for (vector<CCodeGroup>::const_iterator it=groupVec.begin(); it!=groupVec.end(); it++)
			{
				if (it->ID() == pOption->GetTag())
				{
					bHasInVector = true;
					break;
				}
			}

			if (!bHasInVector)
			{
				CCodeGroup group((int)(pOption->GetTag()));  //group id
				groupVec.push_back(group);
			}			
		}		
	}

	m_pRecentGroupContainer->RemoveAll();
	int nAlreadyInsertCount = 0;
	int nMaxCount = _ttoi((LPCTSTR)(m_pRecentGroupContainer->GetUserData()));  //最多个数存放在此
	for (vector<CCodeGroup>::const_iterator it=groupVec.begin(); it!=groupVec.end(); it++)
	{
		AddGroup(m_pRecentGroupContainer, it->ID());
		nAlreadyInsertCount++;
		if (nAlreadyInsertCount >= nMaxCount)
		{
			break;
		}
	}

	//删除购买金额	
	m_pGroupBuyEdit->SetText(_T(""));

	//去除提示
	//m_pGroupBuyEdit->SetExampleText(_T(""));
}

void COrderManageDlg::OnSingleBuyStatus(bool bShow)
{
	m_pSingleBuyWrongLabel->SetVisible(bShow);	
}

void COrderManageDlg::OnCustomerItemChange()
{
	//重新初始化单子
	InitOrder();
}

void COrderManageDlg::OnNewCustomer(const CString& strCustomer)
{
	DuiLib::CDialogBuilder builder;
	DuiLib::CControlUI* pControl = builder.Create(_T("item_list_label_element.xml"));
	if (pControl == NULL)
	{
		return;
	}

	pControl->SetText(strCustomer);
	m_pCustomerCombo->Add(pControl);
	m_pCustomerCombo->SelectItem(m_pCustomerCombo->GetCount()-1);
	m_pNoCustomerContainer->SetVisible(false);	
}

void COrderManageDlg::OnDeleteCustomer(const CString& strCustomer)
{
	for (int i=0; i<m_pCustomerCombo->GetCount(); i++)
	{
		if (strCustomer == m_pCustomerCombo->GetItemAt(i)->GetText().GetData())
		{
			m_pCustomerCombo->RemoveAt(i);
		}
	}

	//一个下庄也没有显示添加下庄页面
	if (m_pCustomerCombo->GetCount() == 0)
	{
		m_pNoCustomerContainer->SetVisible(true);
	}
}