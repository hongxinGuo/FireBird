
// MainFrm.cpp: CMainFrame 类的实现
//

#include "stdafx.h"
#include "StockAnalysis.h"

#include "MainFrm.h"

#include"globedef.h"

#include "MainFrm.h"

#include"RTData.h"
#include"DayLine.h"
#include"Stock.h"
#include"Market.h"
#include"StockID.h"

#include"accessory.h"
#include"ClientThread.h"

using namespace std;
#include<string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()
  ON_WM_TIMER()
  ON_COMMAND(ID_SAVE_RTDATA, &CMainFrame::OnSaveRtdata)
  ON_COMMAND(ID_DownLoad_DayLine, &CMainFrame::OnDownloadDayline)
  ON_COMMAND(ID_SAVE_DAYLINE_DATA, &CMainFrame::OnSaveDaylineData)
  ON_UPDATE_COMMAND_UI(ID_SAVE_DAYLINE_DATA, &CMainFrame::OnUpdateSaveDaylineData)
  ON_COMMAND(ID_COMPILE_TODAY_STOCK, &CMainFrame::OnCompileTodayStock)
  ON_UPDATE_COMMAND_UI(ID_COMPILE_TODAY_STOCK, &CMainFrame::OnUpdateCompileTodayStock)
  ON_COMMAND(ID_CALCULATE_RELATIVE_STRONG, &CMainFrame::OnCalculateRelativeStrong)
  ON_WM_SYSCOMMAND()
  ON_UPDATE_COMMAND_UI(ID_CALCULATE_RELATIVE_STRONG, &CMainFrame::OnUpdateCalculateRelativeStrong)
//  ON_WM_CHAR()
//  ON_WM_KEYUP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
  ID_CURRENT_INPUT,
  ID_CURRENT_SELECT_STOCK,
  ID_CURRENT_SELECT_STOCKNAME,
  ID_CURRENT_TIME,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
    // 生成所有股票代码池.这个函数必须最先调用。
  CreateTotalStockContainer();

  // 设置股票日线查询环境
  time(&gl_ttime);
  localtime_s(&gl_tm, &gl_ttime);
  CString str2;
  gl_lToday = (gl_tm.tm_year + 1900) * 10000 + (gl_tm.tm_mon + 1) * 100 + gl_tm.tm_mday;
  gl_lTime = gl_tm.tm_hour * 10000 + gl_tm.tm_min * 100 + gl_tm.tm_sec;
  switch (gl_tm.tm_wday) {
  case 1: // 星期一
    gl_ttime -= 3 * 24 * 3600; // 
    break;
  case 0: //星期日
    gl_ttime -= 3 * 24 * 3600; // 
    break;
  case 6: // 星期六
    gl_ttime -= 2 * 24 * 3600;
    break;
  default: // 其他
    gl_ttime -= 24 * 3600; // 
  }
  tm tm_;
  localtime_s(&tm_, &gl_ttime);
  gl_lLastTradeDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;


  m_fCheckTodayActiveStock = true;

  m_fGetRTStockData = true;
  m_fGetDayLineData = true;
  m_fCountDownRT = true;      // 初始时执行慢速查询实时行情。
  m_iCountDownDayLine = 2;    // 400ms延时（200ms每次）
  
  CSetStockCode setStockCode;

  setStockCode.Open();

  // 装入股票代码数据库
  while (!setStockCode.IsEOF()) {
    long lIndex = 1;
    lIndex = gl_mapTotalStockToIndex.at(setStockCode.m_StockCode);
    if (setStockCode.m_IPOed != __STOCK_NOT_CHECKED__) { // 如果此股票代码已经被检查过，则设置股票目前状态。否则不设置。
      gl_vTotalStock.at(lIndex)->m_lIPOed = setStockCode.m_IPOed;
    }
    gl_vTotalStock.at(lIndex)->m_lDayLineStartDay = setStockCode.m_DayLineStartDay;
    if (gl_vTotalStock.at(lIndex)->m_lDayLineEndDay < setStockCode.m_DayLineEndDay) { // 有时一个股票会有多个记录，以最后的日期为准。
      gl_vTotalStock.at(lIndex)->m_lDayLineEndDay = setStockCode.m_DayLineEndDay;
    }
    if (gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay < setStockCode.m_NewestDayLineDay) { // 有时一个股票会有多个记录，以最后的日期为准。
      gl_vTotalStock.at(lIndex)->m_lNewestDayLineDay = setStockCode.m_NewestDayLineDay;
    }
    // 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
    if (gl_lLastTradeDay <= gl_vTotalStock.at(lIndex)->m_lDayLineEndDay) { // 最新日线数据为今日或者上一个交易日的数据。
      gl_vTotalStock.at(lIndex)->m_fDayLineNeedUpdated = false; // 日线数据不需要更新
    }
    if (setStockCode.m_IPOed == __STOCK_NULL__) { // 无效代码不需更新日线数据
      gl_vTotalStock.at(lIndex)->m_fDayLineNeedUpdated = false;
    }
    if (setStockCode.m_IPOed == __STOCK_DELISTED__) { // 退市股票不需更新日线数据
      gl_vTotalStock.at(lIndex)->m_fDayLineNeedUpdated = false;
    }
    setStockCode.MoveNext();
  }
  setStockCode.Close();

  CString str = _T("[ID] = 1"); // 采用主键作为搜索Index。
  gl_setSavingDayLineOnly.m_strFilter = str; // 必须设置，否则会把所有的数据读入，浪费时间
  gl_setSavingDayLineOnly.Open(); // 永远打开，用于存储接收到的日线历史数据。

  CSetOption setOption;
  setOption.Open();
  if (setOption.IsEOF()) {
    gl_lRelativeStrongEndDay = gl_lRelativeStrongStartDay = 19900101;
  }
  else {
    if (setOption.m_RelativeStrongEndDay == 0) {
      gl_lRelativeStrongEndDay = 19900101;
    }
    else {
      gl_lRelativeStrongEndDay = setOption.m_RelativeStrongEndDay;
    }
    if (setOption.m_RalativeStrongStartDay == 0) {
      gl_lRelativeStrongStartDay = 19900101;
    }
    else {
      gl_lRelativeStrongStartDay = setOption.m_RalativeStrongStartDay;
    }
  }
  setOption.Close();

  // 启动数据处理线程
  AfxBeginThread(ClientThreadUpdatingDataBaseProc, GetSafeHwnd());
  // 启动计算实时数据线程
  AfxBeginThread(ClientThreadCalculatingRTDataProc, GetSafeHwnd());

}

CMainFrame::~CMainFrame()
{
  gl_fExiting = true;

  if (gl_setSavingDayLineOnly.IsOpen()) gl_setSavingDayLineOnly.Close();

  CSetOption setOption;
  setOption.Open();
  setOption.m_pDatabase->BeginTrans();
  if (setOption.IsEOF()) {
    setOption.AddNew();
    setOption.m_RelativeStrongEndDay = gl_lRelativeStrongEndDay;
    setOption.m_RalativeStrongStartDay = gl_lRelativeStrongStartDay;
    setOption.Update();
  }
  else {
    setOption.Edit();
    setOption.m_RelativeStrongEndDay = gl_lRelativeStrongEndDay;
    setOption.m_RalativeStrongStartDay = gl_lRelativeStrongStartDay;
    setOption.Update();
  }
  setOption.m_pDatabase->CommitTrans();
  setOption.Close();


  while (gl_systemStatus.IsDataBaseInProcess()) {
    Sleep(100); // 等待处理日线历史数据的线程结束。
  }

  // 更新股票代码数据库要放在最后，等待存储日线数据的线程（如果唤醒了的话）结束之后再执行。
  // 因为彼线程也在更新股票代码数据库，而此更新只是消除同类项而已。
  UpdateStockCodeDataBase();

  TRACE("finally exited\n");

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作: 
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

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
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);


	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
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

  // 设置200毫秒每次的软调度，用于接受处理实时网络数据
  m_uIdTimer = SetTimer(1, 200, nullptr);     // 100毫秒每次调度，用于从股票数据提供网站读取数据。
  if (m_uIdTimer == 0) {
    CString str;
  }
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成今日日线数据。每日只能执行一次。此函数目前手动启动，待稍微成熟后即自动启动（每日下午三点后自动执行）。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::CompileTodayStocks(void) {

  if (!gl_ChinaStockMarket.MarketReady()) return false;

  AfxBeginThread(ClientThreadCompileTodayStocks, 0);

  return(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成股票代码的字符串，用于查询此股票在当前市场是否处于活跃状态（或者是否存在此股票号码）
//
//
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::CreateTodayActiveStockRTInquiringStr(CString &str) {
  static int iStarted = 0;
  static bool fCreateStr = false;
  static int siCounter = 0, siTotalStock;

  if (fCreateStr == false) {
    siTotalStock = gl_vTotalStock.size();
    fCreateStr = true;
  }

  str = gl_vTotalStock.at(siCounter)->m_strStockCode;
  siCounter++;
  if (siCounter == siTotalStock) {
    siCounter = 0;
    return true; // 查询到头了
  }

  for (int i = 1; i < 900; i++) {  // 每次轮询900个股票.
    if (siCounter == siTotalStock) {
      siCounter = 0;
      return true; // 查询到头了
    }
    str += _T(",");
    str += gl_vTotalStock.at(siCounter)->m_strStockCode;
    siCounter++;
  }
  siCounter -= 2; // 后退两格。为了防止边缘数据错误，故边缘数据查询两遍。

  return false;
}



bool CMainFrame::GetSinaStockRTData(void)
{
  static int iCountUp = 0;
  char * pCurrentPos = nullptr;
  CStockRTDataPtr pRTData = nullptr;
  long i = 0;
  long long iTotalNumber = 0;

  if (!gl_systemStatus.IsReceiveFromWebInProcess()) {
    if ((gl_stRTDataInquire.fError == false) && gl_systemStatus.IsRTDataReady()) { //网络通信一切顺利？
      iTotalNumber = gl_stRTDataInquire.lByteRead;
      pCurrentPos = gl_stRTDataInquire.buffer;
      long  iCount = 0;
      while (iCount < iTotalNumber) { // 新浪实时数据基本没有错误，不需要抛掉最后一组数据了。
        pRTData = make_shared<CStockRTData>();
        if (pRTData->ReadData(pCurrentPos, iCount)) {
          i++;
          gl_systemDequeData.PushRTData(pRTData); // 将此实时数据指针存入实时数据队列
        }
        else {
          TRACE("实时数据有误,抛掉不用\n");
          iCount = iTotalNumber; // 后面的数据可能出问题，抛掉不用。
        }
      }
    }
    else {
      TRACE("Error reading http file ：hq.sinajs.cn, code = %i\n", gl_dwRTDataHttpFileStatus);
    }

    // 处理接收到的实时数据
    gl_ChinaStockMarket.ProcessRTData();

    bool fFinished = false;
    CString strTemp = _T("");

    // 申请下一批次股票实时数据
    if (m_fCheckTodayActiveStock) {
      gl_stRTDataInquire.strInquire = gl_strRTStockSource;
      fFinished = CreateTodayActiveStockRTInquiringStr(strTemp);
      gl_stRTDataInquire.strInquire += strTemp;
      if (fFinished) m_fCheckTodayActiveStock = false;
    }
    else {
      gl_ChinaStockMarket.SetMarketReadyFlag(true); // 所有的股票实时数据都轮询一遍，当日活跃股票集已经建立，故而可以接受日线数据了。
      gl_stRTDataInquire.strInquire = gl_strRTStockSource;
      gl_ChinaStockMarket.GetInquiringStockStr(gl_stRTDataInquire.strInquire);
    }
    gl_systemStatus.SetRTDataReady(false);
    gl_systemStatus.SetReceiveFromWebInProcess(true);  // 在此先设置一次，以防重入（线程延迟导致）
    AfxBeginThread(ClientThreadRTDataProc, GetSafeHwnd());
  }

  if (iCountUp == 1000) {
    m_fCheckTodayActiveStock = true;
    iCountUp = 0;
  }
  else iCountUp++;

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初始化所有可能的股票代码池，只被CMainFrame的初始函数调用一次。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::CreateTotalStockContainer(void)
{
  char buffer[10];

  StockIDPtr pStockID;
  int iCount = 0;


  // 生成上海股票代码
  for (int i = 600000; i < 602000; i++) {
    _itoa_s(i, buffer, 10);
    pStockID = make_shared<CStockID>();
    pStockID->m_nIndex = iCount;
    pStockID->m_strStockCode = _T("sh");
    pStockID->m_strStockCode += buffer;
    pStockID->m_wMarket = 1; // 上海市场
    pStockID->m_nIndex = iCount;
    gl_vTotalStock.push_back(pStockID);
    gl_mapTotalStockToIndex[pStockID->m_strStockCode] = iCount++; // 生成新的映射
  }


  ///////////////////////////////////////////////
  // 生成深圳股票代码
  for (int i = 0; i < 3000; i++) {
    sprintf_s(buffer, 10, "%06d", i);
    pStockID = make_shared<CStockID>();
    pStockID->m_nIndex = iCount;
    pStockID->m_strStockCode = _T("sz");
    pStockID->m_strStockCode += buffer;
    pStockID->m_wMarket = 2; // 深圳市场
    pStockID->m_nIndex = iCount;
    gl_vTotalStock.push_back(pStockID);
    gl_mapTotalStockToIndex[pStockID->m_strStockCode] = iCount++;
  }


  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 生成股票代码的字符串，用于查询此股票在当前市场是否处于活跃状态（或者是否存在此股票号码）
// 
//  此函数是检查gl_vTotalStock股票池
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::CreateTodayActiveStockDayLineInquiringStr(CString &str, CString& strStartDay) {
  static int iStarted = 0;
  static bool fCreateStr = false;
  static int siCounter = 0, siTotalStock;

  StockIDPtr pStockID;

  if (fCreateStr == false) {
    siTotalStock = gl_vTotalStock.size();
    fCreateStr = true;
  }

  bool fFound = false;
  int iCount = 0;
  CString strTemp;
  while (!fFound && (iCount < 1000)) {
    if (!gl_vTotalStock.at(siCounter)->m_fDayLineNeedUpdated) { // 日线数据不需要更新。在系统初始时，设置此m_fDayLineNeedUpdated标识
      // TRACE("%S 日线数据无需更新\n", static_cast<LPCWSTR>(gl_vTotalStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == siTotalStock) {
        siCounter = 0;
      }
    }
    else if (gl_vTotalStock.at(siCounter)->m_lIPOed == __STOCK_NULL__) {	// 尚未使用过的股票代码无需查询日线数据
      gl_vTotalStock.at(siCounter)->m_fDayLineNeedUpdated = false; // 此股票日线资料不需要更新了。
      // TRACE("无效股票代码：%S, 无需查询日线数据\n", static_cast<LPCWSTR>(gl_vTotalStock.at(siCounter)->m_strStockCode));
      iCount++;
      siCounter++;
      if (siCounter == siTotalStock) {
        siCounter = 0;
      }
    }
    else if (gl_vTotalStock.at(siCounter)->m_lDayLineEndDay >= gl_lLastTradeDay) { // 上一交易日的日线数据已经存储？此时已经处理过一次日线数据了，无需再次处理。
      gl_vTotalStock.at(siCounter)->m_fDayLineNeedUpdated = false; // 此股票日线资料不需要更新了。
      // TRACE("%S 日线数据本日已更新\n", static_cast<LPCWSTR>(gl_vTotalStock.at(siCounter)->m_strStockCode));
      siCounter++;
      iCount++;
      if (siCounter == siTotalStock) {
        siCounter = 0;
      }
    }
    else {
      fFound = true;
    }
  }

  if (iCount >= 1000) { //  没有找到需要申请日线的股票
    TRACE("未找到需更新日线历史数据的股票\n");
    return false;
  }

  switch (gl_vTotalStock.at(siCounter)->m_wMarket) {
  case 1: // 上海市场？
    str += '0'; // 上海市场标识
    break;
  case 2: // 深圳市场？
    str += '1'; // 深圳市场标识
    break;
  default: // 越界
    ASSERT(0);
  }
  char buffer[30];
  gl_strCurrentStockDownLoading = gl_vTotalStock.at(siCounter)->m_strStockCode;
  str += gl_vTotalStock.at(siCounter)->m_strStockCode.Right(6); // 取股票代码的右边六位数字。
  tm tm_;
  tm_.tm_year = gl_vTotalStock.at(siCounter)->m_lDayLineEndDay / 10000 - 1900;
  tm_.tm_mon = gl_vTotalStock.at(siCounter)->m_lDayLineEndDay / 100 - (tm_.tm_year + 1900) * 100 - 1;
  tm_.tm_mday = gl_vTotalStock.at(siCounter)->m_lDayLineEndDay - (tm_.tm_year + 1900) * 10000 - (tm_.tm_mon + 1) * 100;
  tm_.tm_hour = 12;
  tm_.tm_min = 0;
  tm_.tm_sec = 0;
  time_t tTime;
  tTime = mktime(&tm_);
  tTime += 3600 * 24; // 增加一天。
  localtime_s(&tm_, &tTime);
  long lDay = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday; // 日线的起始日期是最新日期的后一天。
  sprintf_s(buffer, "%8d", lDay);
  strStartDay = buffer;
  siCounter++;
  if (siCounter == siTotalStock) {
    siCounter = 0;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 采用网易日线申请接口，读取日线数据。
// 每次读取一个股票。
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::GetNetEaseStockDayLineData(void)
{
  CString strRead;
  string str;
  static bool sfFoundStock = true;
  CDayLinePtr pDayLine = nullptr;

  if (!gl_systemStatus.IsReadingInProcess()) {
    if (sfFoundStock) {
      if ((gl_stDayLineInquire.fError == false) && gl_systemStatus.IsDayLineDataReady()) { //网络通信一切顺利？
        TRACE("股票%s日线数据为%d字节\n", (LPCSTR)gl_strCurrentStockDownLoading, gl_stDayLineInquire.lByteRead);
        ASSERT(gl_stDayLineInquire.lByteRead < 2048 * 1024);
        // 处理当前股票日线数据
        gl_ChinaStockMarket.ProcessDayLineData(gl_stDayLineInquire.buffer, gl_stDayLineInquire.lByteRead);
      }
      else {
        if (gl_stDayLineInquire.lByteRead > 0) {
          TRACE("Error reading http file ：quotes.money.163.com/service/, code = %i\n", gl_dwRTDataHttpFileStatus);
          gl_stDayLineInquire.lByteRead = 0;
        }
      }
    }

    CString strTemp;
    char buffer2[200];
    CString strStartDay;

    // 准备网易日线数据申请格式
    strRead = gl_strDayLineStockSource;
    sfFoundStock = CreateTodayActiveStockDayLineInquiringStr(strRead, strStartDay);
    if (sfFoundStock) {
      strRead += _T("&start=");
      strRead += strStartDay;
      strRead += _T("&end=");
      sprintf_s(buffer2, "%8d", gl_lToday);
      strRead += buffer2;
      strRead += gl_strDayLinePostfix;

      gl_stDayLineInquire.strInquire = strRead;
      gl_systemStatus.SetDayLineDataReady(false);
      gl_systemStatus.SetReadingInProcess(true); // 这里多设置一次(线程内也设置），以防线程由于唤醒延迟导致再次进入（线程退出时会清除此标识）
      // 这个线程的启动可以采用唤醒模式而不是这样直接调用
      AfxBeginThread(ClientThreadReadDayLineProc, GetSafeHwnd());
      return true;
    }
    else return false;
  }
  return false;
}

bool CMainFrame::SchedulingTask(void)
{
  static int i10SecondsCounter = 50; // 十秒每次的计算
  static int i1MinuteCounter = 300;  // 一分钟每次的计算
  static bool sfUpdatedStockCodeDataBase = false;

  if (((gl_lTime > 91000) && (gl_lTime < 113500)) || ((gl_lTime > 125500) && (gl_lTime < 150500))) {
    m_fCountDownRT = false;// 只在市场交易时间快速读取实时数据，其他时间则慢速读取
  }
  else m_fCountDownRT = true;

  if ((gl_lTime < 91000) || (gl_lTime > 150001)) { //下午三点市场交易结束
    gl_ChinaStockMarket.m_fMarketOpened = false;
  }
  else gl_ChinaStockMarket.m_fMarketOpened = true;

  if( gl_lTime > 150005)

  if (i10SecondsCounter >= 0) {
    i10SecondsCounter = 50;
    // do something
  }
  else i10SecondsCounter--;

  if (i1MinuteCounter <= 0) {
    i1MinuteCounter = 300;
    if (gl_ChinaStockMarket.IsTotalStockDayLineChecked() && !sfUpdatedStockCodeDataBase) { // 如果所有股票都检查过且存储日线进数据库的线程已经运行结束
      if (!gl_systemStatus.IsDataBaseInProcess()) { // 如果更新日线数据库线程不是活跃状态，则停止日线数据查询。
        // 更新日线数据库线程处于活跃中时，尚有数据没有存储，不能停止查询过程（查询过程能够激活更新线程）
        sfUpdatedStockCodeDataBase = true;
        TRACE("日线历史数据更新完毕\n");
        CString str;
        str = _T("日线历史数据更新完毕");
        gl_systemMessage.PushOutputMessage(str);
        UpdateStockCodeDataBase();  // 更新股票池数据库
        ASSERT(gl_setSavingDayLineOnly.IsOpen());
        gl_setSavingDayLineOnly.Close(); // 关闭日线历史数据存储记录集
        m_fGetDayLineData = false; // 所有需要更新的日线资料都更新过了，不再执行更新日线资料任务
      }
    }
    else {
      if (!gl_systemStatus.IsDataBaseInProcess() && m_fGetDayLineData) {
        gl_systemStatus.SetDataBaseInProcess(true);
        AfxBeginThread(ClientThreadSaveDayLineProc, GetSafeHwnd());
      }
    }

  }
  else i1MinuteCounter--;

  return true;

}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 基类将执行真正的工作

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  static int iCountReadRT = 2;
  static int iCountDayLine = 2;
  static int iCountDown = 0;

  time(&gl_ttime);
  localtime_s(&gl_tm, &gl_ttime);
  gl_lToday = (gl_tm.tm_year + 1900) * 10000 + (gl_tm.tm_mon + 1) * 100 + gl_tm.tm_mday;
  gl_lTime = gl_tm.tm_hour * 10000 + gl_tm.tm_min * 100 + gl_tm.tm_sec;

  //根据时间，调度各项任务
  SchedulingTask();

  if (m_fGetRTStockData && (iCountDown <= 0)) {
    GetSinaStockRTData(); // 每200毫秒申请一次实时数据。新浪的实时行情服务器响应时间不超过100毫秒（30-70之间），且没有出现过数据错误。
    if (m_fCountDownRT && gl_ChinaStockMarket.MarketReady()) iCountDown = 1000; // 完全轮询一遍后，非交易时段一分钟左右更新一次即可 
    else iCountDown = 1;
  }
  iCountDown--;

  if (m_fGetDayLineData && gl_ChinaStockMarket.MarketReady()) {// 如果允许抓取日线数据且系统初始态已经建立
    GetNetEaseStockDayLineData();
  }

  //更新状态条
  if (gl_ChinaStockMarket.IsCurrentStockChanged()) {
    m_wndStatusBar.SetPaneText(2, (LPCTSTR)gl_ChinaStockMarket.m_pCurrentStock->m_strStockCode);
    m_wndStatusBar.SetPaneText(3, (LPCTSTR)gl_ChinaStockMarket.m_pCurrentStock->m_strStockName);
  }
  CString str;
  if (gl_ChinaStockMarket.m_fCurrentEditStockChanged) {
    str = gl_ChinaStockMarket.m_aStockCodeTemp;
    m_wndStatusBar.SetPaneText(1, (LPCTSTR)str);
    gl_ChinaStockMarket.m_fCurrentEditStockChanged = false;
  }
  //更新时间
  char buffer[30];
  sprintf_s(buffer, "%02d:%02d:%02d", gl_tm.tm_hour, gl_tm.tm_min, gl_tm.tm_sec);
  str = buffer;
  m_wndStatusBar.SetPaneText(4, (LPCTSTR)str);

  CMDIFrameWndEx::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当系统退出时，需要先退出工作线程。
//
// 如果是发行版本的话，不允许在开市时或者未处理今日股票数据前退出系统（为了保存正在处理的信息）。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
#ifndef _DEBUG
  // 如果是发行版本，则不允许在开市时或者尚未处理本日股票数据前退出程序
  if (gl_ChinaStockMarket.m_fMarketOpened || !gl_ChinaStockMarket.IsTodayStockCompiled()) {
    return;
  }
#endif
  if ((nID & 0Xfff0) == SC_CLOSE) { // 如果是退出系统
    if (gl_systemStatus.IsDataBaseInProcess()) { // 如果正在处理日线历史数据
      gl_fExiting = true; // 提示各工作线程中途退出

      while (gl_systemStatus.IsDataBaseInProcess()) {
        Sleep(100); // 等待处理日线历史数据的线程退出
      }
    }
  }

  CMDIFrameWndEx::OnSysCommand(nID, lParam);
}

void CMainFrame::OnCalculateRelativeStrong()
{
  // TODO: 在此添加命令处理程序代码
  static bool sfCalculating = false;

  if (sfCalculating) {
    gl_fExitingCalculatingRelativeStrong = true;
    sfCalculating = false;
  }
  else {
    AfxBeginThread(ClientThreadCalculateRelativeStrongProc, GetSafeHwnd());
    sfCalculating = true;
  }
}

void CMainFrame::OnCompileTodayStock()
{
  // TODO: 在此添加命令处理程序代码
  CompileTodayStocks();
}


void CMainFrame::OnDownloadDayline()
{
  // TODO: 在此添加命令处理程序代码
  if (!m_fGetDayLineData) {
    m_fGetDayLineData = true;
  }
  else m_fGetDayLineData = false;
}


void CMainFrame::OnSaveDaylineData()
{
  // TODO: 在此添加命令处理程序代码

}

void CMainFrame::OnSaveRtdata()
{
  // TODO: 在此添加命令处理程序代码
  gl_ChinaStockMarket.SaveRTData();
}

void CMainFrame::OnUpdateCompileTodayStock(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  if (gl_ChinaStockMarket.MarketReady()) { // 系统自动更新日线数据时，不允许处理当日的实时数据。
    pCmdUI->Enable(true);
  }
  else pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateSaveDaylineData(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  //for( auto pStock : gl_sMark)
}




void CMainFrame::OnUpdateCalculateRelativeStrong(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  if (gl_ChinaStockMarket.MarketReady()) {
    pCmdUI->Enable(true);
  }
  else {
    pCmdUI->Enable(false);
  }
}

