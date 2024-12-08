#include "pch.h"
#include "framework.h"

import FireBird.WindowInterface.PropertyRealTimeDataWnd;

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
		CString str1;
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

	m_pChinaMarketStock = new CMFCPropertyGridProperty(_T(""));

	m_pPropNew = new CMFCPropertyGridProperty(_T("Current Price:"), _T("0.00"));
	m_pPropNew->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropNew);
	m_pPropVolume = new CMFCPropertyGridProperty(_T("Volume:"), _T("0.00"));
	m_pPropVolume->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropVolume);
	m_pPropStockOrdinaryBuy = new CMFCPropertyGridProperty(_T("Ordinary Buy:"), _T("0.00"));
	m_pPropStockOrdinaryBuy->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockOrdinaryBuy);
	m_pPropStockAttackBuy = new CMFCPropertyGridProperty(_T("Attack Buy:"), _T("0.00"));
	m_pPropStockAttackBuy->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockAttackBuy);
	m_pPropStockStrongBuy = new CMFCPropertyGridProperty(_T("Strong Buy:"), _T("0.00"));
	m_pPropStockStrongBuy->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockStrongBuy);
	m_pPropStockCancelBuy = new CMFCPropertyGridProperty(_T("Cancel Buy:"), _T("0.00"));
	m_pPropStockCancelBuy->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockCancelBuy);

	m_pPropStockOrdinarySell = new CMFCPropertyGridProperty(_T("Ordinary Sell:"), _T("0.00"));
	m_pPropStockOrdinarySell->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockOrdinarySell);
	m_pPropStockAttackSell = new CMFCPropertyGridProperty(_T("Attack Sell:"), _T("0.00"));
	m_pPropStockAttackSell->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockAttackSell);
	m_pPropStockStrongSell = new CMFCPropertyGridProperty(_T("Strong Sell:"), _T("0.00"));
	m_pPropStockStrongSell->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockStrongSell);
	m_pPropStockCancelSell = new CMFCPropertyGridProperty(_T("Cancel Sell:"), _T("0.00"));
	m_pPropStockCancelSell->Enable(FALSE);
	m_pChinaMarketStock->AddSubItem(m_pPropStockCancelSell);

	m_wndPropList.AddProperty(m_pChinaMarketStock);
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
	auto pStock = gl_pChinaMarket->GetCurrentStock();

	if (pStock != nullptr) {
		CString symbol = pStock->GetSymbol();
		m_pChinaMarketStock->SetName(symbol);
		if (pStock->GetVolume() > 0) {
			char buffer[100];
			sprintf_s(buffer, 100, "%ld", pStock->GetNew());
			CString str = buffer;
			m_pPropNew->SetValue(str);
			INT64 volume = pStock->GetVolume();
			sprintf_s(buffer, 100, "%lld", volume);
			str = buffer;
			m_pPropVolume->SetValue(str);
			sprintf_s(buffer, 100, "0.%02lld", pStock->GetOrdinaryBuyVolume() * 100 / volume);
			str = buffer;
			m_pPropStockOrdinaryBuy->SetValue(str);
			sprintf_s(buffer, 100, _T("0.%02lld"), pStock->GetOrdinarySellVolume() * 100 / volume);
			str = buffer;
			m_pPropStockOrdinarySell->SetValue(str);

			sprintf_s(buffer, 100, "0.%02lld", pStock->GetAttackBuyVolume() * 100 / volume);
			str = buffer;
			m_pPropStockAttackBuy->SetValue(str);
			sprintf_s(buffer, 100, _T("0.%02lld"), pStock->GetAttackSellVolume() * 100 / volume);
			str = buffer;
			m_pPropStockAttackSell->SetValue(str);

			sprintf_s(buffer, 100, "0.%02lld", pStock->GetStrongBuyVolume() * 100 / volume);
			str = buffer;
			m_pPropStockStrongBuy->SetValue(str);
			sprintf_s(buffer, 100, _T("0.%02lld"), pStock->GetStrongSellVolume() * 100 / volume);
			str = buffer;
			m_pPropStockStrongSell->SetValue(str);

			sprintf_s(buffer, 100, "0.%02lld", pStock->GetCanceledBuyVolume() * 100 / volume);
			str = buffer;
			m_pPropStockCancelBuy->SetValue(str);
			sprintf_s(buffer, 100, _T("0.%02lld"), pStock->GetCanceledSellVolume() * 100 / volume);
			str = buffer;
			m_pPropStockCancelSell->SetValue(str);
		}
	}
	else {
		m_pChinaMarketStock->SetName(_T(""));
	}

	CDockablePane::OnTimer(nIDEvent);
}
