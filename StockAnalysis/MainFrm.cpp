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

#include"Thread.h"

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
  ON_WM_CHAR()
  ON_WM_KEYUP()
  //ON_COMMAND(ID_SHOW_ORDINARY_BUY, &CMainFrame::OnShowOrdinaryBuy)
  //ON_COMMAND(ID_SHOW_ORDINARY_SELL, &CMainFrame::OnShowOrdinarySell)
  //ON_COMMAND(ID_SHOW_STRONG_BUY, &CMainFrame::OnShowStrongBuy)
  //ON_COMMAND(ID_SHOW_STRONG_SELL, &CMainFrame::OnShowStrongSell)
  //ON_COMMAND(ID_SHOW_UNKNOWNBUYSELL, &CMainFrame::OnShowUnknownbuysell)
  //ON_COMMAND(ID_SHOW_ATTACK_BUY, &CMainFrame::OnShowAttackBuy)
  //ON_COMMAND(ID_SHOW_ATTACK_SELL, &CMainFrame::OnShowAttackSell)
  ON_COMMAND(ID_REBUILD_DAYLINE_RS, &CMainFrame::OnRebuildDaylineRs)
  ON_COMMAND(ID_BUILD_RESET_SYSTEM, &CMainFrame::OnBuildResetSystem)
  ON_UPDATE_COMMAND_UI(ID_REBUILD_DAYLINE_RS, &CMainFrame::OnUpdateRebuildDaylineRs)
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // 状态行指示器
  ID_CURRENT_INPUT,
  ID_CURRENT_SELECT_STOCK,
  ID_CURRENT_SELECT_STOCKNAME,
  ID_CURRENT_RTDATA_READING_TIME,
  ID_CURRENT_ACTIVE_STOCK,
  ID_CURRENT_DAYLINE_READING_TIME,
  ID_CURRENT_TIME,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
  // TODO: 在此添加成员初始化代码

  m_uIdTimer = 0;

  gl_systemMessage.PushInformationMessage(_T("系统初始化中....."));

  gl_fNormalMode = true; // 实际系统，测试状态为假。

  // 这个记录集需要在系统启动时就打开，且一直保持打开的状态。但需要使用MySQL驱动而不是MySQLTest驱动，故而在设置gl_fTestMode为假后才执行。
  // 真实系统中，所有的记录集都要使用MySQL驱动而不是MySQLTest驱动，而测试函数都要使用MySQLTest驱动。
  // 这两个驱动的权限不同，MySQLTest只有检索权限，无权修改数据，这样在测试时能够防止误操作数据。
  gl_ChinaStockMarket.OpenSavingDayLineRecord();

  Reset();
}

void CMainFrame::Reset(void)
{
  // 在此之前已经准备好了全局股票池（在CMarket的构造函数中）。

  // 这两个操作记录集的函数也需要位于设置gl_fTestMode之后。
  gl_ChinaStockMarket.LoadStockCodeDataBase();
  gl_ChinaStockMarket.LoadOptionDataBase();

  // 设置股票日线查询环境
  gl_systemTime.CalculateTime();
  gl_systemTime.CalculateLastTradeDay();

  const long lTemp = gl_systemDequeData.GetRTDataDequeSize();
  for (int i = 0; i < lTemp; i++) {
    CStockRTDataPtr pRTData = gl_systemDequeData.PopRTData();
  }

  m_lCurrentPos = 0;
}

CMainFrame::~CMainFrame()
{
  if (gl_fInTestMode) TRACE("使用了Test驱动\n");

  gl_fExiting = true;

  gl_ChinaStockMarket.UpdateOptionDataBase();

  while (gl_ThreadStatus.IsSavingDayLineInProcess()) {
    Sleep(10); // 等待处理日线历史数据的线程结束。
  }

  while (gl_ThreadStatus.IsSinaRTDataReadingInProcess()) {
    Sleep(10); // 等待实时数据读取线程结束
  }

  // 更新股票代码数据库要放在最后，等待存储日线数据的线程（如果唤醒了的话）结束之后再执行。
  // 因为彼线程也在更新股票代码数据库，而此更新只是消除同类项而已。
  gl_ChinaStockMarket.SaveStockCodeDataBase(); // 这里直接调用存储函数，不采用工作线程的模式。

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
  m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

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

  //m_wndOutput2.EnableDocking(CBRS_ALIGN_ANY);
  //DockPane(&m_wndOutput2);

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

  // 设置100毫秒每次的软调度，用于接受处理实时网络数据。目前新浪股票接口的实时数据更新频率为每三秒一次，故而400毫秒（200X2）读取900个股票就足够了。
  m_uIdTimer = SetTimer(1, 100, nullptr);     // 100毫秒每次调度，用于调度各类定时处理任务。
  if (m_uIdTimer == 0) {
    CString str;
  }
  return 0;
}

bool CMainFrame::ResetSystem(void)
{
  CString str;
  TRACE(_T("开始重置系统\n"));
  str = _T("重置系统");
  gl_systemMessage.PushInformationMessage(str);
  gl_ChinaStockMarket.Reset();
  Reset();
  TRACE(_T("重置系统结束\n"));
  return false;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CMDIFrameWndEx::PreCreateWindow(cs))
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
  /*
  CString strOutputWnd2;
  bNameValid = strOutputWnd2.LoadString(IDS_OUTPUT_WND2);
  ASSERT(bNameValid);
  if (!m_wndOutput2.Create(strOutputWnd2, this, CRect(100, 100, 200, 200), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
  {
    TRACE0("未能创建输出窗口\n");
    return FALSE; // 未能创建
  }
  */

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

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
  LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp, lp);
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

  for (int i = 0; i < iMaxUserToolbars; i++)
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

///////////////////////////////////////////////////////////////////////////////////////////
//
// 大部分系统定时操作的发起者，但具体工作由CMarket类的SchedulingTask()完成，本函数只完成显示实时信息的工作。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  // 重启系统在此处执行，容易调用各重置函数
  CString str;

  if (gl_fResetSystem) {
    while (gl_ThreadStatus.IsCalculatingRSThreadRunning() || gl_ThreadStatus.IsCalculatingRTData() || gl_ThreadStatus.IsSavingTempData()
      || gl_ThreadStatus.IsSavingDayLineInProcess()) {
      Sleep(10);
    }
    ResetSystem();
    gl_fResetSystem = false;
  }

  // 调用主调度函数。系统更新任务由CMarket类中的调度函数完成，CMainFrame只执行更新状态任务
  gl_ChinaStockMarket.SchedulingTask();

  //更新状态条
  if (gl_ChinaStockMarket.IsCurrentStockChanged()) {
    m_wndStatusBar.SetPaneText(2, (LPCTSTR)gl_ChinaStockMarket.m_pCurrentStock->GetStockCode());
    m_wndStatusBar.SetPaneText(3, (LPCTSTR)gl_ChinaStockMarket.m_pCurrentStock->GetStockName());
  }

  if (gl_ChinaStockMarket.m_fCurrentEditStockChanged) {
    str = gl_ChinaStockMarket.m_aStockCodeTemp;
    m_wndStatusBar.SetPaneText(1, (LPCTSTR)str);
    gl_ChinaStockMarket.m_fCurrentEditStockChanged = false;
  }
  // 显示实时数据读取时间（单位为毫秒）
  char buffer[30];
  sprintf_s(buffer, "%d", gl_ChinaStockMarket.gl_RTReadingTime);
  str = buffer;
  str += _T("ms");
  m_wndStatusBar.SetPaneText(4, (LPCTSTR)str);

  // 显示活跃股票总数
  sprintf_s(buffer, "%d", gl_ChinaStockMarket.GetTotalStock());
  str = buffer;
  m_wndStatusBar.SetPaneText(5, (LPCTSTR)str);

  // 显示日线历史数据读取时间（单位为毫秒）
  sprintf_s(buffer, "%d", gl_ChinaStockMarket.gl_DayLineReadingTime);
  str = buffer;
  str += _T("ms");
  m_wndStatusBar.SetPaneText(6, (LPCTSTR)str);

  //更新时间
  m_wndStatusBar.SetPaneText(7, (LPCTSTR)gl_systemTime.GetTimeString());

  if (gl_fInTestMode) {
    str = _T("警告：使用了Test驱动");
    gl_systemMessage.PushInformationMessage(str);
  }

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
  if ((nID & 0Xfff0) == SC_CLOSE) { // 如果是退出系统
#ifndef _DEBUG
    // 如果是发行版本，则不允许在开市时或者尚未处理本日股票数据前退出程序（此功能目前不再使用了。因现在可以每分钟存储临时数据）
    if (gl_ChinaStockMarket.m_fMarketOpened || !gl_ChinaStockMarket.IsTodayStockCompiled()) {
      //return; // 无动作
    }
#endif
    gl_fExiting = true; // 提示各工作线程中途退出
    if (gl_ThreadStatus.IsSavingDayLineInProcess()) { // 如果正在处理日线历史数据
      while (gl_ThreadStatus.IsSavingDayLineInProcess()) {
        Sleep(10); // 等待处理日线历史数据的线程退出
      }
    }
  }

  CMDIFrameWndEx::OnSysCommand(nID, lParam);
}

void CMainFrame::OnCalculateRelativeStrong()
{
  // TODO: 在此添加命令处理程序代码
  gl_ChinaStockMarket.SetCalculatingRS(true);
  AfxBeginThread(ThreadCalculateRelativeStrongProc, nullptr);
}

void CMainFrame::OnCompileTodayStock()
{
  // TODO: 在此添加命令处理程序代码
  if (gl_ChinaStockMarket.SystemReady()) {
    AfxBeginThread(ThreadCompileTodayStocks, nullptr);
  }
}

void CMainFrame::OnDownloadDayline()
{
  // TODO: 在此添加命令处理程序代码
  if (!gl_ChinaStockMarket.m_fGetDayLineData) {
    gl_ChinaStockMarket.m_fGetDayLineData = true;
  }
  else gl_ChinaStockMarket.m_fGetDayLineData = false;
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

void CMainFrame::OnUpdateCompileTodayStock(CCmdUI* pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  if (gl_ChinaStockMarket.SystemReady()) { // 系统自动更新日线数据时，不允许处理当日的实时数据。
    pCmdUI->Enable(true);
  }
  else pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateSaveDaylineData(CCmdUI* pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  //for( auto pStock : gl_sMark)
}

void CMainFrame::OnUpdateCalculateRelativeStrong(CCmdUI* pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  if (gl_ChinaStockMarket.SystemReady()) {
    if (gl_ThreadStatus.IsCalculateDayLineRS()) {
      pCmdUI->Enable(false);
    }
    else {
      pCmdUI->Enable(true);
    }
  }
  else {
    pCmdUI->Enable(false);
  }
}

///////////////////////////////////////////////////////////////////
//
// 转发WM_CHAR和WM_KEYUP梁消息至本主框架，由主框架处理之。
//
//
//////////////////////////////////////////////////////////////////
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
  // TODO: 在此添加专用代码和/或调用基类
  if ((pMsg->message == WM_CHAR) || (pMsg->message == WM_KEYUP)) {
    SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
  }

  return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  StockIDPtr pStockID;
  CStockPtr pStock;
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
  case 's':
  case 'h':
  case 'z':
    if (m_lCurrentPos < 10) {
      gl_ChinaStockMarket.m_aStockCodeTemp[m_lCurrentPos] = nChar;
      m_lCurrentPos++;
      gl_ChinaStockMarket.m_aStockCodeTemp[m_lCurrentPos] = 0x000;
    }
    gl_ChinaStockMarket.m_fCurrentEditStockChanged = true;
    break;
  case 0x00d: // 回车
    strTemp = gl_ChinaStockMarket.m_aStockCodeTemp;
    if (gl_ChinaStockMarket.IsStock(strTemp, pStock)) {
      gl_ChinaStockMarket.SetShowStock(pStock);
      //m_fNeedUpdateTitle = true;
      Invalidate();
    }
    gl_ChinaStockMarket.m_aStockCodeTemp[0] = 0x000;
    m_lCurrentPos = 0;
    gl_ChinaStockMarket.m_fCurrentEditStockChanged = true;
    break;
  case 0x008: // back space
    if (m_lCurrentPos > 0) {
      m_lCurrentPos--;
      gl_ChinaStockMarket.m_aStockCodeTemp[m_lCurrentPos] = 0x000;
    }
    gl_ChinaStockMarket.m_fCurrentEditStockChanged = true;
    break;
  default:
    break;
  }

  CMDIFrameWndEx::OnChar(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  CStockPtr pStock;
  long lIndex = 0;
  CString strTemp;
  StockIDPtr pStockID;

  if (gl_ChinaStockMarket.m_pCurrentStock != nullptr) {
    switch (nChar) {
    case 45: // Ins 加入自选股票
      pStock = gl_ChinaStockMarket.GetShowStock();
      pStock->SetChoicedFlag(true);
      if (gl_ChinaStockMarket.GetStockIDPtr(pStock->GetStockCode(), pStockID)) {
        gl_ChinaStockMarket.gl_vStockChoice.push_back(pStockID);
      }
      break;
    case 33: // PAGE UP
      // last stock
      pStock = gl_ChinaStockMarket.GetShowStock();
      gl_ChinaStockMarket.GetStockIndex(pStock->GetStockCode(), lIndex);
      if (lIndex > 0) lIndex--;
      pStock = gl_ChinaStockMarket.GetStockPtr(lIndex);
      gl_ChinaStockMarket.SetShowStock(pStock);
      //m_fNeedUpdateTitle = true;
      break;
    case 34: // PAGE DOWN
      // next stock
      pStock = gl_ChinaStockMarket.GetShowStock();
      gl_ChinaStockMarket.GetStockIndex(pStock->GetStockCode(), lIndex);
      if (lIndex < gl_ChinaStockMarket.GetTotalStock()) lIndex++;
      pStock = gl_ChinaStockMarket.GetStockPtr(lIndex);
      gl_ChinaStockMarket.SetShowStock(pStock);
      //m_fNeedUpdateTitle = true;
      break;
    case 0x00d: // 回车
      strTemp = gl_ChinaStockMarket.m_aStockCodeTemp;
      if (gl_ChinaStockMarket.IsStock(strTemp, pStock)) {
        gl_ChinaStockMarket.SetShowStock(pStock);
        //m_fNeedUpdateTitle = true;
        Invalidate();
      }
      gl_ChinaStockMarket.m_aStockCodeTemp[0] = 0x000;
      m_lCurrentPos = 0;
      gl_ChinaStockMarket.m_fCurrentEditStockChanged = true;
      break;
    default:
      break;
    }
  }

  CMDIFrameWndEx::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnRebuildDaylineRs()
{
  // TODO: Add your command handler code here

  gl_ChinaStockMarket.SetRelativeStrongEndDay(19900101);
  gl_ChinaStockMarket.SetRelativeStrongStartDay(19900101);
  gl_ChinaStockMarket.SetCalculatingRS(true);
  AfxBeginThread(ThreadCalculateRelativeStrongProc, nullptr);
}

void CMainFrame::OnBuildResetSystem()
{
  // TODO: Add your command handler code here
  gl_fResetSystem = true;
}

void CMainFrame::OnUpdateRebuildDaylineRs(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  // 要避免在八点至半九点半之间执行重算相对强度的工作，因为此时间段时要重置系统，结果导致程序崩溃。
  if ((gl_systemTime.GetTime() > 83000) && (gl_systemTime.GetTime() < 93000) ) {
    pCmdUI->Enable(false);
  }
  else if (gl_ThreadStatus.IsCalculateDayLineRS()) {
    pCmdUI->Enable(false);
  }
  else {
    pCmdUI->Enable(true);
  }
}