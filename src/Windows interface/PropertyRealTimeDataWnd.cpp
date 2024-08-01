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
	case SYSTEM_LOG_LEVEL_:
		ASSERT(pVar->vt == VT_BSTR);
		wStr = pVar->bstrVal;
		str = wStr;
		switch (str.GetAt(0)) {
		case 'T': // trace
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_TRACE);
			break;
		case 'D': // debug
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_DEBUG);
			break;
		case 'I': // info
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_INFO);
			break;
		case 'W': // warn
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_WARN);
			break;
		case 'E': // error
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_ERROR);
			break;
		case 'C': // critical
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_CRITICAL);
			break;
		case 'O': // OFF
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_OFF);
			break;
		default:
			gl_systemConfiguration.SetLogLevel(SPDLOG_LEVEL_INFO);
		}
		gl_systemConfiguration.SetUpdate(true);
		break;
	case SYSTEM_DEBUG_MODE_:
		ASSERT(pVar->vt == VT_BOOL);
		gl_systemConfiguration.SetDebugMode(pVar->boolVal);
		gl_systemConfiguration.SetUpdate(true);
		break;
	case SYSTEM_RELOAD_SYSTEM_:
		ASSERT(pVar->vt == VT_BOOL);
		gl_systemConfiguration.SetReloadSystem(pVar->boolVal);
		gl_systemConfiguration.SetUpdate(true);
		break;
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

	m_pPropSystemOption = nullptr;
}

CPropertyRealtimeWnd::~CPropertyRealtimeWnd() {
}

BEGIN_MESSAGE_MAP(CPropertyRealtimeWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
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

	const int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
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

	// Create ToolBar
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

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

void CPropertyRealtimeWnd::OnExpandAllProperties() {
	m_wndPropList.ExpandAll();
}

void CPropertyRealtimeWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */) {
}

void CPropertyRealtimeWnd::OnSortProperties() {
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertyRealtimeWnd::OnUpdateSortProperties(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertyRealtimeWnd::OnProperties1() {
}

void CPropertyRealtimeWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/) {
}

void CPropertyRealtimeWnd::OnProperties2() {
}

void CPropertyRealtimeWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/) {
}

void CPropertyRealtimeWnd::InitPropList() {
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("System Option"));
	CString s;
	switch (gl_systemConfiguration.GetLogLevel()) {
	case SPDLOG_LEVEL_TRACE:
		s = _T("Trace");
		break;
	case SPDLOG_LEVEL_DEBUG:
		s = _T("Debug");
		break;
	case SPDLOG_LEVEL_INFO:
		s = _T("Info");
		break;
	case SPDLOG_LEVEL_WARN:
		s = _T("Warn");
		break;
	case SPDLOG_LEVEL_ERROR:
		s = _T("Error");
		break;
	case SPDLOG_LEVEL_CRITICAL:
		s = _T("Critical");
		break;
	case SPDLOG_LEVEL_OFF:
		s = _T("Off");
		break;
	default:
		s = _T("Info");
	}
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Log level"), s, _T("One of: Trace, Debug, Info, Warn, Error, Critical, or Off"), SYSTEM_LOG_LEVEL_);
	pProp->AddOption(_T("Trace"));
	pProp->AddOption(_T("Debug"));
	pProp->AddOption(_T("Info"));
	pProp->AddOption(_T("Warn"));
	pProp->AddOption(_T("Error"));
	pProp->AddOption(_T("Critical"));
	pProp->AddOption(_T("Off"));
	pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Debug Mode"), static_cast<_variant_t>(gl_systemConfiguration.IsDebugMode()), _T("Debug mode"), SYSTEM_DEBUG_MODE_));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Reload System"), static_cast<_variant_t>(gl_systemConfiguration.IsReloadSystem()), _T("Reload System"), SYSTEM_RELOAD_SYSTEM_));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("China market"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle(static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), static_cast<_variant_t>(true), _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("World market"));

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), nullptr, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static constexpr TCHAR szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup5 = new CMFCPropertyGridProperty(_T("Web Socket"));

	m_wndPropList.AddProperty(pGroup5);
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
