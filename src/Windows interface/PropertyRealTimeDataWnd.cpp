#include "pch.h"
#include "framework.h"

#include "PropertyRealTimeDataWnd.h"

#include "ChinaMarket.h"
#include "MainFrm.h"
#include "FireBird.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

////////////////////////////////////////////////////////////////////////////////////
//
// Configuration实时改变处理函数
//
////////////////////////////////////////////////////////////////////////////////////
void CFireBirdPropertyRealtimeGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const {
	COleVariant value;
	LPVARIANT pVar;
	CStringW wStr;
	CString str;
	value = pProp->GetValue();
	pVar = static_cast<LPVARIANT>(value);
	switch (pProp->GetData()) {
	default:
		TRACE("未处理PropertyGridCtrl例外\n"); // 未处理例外
	//ASSERT(0);
		break;
	}
	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

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

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("China market"));

	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), static_cast<_variant_t>(true), _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);
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
	CDockablePane::OnTimer(nIDEvent);
}
