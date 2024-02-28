// MainFrm.cpp: CMainFrame 类的实现
//
#include"pch.h"

#include"ThreadStatus.h"
#include"Thread.h"

#include "FireBird.h"
#include "MainFrm.h"
#include"FireBirdView.h"

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

#include "ConvertToString.h"
#include"simdjsonGetValue.h"
#include "TimeConvert.h"

#include"ScheduleTask.h"

#undef max
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;
using namespace std::chrono_literals;

bool CMainFrame::sm_fGlobeInit = false;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

constexpr int iMaxUserToolbars = 10;
constexpr UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
constexpr UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PROCESS_TODAY_STOCK, &CMainFrame::OnProcessTodayStock)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_TODAY_STOCK, &CMainFrame::OnUpdateProcessTodayStock)
	ON_COMMAND(ID_CALCULATE_TODAY_RELATIVE_STRONG, &CMainFrame::OnCalculateTodayRS)
	ON_WM_SYSCOMMAND()
	ON_UPDATE_COMMAND_UI(ID_CALCULATE_TODAY_RELATIVE_STRONG, &CMainFrame::OnUpdateCalculateTodayRS)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_COMMAND(ID_REBUILD_DAYLINE_RS, &CMainFrame::OnRebuildDayLineRS)
	ON_COMMAND(ID_BUILD_RESET_SYSTEM, &CMainFrame::OnBuildResetMarket)
	ON_UPDATE_COMMAND_UI(ID_REBUILD_DAYLINE_RS, &CMainFrame::OnUpdateRebuildDayLineRS)
	ON_COMMAND(ID_BUILD_ABORT_BUINDING_RS, &CMainFrame::OnAbortBuildingRS)
	ON_UPDATE_COMMAND_UI(ID_BUILD_ABORT_BUINDING_RS, &CMainFrame::OnUpdateAbortBuildingRS)
	ON_COMMAND(ID_CALCULATE_10DAY_RS1, &CMainFrame::OnCalculate10dayRS1)
	ON_COMMAND(ID_CALCULATE_10DAY_RS2, &CMainFrame::OnCalculate10dayRS2)
	ON_COMMAND(ID_CALCULATE_10DAY_RS, &CMainFrame::OnCalculate10dayRS)
	ON_UPDATE_COMMAND_UI(ID_CALCULATE_10DAY_RS1, &CMainFrame::OnUpdateCalculate10dayRS1)
	ON_UPDATE_COMMAND_UI(ID_CALCULATE_10DAY_RS2, &CMainFrame::OnUpdateCalculate10dayRS2)
	ON_UPDATE_COMMAND_UI(ID_CALCULATE_10DAY_RS, &CMainFrame::OnUpdateCalculate10dayRS)
	ON_COMMAND(ID_STOP_UPDATE_DAYLINE, &CMainFrame::OnStopUpdateDayLine)
	ON_COMMAND(ID_USING_NETEASE_REALTIME_DATA_SERVER, &CMainFrame::OnUsingNeteaseRealtimeDataServer)
	ON_COMMAND(ID_USING_SINA_REALTIME_DATA_SERVER, &CMainFrame::OnUsingSinaRealtimeDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_NETEASE_REALTIME_DATA_SERVER, &CMainFrame::OnUpdateUsingNeteaseRealtimeDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_SINA_REALTIME_DATA_SERVER, &CMainFrame::OnUpdateUsingSinaRealtimeDataServer)
	ON_COMMAND(ID_BUILD_CREATE_WEEKLINE, &CMainFrame::OnBuildCreateWeekLine)
	ON_UPDATE_COMMAND_UI(ID_BUILD_CREATE_WEEKLINE, &CMainFrame::OnUpdateBuildCreateWeekLine)
	ON_COMMAND(ID_REBUILD_WEEKLINE_RS, &CMainFrame::OnRebuildWeekLineRS)
	ON_UPDATE_COMMAND_UI(ID_REBUILD_WEEKLINE_RS, &CMainFrame::OnUpdateRebuildWeekLineRS)
	ON_COMMAND(ID_BUILD_CURRENT_WEEK_LINE, &CMainFrame::OnBuildCurrentWeekLine)
	ON_UPDATE_COMMAND_UI(ID_BUILD_CURRENT_WEEK_LINE, &CMainFrame::OnUpdateBuildCurrentWeekLine)
	ON_COMMAND(ID_BUILD_REBUILD_CURRENT_WEEK_LINE, &CMainFrame::OnBuildRebuildCurrentWeekLine)
	ON_UPDATE_COMMAND_UI(ID_BUILD_REBUILD_CURRENT_WEEK_LINE, &CMainFrame::OnUpdateBuildRebuildCurrentWeekLine)
	ON_COMMAND(ID_BUILD_REBUILD_CURRENT_WEEK_WEEKLINE_TABLE, &CMainFrame::OnBuildRebuildCurrentWeekWeekLineTable)
	ON_UPDATE_COMMAND_UI(ID_BUILD_REBUILD_CURRENT_WEEK_WEEKLINE_TABLE, &CMainFrame::OnUpdateBuildRebuildCurrentWeekWeekLineTable)
	ON_COMMAND(ID_UPDATE_SECTION_INDEX, &CMainFrame::OnUpdateStockSection)
	ON_COMMAND(ID_UPDATE_STOCK_CODE, &CMainFrame::OnUpdateStockCode)
	ON_COMMAND(ID_REBUILD_EPS_SURPRISE, &CMainFrame::OnRebuildEpsSurprise)
	ON_COMMAND(ID_REBUILD_PEER, &CMainFrame::OnRebuildPeer)
	ON_COMMAND(ID_REBUILD_DAYLINE, &CMainFrame::OnRebuildDayLine)
	ON_COMMAND(ID_UPDATE_AMERICA_STOCK_DAYLINE_START_END, &CMainFrame::OnUpdateWorldStockDayLineStartEnd)
	ON_COMMAND(ID_RECORD_FINNHUB_WEB_SOCKET, &CMainFrame::OnRecordFinnhubWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_FINNHUB_WEB_SOCKET, &CMainFrame::OnUpdateRecordFinnhubWebSocket)
	ON_COMMAND(ID_RECORD_TIINGO_CRYPTO_WEB_SOCKET, &CMainFrame::OnRecordTiingoCryptoWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_TIINGO_CRYPTO_WEB_SOCKET, &CMainFrame::OnUpdateRecordTiingoCryptoWebSocket)
	ON_COMMAND(ID_RECORD_TIINGO_FOREX_WEB_SOCKET, &CMainFrame::OnRecordTiingoForexWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_TIINGO_FOREX_WEB_SOCKET, &CMainFrame::OnUpdateRecordTiingoForexWebSocket)
	ON_COMMAND(ID_RECORD_TIINGO_IEX_WEB_SOCKET, &CMainFrame::OnRecordTiingoIEXWebSocket)
	ON_UPDATE_COMMAND_UI(ID_RECORD_TIINGO_IEX_WEB_SOCKET, &CMainFrame::OnUpdateRecordTiingoIEXWebSocket)
	ON_COMMAND(ID_REBUILD_BASIC_FINANCIAL, &CMainFrame::OnRebuildBasicFinancial)
	ON_COMMAND(ID_MAINTAIN_DAYLINE, &CMainFrame::OnMaintainDayLine)
	ON_UPDATE_COMMAND_UI(ID_MAINTAIN_DAYLINE, &CMainFrame::OnUpdateMaintainDayLine)
	ON_WM_SIZE()
	ON_COMMAND(ID_USING_NETEASE_DAYLINE_DATA_SERVER, &CMainFrame::OnUsingNeteaseDayLineDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_NETEASE_DAYLINE_DATA_SERVER, &CMainFrame::OnUpdateUsingNeteaseDayLineDataServer)
	ON_COMMAND(ID_USING_TENGXUN_DAYLINE_DATA_SERVER, &CMainFrame::OnUsingTengxunDayLineDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_TENGXUN_DAYLINE_DATA_SERVER, &CMainFrame::OnUpdateUsingTengxunDayLineDataServer)
	ON_COMMAND(ID_USING_TENGXUN_REALTIME_DATA_SERVER, &CMainFrame::OnUsingTengxunRealtimeDataServer)
	ON_UPDATE_COMMAND_UI(ID_USING_TENGXUN_REALTIME_DATA_SERVER, &CMainFrame::OnUpdateUsingTengxunRealtimeDataServer)
END_MESSAGE_MAP()

static UINT indicators[] =
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
	ID_CURRENT_RTDATA_SIZE,
	ID_CURRENT_RUNNING_THREAD,
	ID_CURRENT_RUNNING_BACKGROUND_THREAD,
	ID_CURRENT_TIME,
};

static UINT innerSystemIndicators[] =
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
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() {
	if (!sm_fGlobeInit) {
		sm_fGlobeInit = true;
		ix::initNetSystem();// 在Windows环境下，IXWebSocket库需要初始化一次，且只能初始化一次。
	}

	m_uIdTimer = 0;
	Reset();
}

void CMainFrame::Reset() {
	// 在此之前已经准备好了全局股票池（在CChinaMarket的构造函数中）。
	m_lCurrentPos = 0;
	m_timeLast = 0;
}

CMainFrame::~CMainFrame() {
	if (!gl_systemConfiguration.IsWorkingMode())
		TRACE("使用了Test驱动\n");

	gl_systemConfiguration.SetExitingSystem(true);

	if (sm_fGlobeInit) {
		sm_fGlobeInit = false;
		ix::uninitNetSystem();// 退出系统时，析构IXWebSocket库，且只能析构一次。
	}

	if (gl_pChinaMarket->IsUpdateOptionDB()) {
		gl_pChinaMarket->UpdateOptionDB();
	}

	if (gl_pChinaMarket->IsUpdateChosenStockDB()) {
		gl_pChinaMarket->UpdateChosenStockDB(); // 这里直接调用存储函数，不采用工作线程的模式。
	}

	gl_pChinaMarket->SaveCalculatingRSOption();

	while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1); // 等待处理日线历史数据的线程结束。

	// 更新股票代码数据库要放在最后，等待存储日线数据的线程（如果唤醒了的话）结束之后再执行。
	// 因为彼线程也在更新股票代码数据库，而此更新只是消除同类项而已。
	if (gl_dataContainerChinaStock.IsUpdateProfileDB()) {
		ASSERT(!gl_ThreadStatus.IsSavingThreadRunning());
		gl_dataContainerChinaStock.UpdateStockProfileDB(); // 这里直接调用存储函数，不采用工作线程的模式。
	}

	while (gl_ThreadStatus.IsWebInquiringThreadRunning()) Sleep(1); // 等待网络查询线程退出
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
	gl_systemMessage.PushInformationMessage(_T("系统初始化中....."));
	::InitializeSystem();
	gl_systemMessage.PushInformationMessage(_T("重置系统"));

	// 生成系统外观显示部件
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

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
	BOOL bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
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

	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr) {
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp"))) { CMFCToolBar::SetUserImages(&m_UserImages); }
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
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

	/*
	// todo 使用这种工作线程调度模式时，目前会出现数据同步问题。
	// 设置100毫秒每次的背景工作线程调度，用于完成系统各项定时任务。
	gl_timer = gl_runtime.timer_queue()->make_timer(
		1000ms,
		100ms,
		gl_runtime.thread_executor(), // 调度任务使用后台工作线程
		::ScheduleTask);
		*/

	// 设置100毫秒每次的软调度，只用于更新状态任务。
	m_uIdTimer = SetTimer(STOCK_ANALYSIS_TIMER_, 100, nullptr);
	if (m_uIdTimer == 0) { TRACE(_T("生成100ms时钟时失败\n")); }
	return 0;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons) {
	const auto hOutputBarIcon = static_cast<HICON>(::LoadImage(AfxGetResourceHandle(),
	                                                           MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND),
	                                                           IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),
	                                                           GetSystemMetrics(SM_CYSMICON), 0));
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);
	const HICON hPropertiesBarIcon = static_cast<HICON>(::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0));
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CFrameWndEx::PreCreateWindow(cs)) return FALSE;
	// TODO: 在此处通过修改CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows() {
	// 创建输出窗口
	CString strOutputWnd;
	BOOL bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND,
	                        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) {
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) {
		TRACE0("Failed to create Properties window\n");
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
	const LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0) { return 0; }

	const auto pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	CString strCustomize;
	const BOOL bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext)) { return FALSE; }

	CString strCustomize;
	const BOOL bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i++) {
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr) { pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize); }
	}

	return TRUE;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) {
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//
// CMainFrame timer只执行更新状态任务， 其他的定时数据采集处理任务由ScheduleTask()负责执行
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnTimer(UINT_PTR nIDEvent) {
	::ScheduleTask(); // 系统更新任务皆位于此函数中

	// 在窗口显示系统状态的更新任务放在这里比较合适。可以减少窗口句柄问题
	UpdateStatus();
	UpdateInnerSystemStatus();

	SysCallOnTimer(nIDEvent);
}

void CMainFrame::UpdateStatus() {
	CString str;
	char buffer[30]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	const CChinaStockPtr pCurrentStock = gl_pChinaMarket->GetCurrentStock();

	//更新状态条
	if (gl_pChinaMarket->IsCurrentEditStockChanged()) {
		str = m_aStockCodeTemp;
		SysCallSetPaneText(1, str);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
	}
	// 显示股票代码和名称
	if (gl_pChinaMarket->IsCurrentStockChanged()) {
		gl_pChinaMarket->SetCurrentStockChanged(false);
		SysCallSetPaneText(2, pCurrentStock->GetSymbol());
		SysCallSetPaneText(3, pCurrentStock->GetDisplaySymbol());
	}

	// 显示当前选择的股票
	sprintf_s(buffer, _T("%d"), gl_pChinaMarket->GetCurrentSelectedStockSet());
	SysCallSetPaneText(4, buffer);

	// 显示当前选择的位置
	sprintf_s(buffer, _T("%d"), gl_pChinaMarket->GetCurrentSelectedPosition());
	SysCallSetPaneText(5, buffer);

	// 显示当前读取的新浪实时数据股票代码
	SysCallSetPaneText(6, gl_systemMessage.GetStockCodeForInquiringRTData());

	// 显示活跃股票总数
	sprintf_s(buffer, _T("%d"), gl_dataContainerChinaStock.GetActiveStockSize());
	SysCallSetPaneText(7, buffer);

	// 显示当前读取网易日线历史的股票代码
	SysCallSetPaneText(8, gl_systemMessage.GetStockCodeForInquiringNeteaseDayLine());

	SysCallSetPaneText(9, gl_pWorldMarket->GetCurrentFunction());

	// 更新当前抓取的实时数据大小
	if ((GetUTCTime() - m_timeLast) > 0) {// 每秒更新一次
		str = FormatToMK(gl_pSinaRTDataSource->GetTotalByteRead());
		gl_pSinaRTDataSource->ClearTotalByteRead();
		m_timeLast = GetUTCTime();
		m_wndStatusBar.SetPaneText(10, str);
	}
	// 更新当前申请网络数据的工作线程数
	sprintf_s(buffer, _T("%02d"), gl_ThreadStatus.GetNumberOfWebInquiringThread());
	SysCallSetPaneText(11, buffer);

	// 更新当前后台工作线程数
	sprintf_s(buffer, _T("%02d"), gl_ThreadStatus.GetNumberOfBackGroundWorkingThread());

	//sprintf_s(buffer, _T("%1.3f"), gl_pChinaMarket->GetCurrentEffectiveRTDataRatio());
	SysCallSetPaneText(12, buffer);

	//更新当地时间的显示
	SysCallSetPaneText(13, gl_pChinaMarket->GetStringOfLocalTime());
}

void CMainFrame::UpdateInnerSystemStatus() {
	char buffer[30];
	CString str;
	// 更新实时数据读取时间
	switch (gl_systemConfiguration.GetChinaMarketRealtimeServer()) {
	case 0: // 新浪实时数据
		sprintf_s(buffer, _T("%5I64d"), gl_pSinaRTDataSource->GetCurrentInquiryTime());
		break;
	case 1: // 更新网易实时数据读取时间
		sprintf_s(buffer, _T("%5I64d"), gl_pNeteaseRTDataSource->GetCurrentInquiryTime());
		break;
	case 2: // 更新腾讯实时数据读取时间
		sprintf_s(buffer, _T("%5I64d"), gl_pTengxunRTDataSource->GetCurrentInquiryTime());
		break;
	default: // error
		break;
	}
	SysCallSetInnerSystemPaneText(1, buffer);

	// 更新实时数据分配及处理时间
	sprintf_s(buffer, _T("%5I64d"), gl_pChinaMarket->m_ttDistributeAndCalculateTime);
	SysCallSetInnerSystemPaneText(2, buffer);
	// 更新日线数据读取时间
	if (gl_systemConfiguration.IsUsingNeteaseDayLineServer()) { // 网易日线服务器
		sprintf_s(buffer, _T("%5I64d"), gl_pNeteaseDayLineDataSource->GetCurrentInquiryTime());
	}
	else if (gl_systemConfiguration.IsUsingTengxunDayLineServer()) { // 腾讯日线服务器
		sprintf_s(buffer, _T("%5I64d"), gl_pTengxunDayLineDataSource->GetCurrentInquiryTime());
	}
	SysCallSetInnerSystemPaneText(4, buffer);

	// 更新Finnhub数据读取时间
	sprintf_s(buffer, _T("%5I64d"), gl_pFinnhubDataSource->GetCurrentInquiryTime());
	SysCallSetInnerSystemPaneText(5, buffer);
	// 更新Tiingo数据读取时间
	sprintf_s(buffer, _T("%6I64d"), gl_pTiingoDataSource->GetCurrentInquiryTime());
	SysCallSetInnerSystemPaneText(6, buffer);
	// 更新Quandl数据读取时间
	sprintf_s(buffer, _T("%6I64d"), gl_pQuandlDataSource->GetCurrentInquiryTime());
	//SysCallSetInnerSystemPaneText(7, buffer);

	if (gl_systemMessage.GetProcessedFinnhubWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(8, gl_systemMessage.GetCurrentFinnhubWebSocketStake());
		str = FormatToMK(gl_systemMessage.GetProcessedFinnhubWebSocket());
		SysCallSetInnerSystemPaneText(9, str);
	}

	if (gl_systemMessage.GetProcessedTiingoIEXWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(10, gl_systemMessage.GetCurrentTiingoWebSocketIEX());
		str = FormatToMK(gl_systemMessage.GetProcessedTiingoIEXWebSocket());
		SysCallSetInnerSystemPaneText(11, str);
	}

	if (gl_systemMessage.GetProcessedTiingoForexWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(12, gl_systemMessage.GetCurrentTiingoWebSocketForex());
		str = FormatToMK(gl_systemMessage.GetProcessedTiingoForexWebSocket());
		SysCallSetInnerSystemPaneText(13, str);
	}

	if (gl_systemMessage.GetProcessedTiingoCryptoWebSocket() > 0) {
		SysCallSetInnerSystemPaneText(14, gl_systemMessage.GetCurrentTiingoWebSocketCrypto());
		str = FormatToMK(gl_systemMessage.GetProcessedTiingoCryptoWebSocket());
		SysCallSetInnerSystemPaneText(15, str);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当系统退出时，需要先退出工作线程。
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0Xfff0) == SC_CLOSE) {	// 如果是退出系统
		gl_systemConfiguration.SetExitingSystem(true); // 提示各工作线程中途退出
		TRACE("应用户申请，准备退出程序\n");
		for (const auto& pMarket : gl_vMarketPtr) {
			pMarket->PreparingExitMarket();
		}
	}

	SysCallOnSysCommand(nID, lParam);
}

void CMainFrame::OnCalculateTodayRS() {
	CalculateTodayRS();
}

void CMainFrame::CalculateTodayRS() {
	gl_runtime.background_executor()->post([] {
		ThreadBuildDayLineRS(gl_pChinaMarket, gl_pChinaMarket->GetMarketDate());
	});
}

void CMainFrame::OnProcessTodayStock() {
	if (gl_pChinaMarket->IsSystemReady()) {
		ProcessChinaMarketStock();
	}
}

void CMainFrame::ProcessChinaMarketStock() {
	gl_runtime.background_executor()->post([] {
		gl_UpdateChinaMarketDB.acquire();
		gl_pChinaMarket->ProcessTodayStock();
		gl_UpdateChinaMarketDB.release();
	});
}

void CMainFrame::OnUpdateProcessTodayStock(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->IsSystemReady()) {
		// 系统自动更新日线数据时，不允许处理当日的实时数据。
		SysCallCmdUIEnable(pCmdUI, true);
	}
	else SysCallCmdUIEnable(pCmdUI, false);
}

void CMainFrame::OnUpdateCalculateTodayRS(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->IsSystemReady()) {
		if (gl_pChinaMarket->IsCalculatingDayLineRS()) {
			SysCallCmdUIEnable(pCmdUI, false);
		}
		else {
			SysCallCmdUIEnable(pCmdUI, true);
		}
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
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

void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CString strTemp;

	switch (nChar) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'S':
	case 's':
	case 'Z':
	case 'z':
	case '.':
		if (m_lCurrentPos < 10) {
			m_aStockCodeTemp[m_lCurrentPos] = toupper(nChar);
			m_lCurrentPos++;
			m_aStockCodeTemp[m_lCurrentPos] = 0x000;
		}
		gl_pChinaMarket->SetCurrentEditStockChanged(true);
		break;
	case 0x00d: // 回车
		strTemp = m_aStockCodeTemp;
		if (gl_dataContainerChinaStock.IsSymbol(strTemp)) {
			const CChinaStockPtr pStock = gl_dataContainerChinaStock.GetStock(strTemp);
			gl_pChinaMarket->SetCurrentStock(pStock);
			SysCallInvalidate();
		}
		m_aStockCodeTemp[0] = 0x000;
		m_lCurrentPos = 0;
		gl_pChinaMarket->SetCurrentEditStockChanged(true);
		break;
	case 0x008: // backspace
		if (m_lCurrentPos > 0) {
			m_lCurrentPos--;
			m_aStockCodeTemp[m_lCurrentPos] = 0x000;
			gl_pChinaMarket->SetCurrentEditStockChanged(true);
		}
		break;
	default:
		break;
	}
	if (gl_pChinaMarket->IsCurrentStockChanged()) {
		const auto pChild = static_cast<CMDIChildWnd*>(GetActiveFrame());
		const auto pView = static_cast<CFireBirdView*>(pChild->GetActiveView());
		if (pView != nullptr) {
			pView->UpdateHistoryDataContainer(gl_pChinaMarket->GetCurrentStock());
		}
	}

	SysCallOnChar(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CChinaStockPtr pStock;
	CString strTemp;
	const CChinaStockPtr pCurrentStock = gl_pChinaMarket->GetCurrentStock();

	if (pCurrentStock != nullptr) {
		switch (nChar) {
		case 118: // F7，选择前一个股票集
			gl_pChinaMarket->ChangeToPrevStockSet();
			gl_pChinaMarket->SetCurrentSelectedPosition(0);
			gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetCurrentSelectedStock());
			break;
		case 119: // F8， 选择后一个股票集
			gl_pChinaMarket->ChangeToNextStockSet();
			gl_pChinaMarket->SetCurrentSelectedPosition(0);
			gl_pChinaMarket->SetCurrentStock(gl_pChinaMarket->GetCurrentSelectedStock());
			break;
		case 33: // PAGE UP
			// last stock
			gl_pChinaMarket->ChangeToPrevStock();
			break;
		case 34: // PAGE DOWN
			// next stock
			gl_pChinaMarket->ChangeToNextStock();
			break;
		case 45: // Ins, 加入自选股票
			pCurrentStock->SetChosen(true);
			if (gl_pChinaMarket->AddChosenStock(pCurrentStock)) {
				gl_pChinaMarket->SetUpdateChosenStockDB(true);
			}
			break;
		case 46: // delete,从自选股票池中删除
			pCurrentStock->SetChosen(false);
			if (gl_pChinaMarket->DeleteChosenStock(pCurrentStock)) {
				gl_pChinaMarket->SetUpdateChosenStockDB(true);
			}
			break;
		default:
			// 无需处理字符，略过
			break;
		}
	}
	if (gl_pChinaMarket->IsCurrentStockChanged()) {
		const auto pChild = static_cast<CMDIChildWnd*>(GetActiveFrame());
		const auto pView = static_cast<CFireBirdView*>(pChild->GetActiveView());
		if (pView != nullptr) {
			pView->UpdateHistoryDataContainer(gl_pChinaMarket->GetCurrentStock());
		}
	}
	gl_pChinaMarket->AddTask(CHINA_MARKET_UPDATE_CHOSEN_STOCK_DB__, 1); // 立即更新自选股数据库
	SysCallOnKeyUp(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnRebuildDayLineRS() {
	gl_runtime.background_executor()->post([] {
		ThreadBuildDayLineRS(gl_pChinaMarket, _CHINA_MARKET_BEGIN_DATE_);
	});
}

void CMainFrame::OnBuildResetMarket() {
	for (const auto& pMarket : gl_vMarketPtr) {
		pMarket->SetResetMarket(true);
	}
}

void CMainFrame::OnUpdateRebuildDayLineRS(CCmdUI* pCmdUI) {
	// 要避免在八点至半九点半之间执行重算相对强度的工作，因为此时间段时要重置系统，结果导致程序崩溃。
	if ((gl_pChinaMarket->GetMarketTime() > 83000) && (gl_pChinaMarket->GetMarketTime() < 93000)) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else if (gl_pChinaMarket->IsCalculatingDayLineRS()) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, true);
	}
}

void CMainFrame::OnAbortBuildingRS() {
	ASSERT(!gl_systemConfiguration.IsExitingCalculatingRS());
	gl_systemConfiguration.SetExitingCalculatingRS(true);
}

void CMainFrame::OnUpdateAbortBuildingRS(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->IsCalculatingDayLineRS()) {
		SysCallCmdUIEnable(pCmdUI, true);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
}

void CMainFrame::OnCalculate10dayRS1() {
	gl_runtime.background_executor()->post([] {
		gl_UpdateChinaMarketDB.acquire();
		gl_systemMessage.PushInformationMessage(_T("开始计算10日RS1\n"));

		// 添加一个注释
		if (gl_dataContainerChinaStock.Choice10RSStrong1StockSet()) {
			gl_systemMessage.PushInformationMessage(_T("10日RS1计算完毕\n"));
			gl_pChinaMarket->SetUpdatedDateFor10DaysRS1(gl_pChinaMarket->GetMarketDate());
			gl_pChinaMarket->SetUpdateOptionDB(true); // 更新选项数据库
		}
		gl_UpdateChinaMarketDB.release();
	});
	gl_pChinaMarket->SetChosen10RSStrong1StockSet(true);
}

void CMainFrame::OnCalculate10dayRS2() {
	gl_runtime.background_executor()->post([] {
		gl_UpdateChinaMarketDB.acquire();
		gl_systemMessage.PushInformationMessage(_T("开始计算10日RS2\n"));

		// 添加一个注释
		if (gl_dataContainerChinaStock.Choice10RSStrong2StockSet()) {
			gl_systemMessage.PushInformationMessage(_T("10日RS2计算完毕\n"));
			gl_pChinaMarket->SetUpdatedDateFor10DaysRS2(gl_pChinaMarket->GetMarketDate());
			gl_pChinaMarket->SetUpdateOptionDB(true); // 更新选项数据库
		}
		gl_UpdateChinaMarketDB.release();
	});
	gl_pChinaMarket->SetChosen10RSStrong2StockSet(true);
}

void CMainFrame::OnCalculate10dayRS() {
	gl_pChinaMarket->Choice10RSStrongStockSet();
	gl_pChinaMarket->SetChosen10RSStrongStockSet(true);
}

void CMainFrame::OnUpdateCalculate10dayRS1(CCmdUI* pCmdUI) {
	if ((gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber() > 0) && (gl_dataContainerChinaStock.GetDayLineNeedSaveNumber() > 0)) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else SysCallCmdUIEnable(pCmdUI, true);
}

void CMainFrame::OnUpdateCalculate10dayRS2(CCmdUI* pCmdUI) {
	if ((gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber() > 0) && (gl_dataContainerChinaStock.GetDayLineNeedSaveNumber() > 0)) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else SysCallCmdUIEnable(pCmdUI, true);
}

void CMainFrame::OnUpdateCalculate10dayRS(CCmdUI* pCmdUI) {
	if ((gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber() > 0) && (gl_dataContainerChinaStock.GetDayLineNeedSaveNumber() > 0)) {
		SysCallCmdUIEnable(pCmdUI, false);
	}
	else SysCallCmdUIEnable(pCmdUI, true);
}

void CMainFrame::OnStopUpdateDayLine() {
	gl_dataContainerChinaStock.ClearDayLineNeedUpdateStatus();
}

void CMainFrame::OnUsingSinaRealtimeDataServer() {
	gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
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
	gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
	gl_systemConfiguration.UsingNeteaseRealtimeServer();
	gl_SystemData.ClearChinaMarketRTDataQueue();
}

void CMainFrame::OnUpdateUsingNeteaseRealtimeDataServer(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingNeteaseRTServer()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnUsingTengxunRealtimeDataServer() {
	gl_systemConfiguration.SetChinaMarketRealtimeServer(2);
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

void BuildWeekLine(long lStartDate) {
	gl_UpdateChinaMarketDB.acquire();

	const long lStartMonday = GetCurrentMonday(lStartDate);
	const long year = lStartMonday / 10000;
	const long month = lStartMonday / 100 - (lStartMonday / 10000) * 100;
	const long mDay = lStartMonday - (lStartMonday / 100) * 100;
	CTime ctCurrent(year, month, mDay, 12, 0, 0);
	const CTimeSpan ts7Day(7, 0, 0, 0);
	long lCurrentMonday = lStartMonday;

	if (lStartDate > 19900101) {// 目前此种情况只用于重新生成本周周线
		ASSERT(lStartMonday == GetCurrentMonday(gl_pChinaMarket->GetMarketDate()));
		do {
			gl_pChinaMarket->DeleteWeekLine(lCurrentMonday);
			ctCurrent += ts7Day;
			lCurrentMonday = ctCurrent.GetYear() * 10000 + ctCurrent.GetMonth() * 100 + ctCurrent.GetDay();
		} while (lCurrentMonday <= gl_pChinaMarket->GetMarketDate());
	}
	else {
		gl_pChinaMarket->DeleteWeekLine();
	}

	gl_dataContainerChinaStock.BuildWeekLine(lStartMonday);

	// 清除当前周周线表
	gl_pChinaMarket->DeleteCurrentWeekWeekLine();
	// 生成新的当前周周线
	gl_pChinaMarket->BuildCurrentWeekWeekLineTable();

	gl_UpdateChinaMarketDB.release();
}

void CMainFrame::OnBuildCreateWeekLine() {
	auto lStartDate = 19900101;
	gl_runtime.background_executor()->post([lStartDate] {
		BuildWeekLine(lStartDate);
	});
}

void CMainFrame::OnUpdateBuildCreateWeekLine(CCmdUI* pCmdUI) {
}

void CMainFrame::OnRebuildWeekLineRS() {
	gl_runtime.background_executor()->post([] {
		ThreadBuildWeekLineRS(gl_pChinaMarket, _CHINA_MARKET_BEGIN_DATE_);
	});
}

void CMainFrame::OnUpdateRebuildWeekLineRS(CCmdUI* pCmdUI) {
}

void CMainFrame::OnBuildCurrentWeekLine() {
	gl_runtime.background_executor()->post([] {
		gl_UpdateChinaMarketDB.acquire();
		gl_pChinaMarket->BuildWeekLineOfCurrentWeek();
		gl_UpdateChinaMarketDB.release();
	});
}

void CMainFrame::OnUpdateBuildCurrentWeekLine(CCmdUI* pCmdUI) {
	if ((gl_pChinaMarket->GetMarketTime() > 151000)) {
		SysCallCmdUIEnable(pCmdUI, true);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
}

void CMainFrame::OnBuildRebuildCurrentWeekLine() {
	auto lStartDate = gl_pChinaMarket->GetMarketDate();
	gl_runtime.background_executor()->post([lStartDate] {
		BuildWeekLine(lStartDate);
	});
}

void CMainFrame::OnUpdateBuildRebuildCurrentWeekLine(CCmdUI* pCmdUI) {
}

void CMainFrame::OnBuildRebuildCurrentWeekWeekLineTable() {
	gl_runtime.background_executor()->post([] {
		gl_UpdateChinaMarketDB.acquire();
		gl_pChinaMarket->DeleteCurrentWeekWeekLine();// 清除当前周周线表
		gl_pChinaMarket->BuildCurrentWeekWeekLineTable();// 生成新的当前周周线
		gl_UpdateChinaMarketDB.release();
	});
}

void CMainFrame::OnUpdateBuildRebuildCurrentWeekWeekLineTable(CCmdUI* pCmdUI) {
}

void CMainFrame::OnUpdateStockSection() {
	gl_dataContainerChinaStockSymbol.SetUpdateStockSection(true);
	gl_pChinaMarket->AddTask(CHINA_MARKET_UPDATE_STOCK_SECTION__, 1);
}

void CMainFrame::OnUpdateStockCode() {
	gl_runtime.background_executor()->post([] {
		gl_UpdateChinaMarketDB.acquire();
		gl_dataContainerChinaStock.UpdateStockProfileDB();
		gl_UpdateChinaMarketDB.release();
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

void CMainFrame::OnUpdateWorldStockDayLineStartEnd() {
	gl_runtime.background_executor()->post([] {
		gl_UpdateWorldMarketDB.acquire();
		gl_pWorldMarket->UpdateStockDayLineStartEndDate();
		gl_UpdateWorldMarketDB.release();
	});
}

void CMainFrame::OnRecordFinnhubWebSocket() {
	if (gl_systemConfiguration.IsUsingFinnhubWebSocket()) {
		gl_systemConfiguration.SetUsingFinnhubWebSocket(false);
		gl_pFinnhubWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingFinnhubWebSocket(true);
		gl_pWorldMarket->TaskStartFinnhubWebSocket();
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
		gl_pTiingoCryptoWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingTiingoCryptoWebSocket(true);
		gl_pWorldMarket->TaskStartTiingoCryptoWebSocket();
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
		gl_pTiingoForexWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingTiingoForexWebSocket(true);
		gl_pWorldMarket->TaskStartTiingoForexWebSocket();
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
		gl_pTiingoIEXWebSocket->TaskDisconnect();
	}
	else {
		gl_systemConfiguration.SetUsingTiingoIEXWebSocket(true);
		gl_pWorldMarket->TaskStartTiingoIEXWebSocket();
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

void CMainFrame::OnMaintainDayLine() {
	gl_dataContainerChinaStock.SetDayLineNeedMaintain();
}

//////////////////////////////////////////////////////////////////////////////////
//
//
// 维护日线数据库的工作，可能会超过一个小时，故而需要在当天交易结束后方允许执行。
//
//
///////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnUpdateMaintainDayLine(CCmdUI* pCmdUI) {
	if (gl_pChinaMarket->IsDummyTime()) {
#ifndef _DEBUG
		if (gl_pChinaMarket->GetMarketTime() > 151000)
#endif
		SysCallCmdUIEnable(pCmdUI, true);
	}
	else {
		SysCallCmdUIEnable(pCmdUI, false);
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) {
	CFrameWndEx::OnSize(nType, cx, cy);

	gl_systemConfiguration.SetCurrentWindowRect(cx, cy);
}

void CMainFrame::OnUsingNeteaseDayLineDataServer() {
	gl_systemConfiguration.SetChinaMarketDayLineServer(0);
	gl_pTengxunDayLineDataSource->Enable(true);
	gl_pNeteaseDayLineDataSource->Enable(false);
}

void CMainFrame::OnUpdateUsingNeteaseDayLineDataServer(CCmdUI* pCmdUI) {
	if (gl_systemConfiguration.IsUsingNeteaseDayLineServer()) {
		SysCallCmdUISetCheck(pCmdUI, true);
	}
	else {
		SysCallCmdUISetCheck(pCmdUI, false);
	}
}

void CMainFrame::OnUsingTengxunDayLineDataServer() {
	gl_systemConfiguration.SetChinaMarketDayLineServer(1);
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
