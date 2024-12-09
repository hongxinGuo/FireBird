#include "pch.h"

#include"globedef.h"

#include "framework.h"
#include"SystemMessage.h"

#include "ChinaMarket.h"
#include "FinnhubDataSource.h"
#include "FireBird.h"

#include "TiingoDataSource.h"
#include "WorldMarket.h"

#include <spdlog/common.h>
import FireBird.System.ThreadStatus;
import FireBird.WindowInterface.MainFrm;
module FireBird.WindowInterface.PropertiesWnd; 


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
void CFireBirdPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const {
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
		gl_systemConfiguration.SetUpdateDB(true);
		break;
	case SYSTEM_DEBUG_MODE_:
		ASSERT(pVar->vt == VT_BOOL);
		gl_systemConfiguration.SetDebugMode(pVar->boolVal);
		gl_systemConfiguration.SetUpdateDB(true);
		break;
	case SYSTEM_RELOAD_SYSTEM_:
		ASSERT(pVar->vt == VT_BOOL);
		gl_systemConfiguration.SetReloadSystem(pVar->boolVal);
		gl_systemConfiguration.SetUpdateDB(true);
		break;
	case FINNHUB_DATA_SOURCE_ENABLE_:
		ASSERT(pVar->vt == VT_BOOL);
		gl_pFinnhubDataSource->Enable(pVar->boolVal);
		break;
	case TIINGO_DATA_SOURCE_ENABLE_:
		ASSERT(pVar->vt == VT_BOOL);
		gl_pTiingoDataSource->Enable(pVar->boolVal);
		break;
	default:
		TRACE("未处理PropertyGridCtrl例外\n"); // 未处理例外
	//ASSERT(0);
		break;
	}
	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

BEGIN_MESSAGE_MAP(CFireBirdComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CFireBirdComboBox::OnCbnSelChange)
END_MESSAGE_MAP()

void CFireBirdComboBox::OnCbnSelChange() {
	if (gl_systemConfiguration.GetDisplayPropertyPage() != GetCurSel()) {
		gl_systemConfiguration.SetDisplayPropertyPage(GetCurSel());
		gl_systemConfiguration.SetUpdateDB(true);
	}
}

//
// CPropertyWnd
//
CPropertiesWnd::CPropertiesWnd() noexcept {
}

CPropertiesWnd::~CPropertiesWnd() {
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
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

void CPropertiesWnd::AdjustLayout() {
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic())) {
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	const int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight + 190, SWP_NOACTIVATE | SWP_NOZORDER); // Note 设置高度时需要留出显示菜单的空间，每个选项高度为30.默认三个选项，故+90
	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	constexpr DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1)) {
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	CString str;
	str.LoadString(nullptr, IDS_PROPERTYVIEW_SYSTEM_STATUS);
	m_wndObjectCombo.AddString(str);
	str.LoadString(nullptr, IDS_PROPERTYVIEW_CHINA_MARKET_REALTIME);
	m_wndObjectCombo.AddString(str);
	str.LoadString(nullptr, IDS_PROPERTYVIEW_PROPERTIES2);
	m_wndObjectCombo.AddString(str);
	m_wndObjectCombo.SetCurSel(gl_systemConfiguration.GetDisplayPropertyPage());

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect(&rectCombo);

	m_nComboHeight = rectCombo.Height();

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

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy) {
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties() {
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */) {
}

void CPropertiesWnd::OnSortProperties() {
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1() {
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/) {
}

void CPropertiesWnd::OnProperties2() {
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/) {
}

void CPropertiesWnd::InitPropList() {
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("System Status"));
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

	m_pPropThreadPoolExecutorMaxWorkerIdleTime = new CMFCPropertyGridProperty(_T("Thread pool idle time:"), _T(""));
	m_pPropThreadPoolExecutorMaxWorkerIdleTime->Enable(false);
	pGroup1->AddSubItem(m_pPropThreadPoolExecutorMaxWorkerIdleTime);
	m_pPropBackgroundExecutorMaxWorkerIdleTime = new CMFCPropertyGridProperty(_T("Background idle time:"), _T(""));
	m_pPropBackgroundExecutorMaxWorkerIdleTime->Enable(false);
	pGroup1->AddSubItem(m_pPropBackgroundExecutorMaxWorkerIdleTime);
	m_pPropCurrentWorkingThread = new CMFCPropertyGridProperty(_T("working thread:"), _T(""));
	m_pPropCurrentWorkingThread->Enable(false);
	pGroup1->AddSubItem(m_pPropCurrentWorkingThread);
	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("China market"));

	m_pPropChinaMarketWebStatus = new CMFCPropertyGridColorProperty(_T("Web Status"), RGB(0, 192, 0), nullptr);
	m_pPropChinaMarketWebStatus->SetColumnsNumber(1);
	m_pPropChinaMarketWebStatus->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE), false);
	m_pPropChinaMarketWebStatus->Enable(false);
	pGroup2->AddSubItem(m_pPropChinaMarketWebStatus);

	m_wndPropList.AddProperty(pGroup2);

	// finnhub group
	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("finnhub.io"));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Enable Data Source"), static_cast<_variant_t>(gl_pFinnhubDataSource->IsEnable()), _T("Enable"), FINNHUB_DATA_SOURCE_ENABLE_));
	m_pPropWorldMarketWebStatus = new CMFCPropertyGridProperty(_T("Web Status"), _T("running"));
	m_pPropWorldMarketWebStatus->Enable(FALSE);
	pGroup3->AddSubItem(m_pPropWorldMarketWebStatus);
	m_pPropFinnhubCurrentFunction = new CMFCPropertyGridProperty(_T("Inquiring:"), _T(""));
	m_pPropFinnhubCurrentFunction->Enable(FALSE);
	pGroup3->AddSubItem(m_pPropFinnhubCurrentFunction);
	m_wndPropList.AddProperty(pGroup3);

	// tiingo group
	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Tiingo.com"));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Enable Data Source"), static_cast<_variant_t>(gl_pTiingoDataSource->IsEnable()), _T("Enable"), TIINGO_DATA_SOURCE_ENABLE_));
	m_pPropTiingoCurrentFunction = new CMFCPropertyGridProperty(_T("Inquiring:"), _T(""));
	m_pPropTiingoCurrentFunction->Enable(FALSE);
	pGroup4->AddSubItem(m_pPropTiingoCurrentFunction);
	m_wndPropList.AddProperty(pGroup4);

	// web socket group
	CMFCPropertyGridProperty* pGroup5 = new CMFCPropertyGridProperty(_T("Web Socket"));
	m_pPropFinnhubWebSocket = new CMFCPropertyGridProperty(_T("Finnhub"), _T("Closed"));
	m_pPropFinnhubWebSocket->Enable(false);
	pGroup5->AddSubItem(m_pPropFinnhubWebSocket);
	m_pPropTiingoIEXWebSocket = new CMFCPropertyGridProperty(_T("TiingoIEX"), _T("Closed"));
	m_pPropTiingoIEXWebSocket->Enable(false);
	pGroup5->AddSubItem(m_pPropTiingoIEXWebSocket);
	m_pPropTiingoForexWebSocket = new CMFCPropertyGridProperty(_T("TiingoForex"), _T("Closed"));
	m_pPropTiingoForexWebSocket->Enable(false);
	pGroup5->AddSubItem(m_pPropTiingoForexWebSocket);
	m_pPropTiingoCryptoWebSocket = new CMFCPropertyGridProperty(_T("TiingoCrypto"), _T("Closed"));
	m_pPropTiingoCryptoWebSocket->Enable(false);
	pGroup5->AddSubItem(m_pPropTiingoCryptoWebSocket);

	m_wndPropList.AddProperty(pGroup5);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd) {
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) {
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont() {
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
	m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CPropertiesWnd::OnTimer(UINT_PTR nIDEvent) {
	char buffer[100];
	CString str;
	auto threadPooExecutorMaxIdleTime = gl_runtime.thread_pool_executor()->max_worker_idle_time();
	auto l = threadPooExecutorMaxIdleTime.count();
	sprintf_s(buffer, _T("%d"), l);
	str = buffer;
	m_pPropThreadPoolExecutorMaxWorkerIdleTime->SetValue(str);
	auto backgroundExecutorMaxIdleTime = gl_runtime.background_executor()->max_worker_idle_time();
	l = backgroundExecutorMaxIdleTime.count();
	sprintf_s(buffer, _T("%d"), l);
	str = buffer;
	m_pPropBackgroundExecutorMaxWorkerIdleTime->SetValue(str);

	sprintf_s(buffer, _T("%d"), gl_ThreadStatus.GetNumberOfBackGroundWorkingThread());
	str = buffer;
	m_pPropCurrentWorkingThread->SetValue(str); // 后台工作线程数

	if (gl_pChinaMarket->IsWebBusy()) m_pPropChinaMarketWebStatus->SetColor(RGB(192, 0, 0));
	else m_pPropChinaMarketWebStatus->SetColor(RGB(0, 192, 0));

	if (gl_pChinaMarket->IsWebBusy()) {
		m_pPropWorldMarketWebStatus->SetValue(_T("Disabled"));
	}
	else {
		if (gl_pFinnhubDataSource->IsWebError()) {
			char buffer2[100];
			sprintf_s(buffer2, _T("running (EC:%5zd)"), gl_pFinnhubDataSource->GetWebErrorCode());
			CString str3 = buffer2;
			m_pPropWorldMarketWebStatus->SetValue(str3);
		}
		else {
			m_pPropWorldMarketWebStatus->SetValue(_T("running"));
		}
	}

	CString strMessage = gl_systemMessage.GetCurrentFinnhubFunction();
	m_pPropFinnhubCurrentFunction->SetValue(strMessage);

	strMessage = gl_systemMessage.GetCurrentTiingoFunction();
	m_pPropTiingoCurrentFunction->SetValue(strMessage);

	str = _T("");
	switch (gl_pFinnhubWebSocket->GetState()) {
	case ix::ReadyState::Closed:
		str = _T("Closed");
		break;
	case ix::ReadyState::Closing:
		str = _T("Closing");
		break;
	case ix::ReadyState::Connecting:
		str = _T("Connecting");
		break;
	case ix::ReadyState::Open:
		str = _T("Open");
		break;
	}
	m_pPropFinnhubWebSocket->SetValue(str);

	switch (gl_pTiingoIEXWebSocket->GetState()) {
	case ix::ReadyState::Closed:
		str = _T("Closed");
		break;
	case ix::ReadyState::Closing:
		str = _T("Closing");
		break;
	case ix::ReadyState::Connecting:
		str = _T("Connecting");
		break;
	case ix::ReadyState::Open:
		str = _T("Open");
		break;
	}
	m_pPropTiingoIEXWebSocket->SetValue(str);

	switch (gl_pTiingoForexWebSocket->GetState()) {
	case ix::ReadyState::Closed:
		str = _T("Closed");
		break;
	case ix::ReadyState::Closing:
		str = _T("Closing");
		break;
	case ix::ReadyState::Connecting:
		str = _T("Connecting");
		break;
	case ix::ReadyState::Open:
		str = _T("Open");
		break;
	}
	m_pPropTiingoForexWebSocket->SetValue(str);

	switch (gl_pTiingoCryptoWebSocket->GetState()) {
	case ix::ReadyState::Closed:
		str = _T("Closed");
		break;
	case ix::ReadyState::Closing:
		str = _T("Closing");
		break;
	case ix::ReadyState::Connecting:
		str = _T("Connecting");
		break;
	case ix::ReadyState::Open:
		str = _T("Open");
		break;
	}
	m_pPropTiingoCryptoWebSocket->SetValue(str);

	CDockablePane::OnTimer(nIDEvent);
}
