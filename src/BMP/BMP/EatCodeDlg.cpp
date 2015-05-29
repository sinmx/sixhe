#include "StdAfx.h"
#include "EatCodeDlg.h"
#include "CommunicationDefine.h"
#include "OrderManager.h"
#include "ConfManager.h"
#include "EatMethod.h"
#include "resource.h"
#include "SoundManager.h"

CEatCodeDlg::CEatCodeDlg(void) :
m_pNoOrderContainer(NULL),
m_pOrderBtn(NULL),

m_pAvgDiscountEdit(NULL),
m_pAvgBettingOddsEdit(NULL),

m_pCancelEatBtn(NULL),
m_pPrintSellBtn(NULL),
m_pMinMaxProfitLabel(NULL),

m_pFirstColumnList(NULL),
m_pSecondColumnList(NULL),
m_pThirdColumnList(NULL),

m_pTotalInfoLabel(NULL),

m_pSuggestionLabel(NULL),
m_pAvgEatEdit(NULL),
m_pCalcEatEdit(NULL),
m_pCalcAvgEatEdit(NULL)
{
}

CEatCodeDlg::~CEatCodeDlg(void)
{
}

int CEatCodeDlg::GetID()
{
	return COMID_EatCodeDlg;
}

int CEatCodeDlg::AcceptEvent(int nEventCode, WPARAM wParam, LPARAM lParam)
{
	return SendEvent(nEventCode, wParam, lParam);
}

void CEatCodeDlg::OnInitDialog()
{
	CUIDialogFrameWnd::OnInitDialog(); //call base

	//��ӵ�ͨ�Ź�������
	CCommunicationManager::Instance()->Attach(this);

	//�����������ݵı仯
	COrderManager::Instance()->Attach(this);

	//�������ƻ�ȡ�ؼ������ַ
	GetLogicControlObject();

	//��ʼ���߼��ؼ�����
	InitLogicControlObject();		
}

void CEatCodeDlg::OnFinalMessage(HWND hWnd)
{
	//��ͨ�Ź�������ȥ��
	CCommunicationManager::Instance()->Detach(this);

	//�ӵ��ӹ����������б���ȥ��
	COrderManager::Instance()->Detach(this);

	DuiLib::CUIDialogFrameWnd::OnFinalMessage(hWnd);
}

void CEatCodeDlg::GetLogicControlObject()
{
	//
	//ͨ�����Ʋ��ҿؼ����󣬲���dynamic_cast���а�ȫת��	
	//

	m_pNoOrderContainer = dynamic_cast<DuiLib::CContainerUI*>(m_pm.FindControl(_T("noOrderContainer")));
	ASSERT(m_pNoOrderContainer);
	m_pOrderBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindSubControlByName(m_pNoOrderContainer, _T("orderBtn")));
	ASSERT(m_pOrderBtn);

	m_pAvgDiscountEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("avgDiscountEdit")));
	ASSERT(m_pAvgDiscountEdit);
	m_pAvgBettingOddsEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("avgBettingOddsEdit")));
	ASSERT(m_pAvgBettingOddsEdit);

	m_pMinMaxProfitLabel = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("maxMinProfitLabel")));
	ASSERT(m_pMinMaxProfitLabel);

	m_pCancelEatBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("cancelEatBtn")));
	ASSERT(m_pCancelEatBtn);
	m_pPrintSellBtn = dynamic_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("printSellBtn")));
	ASSERT(m_pPrintSellBtn);

	m_pFirstColumnList = dynamic_cast<DuiLib::CListUI*>(m_pm.FindControl(_T("firstColumnList")));
	ASSERT(m_pFirstColumnList);
	m_pSecondColumnList = dynamic_cast<DuiLib::CListUI*>(m_pm.FindControl(_T("secondColumnList")));
	ASSERT(m_pSecondColumnList);
	m_pThirdColumnList = dynamic_cast<DuiLib::CListUI*>(m_pm.FindControl(_T("thirdColumnList")));
	ASSERT(m_pThirdColumnList);	

	m_pTotalInfoLabel = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("totalInfoLabel")));
	ASSERT(m_pTotalInfoLabel);

	m_pSuggestionLabel = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("suggestionLabel")));
	ASSERT(m_pSuggestionLabel);
	m_pAvgEatEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("avgEatEdit")));
	ASSERT(m_pAvgEatEdit);

	m_pCalcEatEdit = dynamic_cast<DuiLib::CEditUI*>(m_pm.FindControl(_T("calcEatEdit")));
	ASSERT(m_pCalcEatEdit);
	m_pCalcAvgEatEdit = dynamic_cast<DuiLib::CLabelUI*>(m_pm.FindControl(_T("calcAvgEatLabel")));
	ASSERT(m_pCalcAvgEatEdit);	
}

void CEatCodeDlg::InitLogicControlObject()
{
	//��ʼ��������ؿؼ�
	InitEat();	
}

void CEatCodeDlg::InitEat()
{
	//ƽ����ˮ��ƽ������
	m_pAvgDiscountEdit->SetText(CStringUtil::IntToStr(CConfManager::Instance()->AvgDiscount()));
	m_pAvgBettingOddsEdit->SetText(CStringUtil::IntToStr(CConfManager::Instance()->AvgBettingOdds()));	

	//��ӡ�ϱ�
	m_pPrintSellBtn->SetVisible(!COrderManager::Instance()->IsAllEat());

	//�����С����
	int nMinCode, nMaxCode;
	__int64 nMinProfit, nMaxProfit;
	COrderManager::Instance()->GetMinMaxProfit(nMinProfit, nMinCode, nMaxProfit, nMaxCode);
	CString strMinMaxProfit;
	if (nMinProfit < 0)
	{
		//34=��������Ϊ%d��������%I64d ��������Ϊ%d׬������%I64d
		strMinMaxProfit.Format(CLanguageManager::Instance()->LoadString(34), nMinCode, nMinProfit*(-1), nMaxCode, nMaxProfit);
	}
	else
	{
		//35=��������Ϊ%d׬������ֻ��%I64d ��������Ϊ%d׬������%I64d
		strMinMaxProfit.Format(CLanguageManager::Instance()->LoadString(35), nMinCode, nMinProfit, nMaxCode, nMaxProfit);
	}
	m_pMinMaxProfitLabel->SetText(strMinMaxProfit);

	//��Ӻ���
	for (int nCode=1; nCode<=COrder::CODE_NUMBER; nCode++)
	{
		if (nCode <= 16)
		{
			AddOneCode(m_pFirstColumnList, nCode);
		}
		else if (nCode <= 32)
		{
			AddOneCode(m_pSecondColumnList, nCode);
		}
		else
		{
			AddOneCode(m_pThirdColumnList, nCode);
		}
	}

	//������Ϣ 15=��ע����%I64d   ��������%I64d   �ϱ�����%I64d
	__int64 nTotalBuy = COrderManager::Instance()->GetTotalBuy();
	__int64 nTotalEat = COrderManager::Instance()->GetEatValue();
	CString strTotal;
	strTotal.Format(CLanguageManager::Instance()->LoadString(15), nTotalBuy, nTotalEat, nTotalBuy-nTotalEat);
	m_pTotalInfoLabel->SetText(strTotal);

	//����ƽ������
	__int64 nAbsoluteProfit, nAvgEat;
	COrderManager::Instance()->CalcAbsoluteProfit(nAbsoluteProfit, nAvgEat);
	if (nAbsoluteProfit == 0)  //û�о�������
	{
		//9=û�о������󲻽������
		m_pSuggestionLabel->SetText(CLanguageManager::Instance()->LoadString(9));
	}
	else
	{
		__int64 nMinProfit, nMaxProfit;
		COrderManager::Instance()->CalcAvgEatProfit(nAvgEat, nMinProfit, nMaxProfit);

		//8=����ƽ������%I64d���ٿ�׬%d����׬%I64d
		CString strTemp;
		strTemp.Format(CLanguageManager::Instance()->LoadString(8), nAvgEat, nMinProfit, nMaxProfit);
		m_pSuggestionLabel->SetText(strTemp);
	}

	m_pAvgEatEdit->SetText(_T(""));
	m_pCalcEatEdit->SetText(_T(""));
	m_pCalcAvgEatEdit->SetText(_T(""));

	//û���κζ�������ʾ�ӵ�ҳ��
	m_pNoOrderContainer->SetVisible(!COrderManager::Instance()->IsHasOrder());
}

void CEatCodeDlg::AddOneCode(DuiLib::CContainerUI* pContainer, int nCode)
{
	DuiLib::CControlUI* pCodeContainer = NULL;
	DuiLib::CStdPtrArray* tmpArray = m_pm.FindSubControlsByClass(pContainer, _T("ListContainerElementUI"));
	for (int i=0; i<tmpArray->GetSize(); i++)
	{
		DuiLib::CControlUI* pTempControl = (DuiLib::CControlUI*)tmpArray->GetAt(i);
		if (pTempControl->GetName() == _T("codeContainer") && pTempControl->GetTag() == nCode)  //�ҵ�
		{
			pCodeContainer = pTempControl;
			break;
		}
	}

	if (pCodeContainer == NULL)  //û�ҵ�������һ��
	{
		DuiLib::CDialogBuilder builder;
		pCodeContainer = builder.Create(_T("item_eat_code.xml"), NULL, NULL, &m_pm);
		if (pCodeContainer == NULL)
		{
			return;
		}

		pContainer->Add(pCodeContainer);
		pCodeContainer->SetTag(nCode);  //���봢���ڴ�
	}	

	//���ø��ֶ�����
	DuiLib::CLabelUI* pCodeName = (DuiLib::CLabelUI*)m_pm.FindSubControlByName(pCodeContainer, _T("codeLabel"));
	DuiLib::CEditUI* pBuyEdit = (DuiLib::CEditUI*)m_pm.FindSubControlByName(pCodeContainer, _T("buyTotalEdit"));
	DuiLib::CEditUI* pEatEdit = (DuiLib::CEditUI*)m_pm.FindSubControlByName(pCodeContainer, _T("eatEdit"));
	DuiLib::CEditUI* pSellEdit = (DuiLib::CEditUI*)m_pm.FindSubControlByName(pCodeContainer, _T("sellEdit"));
	DuiLib::CEditUI* pProfitEdit = (DuiLib::CEditUI*)m_pm.FindSubControlByName(pCodeContainer, _T("profitEdit"));

	__int64 nTotalBuy = COrderManager::Instance()->GetTotalBuy(nCode);
	__int64 nEat = COrderManager::Instance()->GetEatValue(nCode);
	
	pCodeName->SetText(CStringUtil::IntToStr(nCode));
	
	if (nTotalBuy > 0)
	{
		pBuyEdit->SetText(CStringUtil::Int64ToStr(nTotalBuy));
	}
	else
	{
		pBuyEdit->SetText(_T(""));
	}
	
	if (nEat > 0)
	{
		pEatEdit->SetText(CStringUtil::Int64ToStr(nEat));
		pEatEdit->SetEnabled(true);
	}
	else
	{
		pEatEdit->SetText(_T(""));
		pEatEdit->SetEnabled(false);
	}

	if (nTotalBuy - nEat > 0)
	{
		pSellEdit->SetText(CStringUtil::Int64ToStr(nTotalBuy-nEat));
	}
	else
	{
		pSellEdit->SetText(_T(""));
	}

	pProfitEdit->SetText(CStringUtil::Int64ToStr(COrderManager::Instance()->CalcCodeProfit(nCode)));
}

void CEatCodeDlg::OnClick(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ����������
	//

	if (pSender == m_pCancelEatBtn)  //ȡ������
	{
		OnCancelEatBtn();
	}
	else if (pSender == m_pPrintSellBtn)  //��ӡ�ϱ�
	{
		OnPrintSellBtn();
	}
	else if (pSender == m_pOrderBtn)  //�ӵ�
	{
		OnOrderBtn();
	}
	else
	{
		CUIDialogFrameWnd::OnClick(pSender);
	}
}

void CEatCodeDlg::OnOrderBtn()
{
	CCommunicationManager::Instance()->Notify(COMID_MainDlg, COMEVENT_ShowOrderPage);
}

void CEatCodeDlg::OnCancelEatBtn()
{
	COrderManager::Instance()->RestEatTable();
}

void CEatCodeDlg::OnOrderChange()
{
	//���³�ʼ������
	InitLogicControlObject();
}

void CEatCodeDlg::OnPrintSellBtn()
{
	COrderManager::Instance()->PrintSelfBuyTable();
}

void CEatCodeDlg::OnSelectChange(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ������������ťʶ��ͳһ�����ƱȽ�
	//	

	CUIDialogFrameWnd::OnSelectChange(pSender);	
}

void CEatCodeDlg::OnTextReturn(DuiLib::CControlUI* pSender)
{
	//
	//����ÿ��if��һ����������
	//
	if (pSender == m_pAvgDiscountEdit)  //ƽ����ˮ
	{
		OnAvgDiscountEditReturn();
	}
	else if (pSender == m_pAvgBettingOddsEdit)  //ƽ������
	{
		OnAvgBettingOddsEditReturn();
	}
	else if (pSender->GetName() == _T("eatEdit"))  //��������
	{
		OnEatEditReturn(pSender);
	}
	else if (pSender == m_pAvgEatEdit)  //ƽ������
	{
		OnAvgEatEditReturn();
	}
	else if (pSender == m_pCalcEatEdit) //���������
	{
		OnCalcEatEditReturn();
	}	
	else  //call base
	{
		CUIDialogFrameWnd::OnTextReturn(pSender);
	}
}

void CEatCodeDlg::OnTextChange(DuiLib::CControlUI* pSender)
{
	//ÿ����һ����������	
}

LRESULT CEatCodeDlg::OnAcceptEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	//ÿ��if��һ����������
	if (nCode == COMEVENT_OrderChange)
	{
		OnOrderChange();
	}

	return 0;
}

void CEatCodeDlg::OnChar(DuiLib::CControlUI* pSender, TCHAR ch)
{
	//�����������У�飬���������벥����Ƶ
	if (CConfManager::Instance()->IsSoundCheck())
	{
		int num = CStringUtil::CharToInt(ch);
		if (num != -1)
		{
			CSoundManager::Instance()->Play(IDR_WAVE1+num);
		}
	}
}

void CEatCodeDlg::OnAvgDiscountEditReturn()
{
	int nDiscount = _ttoi(m_pAvgDiscountEdit->GetText().GetData());
	if (nDiscount > 1000)  //����1000����ֱ������Ϊ1000
	{
		nDiscount = 1000;
		m_pAvgDiscountEdit->SetText(CStringUtil::IntToStr(nDiscount));
	}

	//���沢���³�ʼ������
	if (CConfManager::Instance()->AvgDiscount(nDiscount))
	{
		InitLogicControlObject();
	}

	//ȥ����ʾ
	m_pAvgDiscountEdit->SetExampleText(_T(""));
}

void CEatCodeDlg::OnAvgBettingOddsEditReturn()
{
	int nBettingOdds = _ttoi(m_pAvgBettingOddsEdit->GetText().GetData());
	if (CConfManager::Instance()->AvgBettingOdds(nBettingOdds))
	{
		InitLogicControlObject();
	}

	//ȥ����ʾ
	m_pAvgBettingOddsEdit->SetExampleText(_T(""));
}

void CEatCodeDlg::OnEatEditReturn(DuiLib::CControlUI* pControl)
{
	DuiLib::CControlUI* pContainer = m_pm.FindFirstParentByClass(pControl, _T("ListContainerElementUI"));
	if (pContainer && pContainer->GetName() == _T("codeContainer"))
	{
		CCode code;
		code.SetCode((int)pContainer->GetTag());
		code.Buy(_ttoi64(pControl->GetText().GetData()));

		vector<CCode> codeVec;
		codeVec.push_back(code);

		COrderManager::Instance()->UpdateEatTable(codeVec);		
	}
}

void CEatCodeDlg::OnAvgEatEditReturn()
{
	CAvgEatMethod avgMethod(_ttoi64(m_pAvgEatEdit->GetText().GetData()));
	vector<CCode> eatTable = avgMethod.Eat();
	COrderManager::Instance()->UpdateEatTable(eatTable);
	m_pAvgEatEdit->SetExampleText(_T(""));
}

void CEatCodeDlg::OnCalcEatEditReturn()
{
	__int64 nAvgEat = 0;
	COrderManager::Instance()->CalcMinProfit(_ttoi64(m_pCalcEatEdit->GetText().GetData())*(-1), nAvgEat);
	m_pCalcAvgEatEdit->SetText(CStringUtil::Int64ToStr(nAvgEat));
	m_pCalcEatEdit->SetExampleText(_T(""));  //ȥ����ʾ
}