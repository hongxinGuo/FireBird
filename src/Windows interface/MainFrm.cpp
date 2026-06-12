// MainFrm.cpp: CMainFrame 类的实现
#include"pch.h"

#include"SystemPublicDeclaration.h"

#include"ThreadStatus.h"
#include"Thread.h"

#include "FireBird.h"
#include "MainFrm.h"
#include "FireBirdDoc.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"
#include"WorldMarket.h"

#include"SinaRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"NeteaseRTDataSource.h"
#include"NeteaseDayLineDataSource.h"
#include"TengxunDayLineDataSource.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"
#include"QuandlDataSource.h"

#include <ixwebsocket/IXNetSystem.h>

#include "TimeConvert.h"

#include"Initialization.h"
#include "InfoReport.h"

#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

bool CMainFrame::sm_fGlobeInit = false;
constexpr int iMaxUserToolbars = 10;
constexpr UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
constexpr UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

IMPLEMENT_DYNCREATE(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PROCESS_TODAY_STOCK, &CMainFrame::OnProcessTodayStock)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_TODAY_STOCK, &CMainFrame::OnUpdateProcessTodayStock)
	ON_WM_SYSCOMMAND()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_COMMAND(ID_STOP_UPDATE_DAYLINE, &CMainFrame::OnStopUpdateDayLine)
	ON_COMMAND(ID_USING_NETEASE_REALTIME_DATA_SERVER, &CMainFrame::OnUsingNeteaseRealtimeDataServer)
	ON_COMMAND(ID_USING_SINA_REALTIME_DATA_SERVER, &CMainFrame::OnUsingSinaRealtimeDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_NETEASE_REALTIME_DATA_SERVER, &CMainFrame::OnUpdateUsingNeteaseRealtimeDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_SINA_REALTIME_DATA_SERVER, &CMainFrame::OnUpdateUsingSinaRealtimeDataServer)
	ON_COMMAND(ID_UPDATE_SECTION_INDEX, &CMainFrame::OnUpdateStockSection)
	ON_COMMAND(ID_UPDATE_STOCK_CODE, &CMainFrame::OnUpdateStockProfile)
	ON_COMMAND(ID_REBUILD_EPS_SURPRISE, &CMainFrame::OnRebuildEpsSurprise)
	ON_COMMAND(ID_REBUILD_PEER, &CMainFrame::OnRebuildPeer)
	ON_COMMAND(ID_REBUILD_DAYLINE, &CMainFrame::OnRebuildDayLine)
	ON_COMMAND(ID_RECORD_FINNHUB_WEB_SOCKET, &CMainFrame::OnRecordFinnhubWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_FINNHUB_WEB_SOCKET, &CMainFrame::OnUpdateRecordFinnhubWebSocket)
	ON_COMMAND(ID_RECORD_TIINGO_CRYPTO_WEB_SOCKET, &CMainFrame::OnRecordTiingoCryptoWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_TIINGO_CRYPTO_WEB_SOCKET, &CMainFrame::OnUpdateRecordTiingoCryptoWebSocket)
	ON_COMMAND(ID_RECORD_TIINGO_FOREX_WEB_SOCKET, &CMainFrame::OnRecordTiingoForexWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_TIINGO_FOREX_WEB_SOCKET, &CMainFrame::OnUpdateRecordTiingoForexWebSocket)
	ON_COMMAND(ID_RECORD_TIINGO_IEX_WEB_SOCKET, &CMainFrame::OnRecordTiingoIEXWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_TIINGO_IEX_WEB_SOCKET, &CMainFrame::OnUpdateRecordTiingoIEXWebSocket)
	ON_COMMAND(ID_REBUILD_BASIC_FINANCIAL, &CMainFrame::OnRebuildBasicFinancial)
	ON_COMMAND(ID_MAINTAIN_DAYLINE, &CMainFrame::OnMaintainChinaMarketStockDayLine)
	ON_UPDATE_COMMAND_UI(ID_MAINTAIN_DAYLINE, &CMainFrame::OnUpdateMaintainChinaMarketStockDayLine)
	ON_WM_SIZE()
	ON_COMMAND(ID_USING_NETEASE_DAYLINE_DATA_SERVER, &CMainFrame::OnUsingNeteaseDayLineDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_NETEASE_DAYLINE_DATA_SERVER, &CMainFrame::OnUpdateUsingNeteaseDayLineDataServer)
	ON_COMMAND(ID_USING_TENGXUN_DAYLINE_DATA_SERVER, &CMainFrame::OnUsingTengxunDayLineDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_TENGXUN_DAYLINE_DATA_SERVER, &CMainFrame::OnUpdateUsingTengxunDayLineDataServer)
	ON_COMMAND(ID_USING_TENGXUN_REALTIME_DATA_SERVER, &CMainFrame::OnUsingTengxunRealtimeDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_TENGXUN_REALTIME_DATA_SERVER, &CMainFrame::OnUpdateUsingTengxunRealtimeDataServer)
	ON_COMMAND(ID_UPDATE_TIINGO_FUNDAMENTAL_DEFINITION, &CMainFrame::OnUpdateTiingoFundamentalDefinition)
	ON_UPDATE_COMMAND_UI(ID_UPDATE_TIINGO_FUNDAMENTAL_DEFINITION, &CMainFrame::OnUpdateUpdateTiingoFundamentalDefinition)
	ON_COMMAND(ID_RESET_TIINGO_DAYLINE_DATE, &CMainFrame::OnResetTiingoDayLineDate)
	ON_COMMAND(ID_CREATE_TIINGO_TRADEDAY_DAYLINE, &CMainFrame::OnCreateTiingoTradeDayDayLine)
	ON_UPDATE_COMMAND_UI(ID_CREATE_TIINGO_TRADEDAY_DAYLINE, &CMainFrame::OnUpdateCreateTiingoTradeDayDayLine)
	ON_COMMAND(ID_PROCESS_TIINGO_DAYLINE, &CMainFrame::OnProcessTiingoDayLine)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_TIINGO_DAYLINE, &CMainFrame::OnUpdateProcessTiingoDayLine)
	ON_COMMAND(ID_CALCULATE_NEW_LOW_FIVE_TIMES, &CMainFrame::OnCalculateNewLowFiveTimes)
	ON_COMMAND(ID_INQUIRE_IEX_TOP_OF_BOOK, &CMainFrame::OnInquireIexTopOfBook)
	ON_COMMAND(ID_CalculateNasdaq100_200MA_UpDownRate, &CMainFrame::OnCalculateNasdaq100200maUpdownRate)
	ON_UPDATE_COMMAND_UI(ID_CalculateNasdaq100_200MA_UpDownRate, &CMainFrame::OnUpdateCalculateNasdaq100200maUpdownRate)
	ON_COMMAND(ID_TIINGO_REBUILD_STOCK_SPLIT, &CMainFrame::OnTiingoRebuildStockSplit)
	ON_COMMAND(ID_TIINGO_DOWNLOAD_ALL_DAYLINE, &CMainFrame::OnTiingoDownloadAllDayline)
	ON_COMMAND(ID_TIINGO_DOWNLOAD_ONE_YEAR_DAYLINE, &CMainFrame::OnTiingoDownloadOneYearDayline)
	ON_COMMAND(ID_BUILD_CHINA_STOCK_ONE_YEAR_DAYLINE, &CMainFrame::OnBuildChinaStockOneYearDayline)
	ON_COMMAND(ID_BUILD_CHINA_MARKET_ALL_STOCK_DAYLINE, &CMainFrame::OnBuildChinaMarketAllStockDayline)
END_MESSAGE_MAP()

namespace {
	UINT indicators[] =
	{
		ID_SEPARATOR, // 状态行指示器
		ID_CURRENT_INPUT,
		ID_CURRENT_SELECT_STOCK,
		ID_CURRENT_SELECT_STOCKNAME,
		ID_CURRENT_SELECT_STOCK_SET,
		ID_CURRENT_SELECT_POSITION,
		ID_CURRENT_RTDATA_READING_STOCK,
		ID_CURRENT_ACTIVE_STOCK,
		ID_CURRENT_DAYLINE_READING_STOCK,
		ID_CURRENT_FUNCTION,
		ID_CURRENT_TIINGO_FUNCTION,
		ID_CURRENT_RTDATA_SIZE,
		ID_CURRENT_RUNNING_THREAD,
		ID_CURRENT_RUNNING_BACKGROUND_THREAD,
		ID_CURRENT_TIME,
	};

	UINT innerSystemIndicators[] =
	{
		ID_SEPARATOR, // 状态行指示器
		ID_SHOW_SINA_RT,
		ID_SHOW_NETEASE_DAYLINE,
		ID_SHOW_NETEASE_RT,
		ID_SHOW_TENGXUN_RT,
		ID_SHOW_FINNHUB_WEB,
		ID_SHOW_TIINGO_WEB,
		ID_SHOW_QUANDL_WEB,
		ID_CURRENT_FINNHUB_STAKE,
		ID_SHOW_FINNHUB_WEBSOCKET_DATASIZE,
		ID_CURRENT_TIINGO_IEX,
		ID_SHOW_TIINGO_IEX_WEBSOCKET_DATASIZE,
		ID_CURRENT_TIINGO_FOREX,
		ID_SHOW_TIINGO_CRYPTO_WEBSOCKET_DATASIZE,
		ID_CURRENT_TIINGO_CRYPTO,
		ID_SHOW_TIINGO_FOREX_WEBSOCKET_DATASIZE,
		ID_SHOW_TIINGO_TOTAL_DATASIZE,
		ID_SHOW_FINNHUB_TOTAL_DATASIZE,
		ID_SHOW_CHINA_MARKET_TOTAL_DATASIZE,
	};
}

// CMainFrame 构造/析构

CMainFrame::CMainFrame() {
	if (!sm_fGlobeInit) {
		sm_fGlobeInit = true;
		ix::initNetSystem();// 在Windows环境下，IXWebSocket库需要初始化一次，且只能初始化一次。
	}
	else {
		ix::initNetSystem();// 在Windows环境下，IXWebSocket库需要初始化一次，且只能初始化一次。
	}

	// 默认下后台工作线程数为32，使用系统配置降低至实际数量。
	for (int i = 0; i < MAX_BACKGROUND_WORKING_THREAD_ - gl_systemConfiguration.GetBackgroundThreadPermittedNumber(); i++) {
		gl_BackgroundWorkingThread.acquire();
	}
}

CMainFrame::~CMainFrame() {
	if (!gl_systemConfiguration.IsWorkingMode())
		TRACE(_T("使用了Test驱动\n"));

	gl_systemConfiguration.SetExitingSystem(true);

	for (int i = 0; i < MAX_BACKGROUND_WORKING_THREAD_ - gl_systemConfiguration.GetBackgroundThreadPermittedNumber(); i++) {
		gl_BackgroundWorkingThread.release();
	}

	if (gl_hFireBirdMutex != nullptr) {
		::CloseHandle(gl_hFireBirdMutex); //Note 采用显式关闭。偶尔程序出现无法再次启动的现象（Mutex未关闭）。
		gl_hFireBirdMutex = nullptr;
	}

	if (sm_fGlobeInit) {
		sm_fGlobeInit = false;
		ix::uninitNetSystem();// 退出系统时，析构IXWebSocket库，且只能析构一次。
	}
	else {
		ix::uninitNetSystem();// 退出系统时，析构IXWebSocket库，且只能析构一次。
	}

	if (gl_pChinaMarket->IsUpdateOptionDB()) {
		gl_pChinaMarket->UpdateOptionDB();
	}

	if (gl_pChinaMarket->IsUpdateChosenStockDB()) {
		gl_pChinaMarket->UpdateChosenStockDB(); // 这里直接调用存储函数，不采用工作线程的模式。
	}

	// 更新股票代码数据库要放在最后，等待存储日线数据的线程（如果唤醒了的话）结束之后再执行。
	// 因为彼线程也在更新股票代码数据库，而此更新只是消除同类项而已。
	if (gl_dataContainerChinaStock.IsUpdateProfileDB()) {
		gl_dataContainerChinaStock.UpdateProfileDB(); // 这里直接调用存储函数，不采用工作线程的模式。
	}

	while (gl_ThreadStatus.IsBackGroundThreadsWorking()) Sleep(1); // 等待后台工作线程运行结束

	TRACE("exit finally \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此函数最后时执行系统初始化的各项任务，并设置系统调度任务
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	// 系统初始化开始
	gl_systemMessage.PushInformationMessage("系统初始化中.....");
	::SystemInitialization();
	gl_systemMessage.PushInformationMessage("重置系统");

	// 创建FireBird调度器互斥量
	gl_MsgFireBirdRunning = RegisterWindowMessageW(gl_wsFireBirdRunning.c_str());
	gl_MsgFireBirdExit = RegisterWindowMessageW(gl_wsFireBirdExit.c_str());
	gl_MsgFireBirdSchedulingExit = RegisterWindowMessageW(gl_wsFireBirdSchedulingExit.c_str());
	gl_MsgFireBirdCheckRunningExit = RegisterWindowMessageW(gl_wsFireBirdCheckRunningExit.c_str());

	// 生成系统外观显示部件
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE; // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE; // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE; // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this)) {
		TRACE0("未能创建菜单栏\n");
		return -1; // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,
	                           WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |
	                           CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME)) {
		TRACE0("Failed to create toolbar\n");
		return -1; // fail to create
	}

	CString strToolBarName;
	ASSERT(strToolBarName.LoadString(IDS_TOOLBAR_STANDARD));
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	ASSERT(strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE));
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this)) {
		TRACE0("Failed to create status bar\n");
		return -1; // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	if (!m_wndInnerSystemBar.Create(this)) {
		TRACE0("未能创建状态栏\n");
		return -1; // 未能创建
	}
	m_wndInnerSystemBar.SetIndicators(innerSystemIndicators, sizeof(innerSystemIndicators) / sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 创建停靠窗口
	if (!CreateDockingWindows()) {
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_BOTTOM);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	m_wndPropertyRealTime.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndPropertyRealTime);

	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr) {
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp"))) {
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);

	// 设置500毫秒每次的软调度，只用于更新状态任务。
	m_uIdTimer = SetTimer(1, 500, nullptr);
	if (m_uIdTimer == 0) {
		TRACE(_T("生成500ms时钟时失败\n"));
	}

	// 更新系统显示高度和宽度
	gl_systemConfiguration.SetSystemDisplayRect(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
	gl_systemConfiguration.SetCurrentWindowRect(GetSystemMetrics(SM_CXMAXIMIZED), GetSystemMetrics(SM_CYMAXIMIZED));

	ReportRunningToWatchdog(); // report to watchdog
	return 0;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons) {
	const auto hOutputBarIcon = static_cast<HICON>(::LoadImage(AfxGetResourceHandle(),
	                                                           MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND),
	                                                           IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),
	                                                           GetSystemMetrics(SM_CYSMICON), 0));
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);
	HICON hPropertiesBarIcon = static_cast<HICON>(::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0));
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
	m_wndPropertyRealTime.SetIcon(hPropertiesBarIcon, FALSE);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CMDIFrameWndEx::PreCreateWindow(cs)) return FALSE;
	// TODO: 在此处通过修改CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows() {
	// 创建输出窗口
	CString strOutputWnd;
	ASSERT(strOutputWnd.LoadString(IDS_OUTPUT_WND));
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND,
	                        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) {
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	// Create properties window
	CString strPropertiesWnd;
	ASSERT(strPropertiesWnd.LoadString(IDS_PROPERTIES_WND));
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) {
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	// Create Realtime property window
	CString strPropertyRealtimeWnd;
	ASSERT(strPropertyRealtimeWnd.LoadString(IDS_PROPERTY_REALTIME_WND));
	if (!m_wndPropertyRealTime.Create(strPropertyRealtimeWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTY_REALTIME_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) {
		TRACE0("Failed to create Property realtime window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager() {
	//ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize() {
	auto pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp) {
	const LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0) { return 0; }

	const auto pUserToolbar = reinterpret_cast<CMFCToolBar*>(lres);
	ASSERT_VALID(pUserToolbar);

	CString strCustomize;
	ASSERT(strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE));

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
	// 基类将执行真正的工作

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext)) { return FALSE; }

	CString strCustomize;
	ASSERT(strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE));

	for (int i = 0; i < iMaxUserToolbars; i++) {
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr) { pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize); }
	}

	return TRUE;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) {
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//
// CMainFrame timer只执行更新状态任务， 其他的定时数据采集处理任务由TaskSchedulePer100ms()负责执行。
// Note 注意同步问题。
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnTimer(UINT_PTR nIDEvent) {
	//::TaskSchedulePer100ms(); // 系统更新任务皆位于此函数中

	// 在窗口显示系统状态的更新任务放在这里比较合适。可以减少窗口句柄问题
	if (!IsMarketResetting()) {
		UpdateStatus();
		UpdateInnerSystemStatus();
	}

	SysCallOnTimer(nIDEvent);
}

void CMainFrame::UpdateStatus() {
	ASSERT(!IsMarketResetting());

	//更新状态条
	if (IsCurrentEditStockChanged()) {
		CStringW strW = m_aStockCodeTemp;
		SysCallSetPaneText(1, strW);
		SetCurrentEditStockChanged(false);
	}
	// 显示股票代码和名称
	if (gl_pCurrentStock != nullptr) {
		SysCallSetPaneText(2, gl_pCurrentStock->GetSymbol());
		SysCallSetPaneText(3, gl_pCurrentStock->GetDisplaySymbol());//Note 股票名称已经是UTF8编码，直接调用SysCallSetPaneText()就可以。
	}

	// 显示当前选择的股票
	string s = std::format("{:Ld}", gl_pChinaMarket->GetCurrentSelectedStockSet());
	SysCallSetPaneText(4, s);

	// 显示当前选择的位置
	s = std::format("{:Ld}", gl_pChinaMarket->GetCurrentSelectedPosition());
	SysCallSetPaneText(5, s);

	// 显示当前读取的新浪实时数据股票代码
	SysCallSetPaneText(6, gl_systemMessage.GetStockCodeForInquiringRTData());

	// 显示活跃股票总数
	s = std::format("{:Ld}", gl_dataContainerChinaStock.GetActiveStockSize());
	SysCallSetPaneText(7, s);

	// 显示当前读取网易日线历史的股票代码
	SysCallSetPaneText(8, gl_systemMessage.GetStockCodeForInquiryDayLine());

	// 
	SysCallSetPaneText(9, gl_systemMessage.GetCurrentFinnhubFunction());

	SysCallSetPaneText(10, gl_systemMessage.GetCurrentTiingoFunction());

	// 更新当前抓取的实时数据大小
	SysCallSetPaneText(11, FormatToMK(gl_pSinaRTDataSource->GetTotalByteReadPerSecond()));

	// 更新当前申请网络数据的工作线程数
	gl_systemMessage.SetChinaMarketSavingFunction("");
	SysCallSetPaneText(12, "");

	gl_systemMessage.SetWorldMarketSavingFunction("");
	SysCallSetPaneText(13, "");

	// 更新当前后台工作线程数
	//s = std::format("{:d}", gl_ThreadStatus.GetNumberOfBackGroundWorkingThread());
	//SysCallSetPaneText(13, s.c_str());

	//更新当地时间的显示
	SysCallSetPaneText(14, gl_pChinaMarket->GetStringOfLocalTime());
}

void CMainFrame::UpdateInnerSystemStatus() {
	string s;
	// 更新实时数据读取时间
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case SinaRealTime_: // 新浪实时数据
		s = std::format("{:5Ld}", gl_pSinaRTDataSource->GetCurrentInquiryTime());
		break;
	case NeteaseRealTime_: // 更新网易实时数据读取时间
		s = std::format("{:5Ld}", gl_pNeteaseRTDataSource->GetCurrentInquiryTime());
		break;
	case TengxunRealTime_: // 更新腾讯实时数据读取时间
		s = std::format("{:5Ld}", gl_pTengxunRTDataSource->GetCurrentInquiryTime());
		break;
	default: // error
		break;
	}
	SysCallSetInnerSystemPaneText(1, s);

	// 更新实时数据分配及处理时间
	s = std::format("{:5Ld}", gl_pChinaMarket->m_ttDistributeAndCalculateTime.load());
	SysCallSetInnerSystemPaneText(2, s);

	// 更新TaskSchedulePer100ms()处理时间
	const long time = gl_systemMessage.GetScheduleTaskTimePerSecond() / 1000;
	s = std::format("{:5Ld}", time);
	SysCallSetInnerSystemPaneText(3, s);

	// 更新日线数据读取时间
	if (gl_systemConfiguration.IsUsingNeteaseDayLineServer()) { // 网易日线服务器
		s = std::format("{:5Ld}", gl_pNeteaseDayLineDataSource->GetCurrentInquiryTime());
	}
	else if (gl_systemConfiguration.IsUsingTengxunDayLineServer()) { // 腾讯日线服务器
		s = std::format("{:5Ld}", gl_pTengxunDayLineDataSource->GetCurrentInquiryTime());
	}
	SysCallSetInnerSystemPaneText(4, s);

	// 更新Finnhub数据读取时间
	s = std::format("{:5Ld}", gl_pFinnhubDataSource->GetCurrentInquiryTime());
	SysCallSetInnerSystemPaneText(5, s);
	// 更新Tiingo数据读取时间
	s = std::format("{:6Ld}", gl_pTiingoDataSource->GetCurrentInquiryTime());
	SysCallSetInnerSystemPaneText(6, s);
	// 更新Quandl数据读取时间
	s = std::format("{:6Ld}", gl_pQuandlDataSource->GetCurrentInquiryTime());
	//SysCallSetInnerSystemPaneText(7, buffer);

	if (gl_systemMessage.GetProcessedFinnhubWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(8, gl_systemMessage.GetCurrentFinnhubWebSocketStake());
		SysCallSetInnerSystemPaneText(9, FormatToMK(gl_systemMessage.GetProcessedFinnhubWebSocket()));
	}

	if (gl_systemMessage.GetProcessedTiingoIEXWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(10, gl_systemMessage.GetCurrentTiingoWebSocketIEX());
		SysCallSetInnerSystemPaneText(11, FormatToMK(gl_systemMessage.GetProcessedTiingoIEXWebSocket()));
	}

	if (gl_systemMessage.GetProcessedTiingoForexWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(12, gl_systemMessage.GetCurrentTiingoWebSocketForex());
		SysCallSetInnerSystemPaneText(13, FormatToMK(gl_systemMessage.GetProcessedTiingoForexWebSocket()));
	}

	if (gl_systemMessage.GetProcessedTiingoCryptoWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(14, gl_systemMessage.GetCurrentTiingoWebSocketCrypto());
		SysCallSetInnerSystemPaneText(15, FormatToMK(gl_systemMessage.GetProcessedTiingoCryptoWebSocket()));
	}

	SysCallSetInnerSystemPaneText(16, FormatToMK(gl_TiingoTotalData));
	SysCallSetInnerSystemPaneText(17, FormatToMK(gl_FinnhubTotalData));
	SysCallSetInnerSystemPaneText(18, FormatToMK(gl_ChinaMarketTotalData));
}
void CMainFrame::SetCurrentStock(const CVirtualStockPtr& pStock) {
	if (pStock == nullptr) {
		gl_pCurrentStock = nullptr;
		return;
	}
	if (gl_pCurrentStock != nullptr) {
		gl_pCurrentStock->SetSelected(false);
	}
	gl_pCurrentStock = pStock;
	gl_pCurrentStock->SetSelected(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当系统退出时，需要先退出工作线程。
//
// 系统自动重启时，是发出系统message执行到此处，外部发出的关闭窗口命令，也是执行到此处。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0Xfff0) == SC_CLOSE) {	// 如果是退出系统
		TRACE(_T("应用户申请，准备退出程序\n"));
		gl_systemConfiguration.SetExitingSystem(true); // 提示各工作线程中途退出
		ReportExitToWatchdog();
		for (auto& timer : gl_aTimer) {// 退出所有的计时器，关闭所有的工作线程。
			timer.cancel();
		}
		for (const auto& pMarket : gl_vMarket) {
			pMarket->PrepareToCloseMarket();
		}
	}

	SysCallOnSysCommand(nID, lParam);
}

void CMainFrame::OnProcessTodayStock() {
	if (gl_pChinaMarket->IsSystemReady()) {
		ProcessChinaMarketStock();
	}
}

void CMainFrame::ProcessChinaMarketStock() {
	gl_runtime.thread_executor()->post([] {
		TRACE("China market Process today stock\n");
		gl_systemMessage.SetChinaMarketSavingFunction("Process today stock");
		gl_pChinaMarket->ProcessTodayStock();
		TRACE("China market Processed today stock\n");
	});
}

void CMainFrame::OnUpdateProcessTodayStock(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->IsSystemReady()) {
		// 系统自动更新日线数据时，不允许处理当日的实时数据。
		SysCallCmdUIEnable(pCmdUI, true);
	}
	else SysCallCmdUIEnable(pCmdUI, false);
}

///////////////////////////////////////////////////////////////////
//
// 转发WM_CHAR和WM_KEYUP梁消息至本主框架，由主框架处理之。
//
//
//////////////////////////////////////////////////////////////////
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) {
	if ((pMsg->message == WM_CHAR) || (pMsg->message == WM_KEYUP)) {
		SysCallSendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}

	return SysCallPreTranslateMessage(pMsg);
}

void CMainFrame::CreateDocumentViewIfNeeded() {
	auto pDoc = dynamic_cast<CFireBirdDoc*>(GetActiveFrame()->GetActiveDocument());
	if (pDoc == nullptr) {
		CreateNewView();
	}
}

void CMainFrame::CreateNewView() {
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW, 0);
}

void CMainFrame::SetCurrentDocumentStock(const CVirtualStockPtr& pStock) {
	auto pDoc = dynamic_cast<CFireBirdDoc*>(GetActiveFrame()->GetActiveDocument());
	ASSERT(pDoc != nullptr);
	pDoc->SetCurrentStock(pStock);
}

void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	string strTemp;
	CVirtualStockPtr pStock = nullptr;

	switch (nChar) {
	case 0x00d: // 回车
		strTemp = m_aStockCodeTemp;
		if (gl_dataContainerChinaStock.IsSymbol(strTemp)) { // 中国市场股票
			pStock = gl_dataContainerChinaStock.GetStock(strTemp);
		}
		else if (gl_dataContainerTiingoStock.IsSymbol(strTemp)) { // Tiingo股票
			pStock = gl_dataContainerTiingoStock.GetStock(strTemp);
		}
		SetCurrentStock(pStock);
		CreateDocumentViewIfNeeded();
		SetCurrentDocumentStock(pStock);
		SysCallInvalidate();
		m_aStockCodeTemp[0] = 0x000;
		m_lCurrentPos = 0;
		SetCurrentEditStockChanged(true);
		break;
	case 0x008: // backspace
		if (m_lCurrentPos > 0) {
			m_lCurrentPos--;
			m_aStockCodeTemp[m_lCurrentPos] = 0x000;
			SetCurrentEditStockChanged(true);
		}
		break;
	default:
		if (m_lCurrentPos < 10) {
			m_aStockCodeTemp[m_lCurrentPos] = toupper(nChar);
			m_lCurrentPos++;
			m_aStockCodeTemp[m_lCurrentPos] = 0x000;
		}
		SetCurrentEditStockChanged(true);
		break;
	}

	SysCallOnChar(nChar, nRepCnt, nFlags);
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// todo: 此函数处理用户按键抬起事件，需要考虑不同的市场
///
///
////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CChinaStockPtr pStock;

	if (gl_pCurrentStock != nullptr) {
		switch (nChar) {
		case 33: // PAGE UP
			// last stock
			//gl_pChinaMarket->ChangeToPrevStock();
			break;
		case 34: // PAGE DOWN
			// next stock
			//gl_pChinaMarket->ChangeToNextStock();
			break;
		case 45: // Ins, 加入自选股票
			//gl_pCurrentStock->SetChosen(true);
			//if (gl_pChinaMarket->AddChosenStock(gl_pCurrentStock)) {
			//	gl_pChinaMarket->SetUpdateChosenStockDB(true);
			//}
			break;
		case 46: // delete,从自选股票池中删除
			//gl_pCurrentStock->SetChosen(false);
			//if (gl_pChinaMarket->DeleteChosenStock(gl_pCurrentStock)) {
			//	gl_pChinaMarket->SetUpdateChosenStockDB(true);
			//}
			break;
		default:
			// 无需处理字符，略过
			break;
		}
	}
	//gl_pChinaMarket->AddImmediateTask(CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__); // 立即更新自选股数据库
	SysCallOnKeyUp(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnStopUpdateDayLine() {
	gl_dataContainerChinaStock.ClearDayLineNeedUpdateStatus();
}

void CMainFrame::OnUsingSinaRealtimeDataServer() {
	gl_systemConfiguration.SetChinaMarketRealtimeServer(SinaRealTime_);
	gl_systemConfiguration.UsingSinaRealtimeServer();
	gl_SystemData.ClearChinaMarketRTDataQueue();
}

void CMainFrame::OnUpdateUsingSinaRealtimeDataServer(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingSinaRTServer()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnUsingNeteaseRealtimeDataServer() {
	gl_systemConfiguration.SetChinaMarketRealtimeServer(NeteaseRealTime_);
	gl_systemConfiguration.UsingNeteaseRealtimeServer();
	gl_SystemData.ClearChinaMarketRTDataQueue();
}

void CMainFrame::OnUpdateUsingNeteaseRealtimeDataServer(CCmdUI* pCmdUI) {
	SysCallCmdUIEnable(pCmdUI, false); // 网易不再提供实时数据服务
	if (gl_systemConfiguration.IsUsingNeteaseRTServer()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnUsingTengxunRealtimeDataServer() {
	gl_systemConfiguration.SetChinaMarketRealtimeServer(TengxunRealTime_);
	gl_systemConfiguration.UsingTengxunRealtimeServer();
	gl_SystemData.ClearChinaMarketRTDataQueue();
}

void CMainFrame::OnUpdateUsingTengxunRealtimeDataServer(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingTengxunRTServer()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnUpdateStockSection() {
	gl_dataContainerChinaStockSymbol.SetUpdateStockSection(true);
	gl_pChinaMarket->AddTask(CHINA_MARKET_UPDATE_STOCK_SECTION__, 1);
}

void CMainFrame::OnUpdateStockProfile() {
	gl_runtime.thread_executor()->post([] {
		gl_systemMessage.SetChinaMarketSavingFunction("update china stock profile");
		gl_dataContainerChinaStock.UpdateProfileDB();
	});
}

void CMainFrame::OnRebuildEpsSurprise() {
	gl_pWorldMarket->RebuildEPSSurprise();
}

void CMainFrame::OnRebuildPeer() {
	gl_pWorldMarket->RebuildPeer();
}

void CMainFrame::OnRebuildDayLine() {
	gl_pWorldMarket->RebuildStockDayLineDB();
}

void CMainFrame::OnRecordFinnhubWebSocket() {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) {
		gl_systemConfiguration.SetUsingFinnhubWebSocket(false);
		if (gl_pWorldMarket->IsSystemReady()) gl_pFinnhubWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingFinnhubWebSocket(true);
		if (gl_pWorldMarket->IsSystemReady()) {
			gl_pFinnhubWebSocket->TaskConnectAndSendMessage(gl_pWorldMarket->GetFinnhubWebSocketSymbols());
		}
	}
}

void CMainFrame::OnUpdateRecordFinnhubWebSocket(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnRecordTiingoCryptoWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) {
		gl_systemConfiguration.SetUsingTiingoCryptoWebSocket(false);
		if (gl_pWorldMarket->IsSystemReady()) gl_pTiingoCryptoWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingTiingoCryptoWebSocket(true);
		if (gl_pWorldMarket->IsSystemReady()) {
			gl_pTiingoCryptoWebSocket->TaskConnectAndSendMessage(gl_dataContainerChosenWorldCrypto.GetSymbols());
		}
	}
}

void CMainFrame::OnUpdateRecordTiingoCryptoWebSocket(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingTiingoCryptoWebSocket()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnRecordTiingoForexWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) {
		gl_systemConfiguration.SetUsingTiingoForexWebSocket(false);
		if (gl_pWorldMarket->IsSystemReady()) gl_pTiingoForexWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingTiingoForexWebSocket(true);
		if (gl_pWorldMarket->IsSystemReady()) {
			gl_pTiingoForexWebSocket->TaskConnectAndSendMessage(gl_dataContainerChosenWorldForex.GetSymbols());
		}
	}
}

void CMainFrame::OnUpdateRecordTiingoForexWebSocket(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingTiingoForexWebSocket()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnRecordTiingoIEXWebSocket() {
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) {
		gl_systemConfiguration.SetUsingTiingoIEXWebSocket(false);
		if (gl_pWorldMarket->IsSystemReady()) gl_pTiingoIEXWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingTiingoIEXWebSocket(true);
		if (gl_pWorldMarket->IsSystemReady()) {
			gl_pTiingoIEXWebSocket->TaskConnectAndSendMessage(gl_dataContainerChosenFinnhubStock.GetSymbols());
		}
	}
}

void CMainFrame::OnUpdateRecordTiingoIEXWebSocket(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingTiingoIEXWebSocket()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnRebuildBasicFinancial() {
	gl_pWorldMarket->RebuildBasicFinancial();
}

void CMainFrame::OnMaintainChinaMarketStockDayLine() {
	gl_dataContainerChinaStock.SetDayLineNeedMaintain();
}

//////////////////////////////////////////////////////////////////////////////////
//
//
// 维护日线数据库的工作，可能会超过一个小时，故而需要在当天交易结束后方允许执行。
//
//
///////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnUpdateMaintainChinaMarketStockDayLine(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->IsDummyTime()) {
		SysCallCmdUIEnable(pCmdUI, true);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) {
	CMDIFrameWndEx::OnSize(nType, cx, cy);

	gl_systemConfiguration.SetCurrentWindowRect(cx, cy);
}

void CMainFrame::OnUsingNeteaseDayLineDataServer() {
	gl_systemConfiguration.SetChinaMarketDayLineServer(NeteaseDayLine_);
	gl_pTengxunDayLineDataSource->Enable(true);
	gl_pNeteaseDayLineDataSource->Enable(false);
}

void CMainFrame::OnUpdateUsingNeteaseDayLineDataServer(CCmdUI* pCmdUI) {
	SysCallCmdUIEnable(pCmdUI, false); // 网易不再提供日线服务
	if (gl_systemConfiguration.IsUsingNeteaseDayLineServer()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnUsingTengxunDayLineDataServer() {
	gl_systemConfiguration.SetChinaMarketDayLineServer(TengxunDayLine_);
	gl_pTengxunDayLineDataSource->Enable(false);
	gl_pNeteaseDayLineDataSource->Enable(true);
}

void CMainFrame::OnUpdateUsingTengxunDayLineDataServer(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingTengxunDayLineServer()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnUpdateTiingoFundamentalDefinition() {
	gl_pWorldMarket->SetPermitUpdateTiingoFundamentalDefinitionDB(true);
}

void CMainFrame::OnUpdateUpdateTiingoFundamentalDefinition(CCmdUI* pCmdUI) {
	if (gl_dataContainerTiingoFundamentalDefinition.IsUpdateDB()) {
		pCmdUI->Enable(true);
	}
	else {
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnResetTiingoDayLineDate() {
	gl_dataContainerTiingoStock.ResetDayLineStartEndDate();
}

void CMainFrame::OnCreateTiingoTradeDayDayLine() {
	gl_runtime.thread_executor()->post([] {
		gl_systemMessage.SetWorldMarketSavingFunction("T create dayLine");
		gl_dataContainerTiingoStock.BuildDayLine(gl_pWorldMarket->GetCurrentTradeDate());
	});
}

void CMainFrame::OnUpdateCreateTiingoTradeDayDayLine(CCmdUI* pCmdUI) {
	if (gl_pTiingoDataSource->IsUpdateIEXTopOfBook()) {
		pCmdUI->Enable(false);
	}
	else {
		pCmdUI->Enable(true);
	}
}

void CMainFrame::OnProcessTiingoDayLine() {
	gl_runtime.thread_executor()->post([] {
		gl_dataContainerTiingoStock.TaskProcessTodayDayLine();
	});
}

void CMainFrame::OnUpdateProcessTiingoDayLine(CCmdUI* pCmdUI) {
	if (gl_pTiingoDataSource->IsUpdateDayLine()) {
		pCmdUI->Enable(false); // 更新日线数据时，不允许处理日线数据。
	}
	else {
		pCmdUI->Enable(true); // 随时可以执行
	}
}

void CMainFrame::OnCalculateNewLowFiveTimes() {
	gl_runtime.thread_executor()->post([] {
		gl_dataContainerTiingoStock.TaskCalculate2();
	});
}

void CMainFrame::OnInquireIexTopOfBook() {
	gl_pTiingoDataSource->SetUpdateIEXTopOfBook(true); //
}

void CMainFrame::OnCalculateNasdaq100200maUpdownRate() {
	gl_pWorldMarket->TaskCalculateNasdaq100MA200UpDownRate();
}

void CMainFrame::OnUpdateCalculateNasdaq100200maUpdownRate(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnTiingoRebuildStockSplit() {
	gl_pWorldMarket->RebuildTiingoStockSplitDB();
}

void CMainFrame::OnTiingoDownloadAllDayline() {
	gl_pWorldMarket->UpdateTiingoAllStockDayLine();
}

void CMainFrame::OnTiingoDownloadOneYearDayline() {
	gl_pWorldMarket->UpdateTiingoOneYearStockDayLine();
}

void CMainFrame::OnBuildChinaStockOneYearDayline() {
	gl_pChinaMarket->UpdateOneYearStockDayLine();
}

void CMainFrame::OnBuildChinaMarketAllStockDayline() {
	gl_pChinaMarket->UpdateAllStockDayLine();
}
