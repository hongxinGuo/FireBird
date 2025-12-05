#include "pch.h"
#include "framework.h"

#include "PropertyRealTimeDataWnd.h"

#include "ChinaMarket.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

//
// CPropertyWnd
//
CPropertyRealtimeWnd::CPropertyRealtimeWnd() noexcept {
	m_nComboHeight = 0;
	m_uIdTimer = 0;
}

CPropertyRealtimeWnd::~CPropertyRealtimeWnd() {
}

BEGIN_MESSAGE_MAP(CPropertyRealtimeWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertyRealtimeWnd::AdjustLayout() {
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic())) {
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), rectClient.Height() - (m_nComboHeight), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertyRealtimeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create Properties
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2)) {
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	AdjustLayout();

	// 设置1秒每次的软调度
	m_uIdTimer = SetTimer(static_cast<UINT_PTR>(4), 1000, nullptr);
	if (m_uIdTimer == 0) {
	}

	return 0;
}

void CPropertyRealtimeWnd::OnSize(UINT nType, int cx, int cy) {
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertyRealtimeWnd::InitPropList() {
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	m_pPropertyStock = new CGridProperty("");

	m_pPropNew = new CGridProperty(_T("Current Price:"), _T("0.00"));
	m_pPropNew->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropNew);
	m_pPropVolume = new CGridProperty(_T("Volume:"), _T("0.00"));
	m_pPropVolume->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropVolume);
	m_pPropStockOrdinaryBuy = new CGridProperty(_T("Ordinary Buy:"), _T("0.00"));
	m_pPropStockOrdinaryBuy->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockOrdinaryBuy);
	m_pPropStockAttackBuy = new CGridProperty(_T("Attack Buy:"), _T("0.00"));
	m_pPropStockAttackBuy->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockAttackBuy);
	m_pPropStockStrongBuy = new CGridProperty(_T("Strong Buy:"), _T("0.00"));
	m_pPropStockStrongBuy->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockStrongBuy);
	m_pPropStockCancelBuy = new CGridProperty(_T("Cancel Buy:"), _T("0.00"));
	m_pPropStockCancelBuy->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockCancelBuy);

	m_pPropStockOrdinarySell = new CGridProperty(_T("Ordinary Sell:"), _T("0.00"));
	m_pPropStockOrdinarySell->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockOrdinarySell);
	m_pPropStockAttackSell = new CGridProperty(_T("Attack Sell:"), _T("0.00"));
	m_pPropStockAttackSell->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockAttackSell);
	m_pPropStockStrongSell = new CGridProperty(_T("Strong Sell:"), _T("0.00"));
	m_pPropStockStrongSell->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockStrongSell);
	m_pPropStockCancelSell = new CGridProperty(_T("Cancel Sell:"), _T("0.00"));
	m_pPropStockCancelSell->Enable(FALSE);
	m_pPropertyStock->AddSubItem(m_pPropStockCancelSell);

	m_wndPropList.AddProperty(m_pPropertyStock);
}

void CPropertyRealtimeWnd::OnSetFocus(CWnd* pOldWnd) {
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertyRealtimeWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) {
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertyRealtimeWnd::SetPropListFont() {
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CPropertyRealtimeWnd::OnTimer(UINT_PTR nIDEvent) {
	if (gl_pCurrentStock != nullptr) {
		m_pPropertyStock->SetName(ToUTF16(gl_pCurrentStock->GetSymbol()).c_str());
		if (IsChinaStock(gl_pCurrentStock)) {
			CChinaStockPtr pStock = dynamic_pointer_cast<CChinaStock>(gl_pCurrentStock);
			if (pStock->GetVolume() > 0) {
				string s = fmt::format("{:Ld}", pStock->GetNew());
				m_pPropNew->SetValue(s);
				INT64 volume = pStock->GetVolume();
				s = fmt::format("{:Ld}", volume);
				m_pPropVolume->SetValue(s);
				s = fmt::format("0.{:02Ld}", pStock->GetOrdinaryBuyVolume() * 100 / volume);
				m_pPropStockOrdinaryBuy->SetValue(s);
				s = fmt::format("0.{:02Ld}", pStock->GetOrdinarySellVolume() * 100 / volume);
				m_pPropStockOrdinarySell->SetValue(s);

				s = fmt::format("0.{:02Ld}", pStock->GetAttackBuyVolume() * 100 / volume);
				m_pPropStockAttackBuy->SetValue(s);
				s = fmt::format("0.{:02Ld}", pStock->GetAttackSellVolume() * 100 / volume);
				m_pPropStockAttackSell->SetValue(s);

				s = fmt::format("0.{:02Ld}", pStock->GetStrongBuyVolume() * 100 / volume);
				m_pPropStockStrongBuy->SetValue(s);
				s = fmt::format("0.{:02Ld}", pStock->GetStrongSellVolume() * 100 / volume);
				m_pPropStockStrongSell->SetValue(s);

				s = fmt::format("0.{:02Ld}", pStock->GetCanceledBuyVolume() * 100 / volume);
				m_pPropStockCancelBuy->SetValue(s);
				s = fmt::format("0.{:02Ld}", pStock->GetCanceledSellVolume() * 100 / volume);
				m_pPropStockCancelSell->SetValue(s);
			}
		}
	}
	else {
		m_pPropertyStock->SetName(_T(""));
	}

	CDockablePane::OnTimer(nIDEvent);
}
