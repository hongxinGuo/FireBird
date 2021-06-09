////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
/////////////////////////////////////////////////////////////////////////////////////

// GoogleMock使用方法：
//
//   ON_CALL(mock_object, Method(...))
//     .With(...) ?
//     .WillByDefault(...);
//
// where With() is optional and WillByDefault() must appear exactly
// once.
//
//   EXPECT_CALL(mock_object, Method(...))
//     .With(...) ?
//     .Times(...) ?
//     .InSequence(...) *
//     .WillOnce(...) *
//     .WillRepeatedly(...) ?
//     .RetiresOnSaturation() ? ;
//
// where all clauses are optional and WillOnce() can be repeated.

#include"pch.h"

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

#ifndef __GOOGLEMOCK__
#error _T("本系统必须使用GOOGLE MOCK")
#endif

// 排除外部代码
#include<CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)
ExcludeSourceFromCodeCoverage(Exclude1, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.*"); // 排除GoogleTest
ExcludeSourceFromCodeCoverage(Exclude2, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\Packages\\*.*"); // 排除扩展程序包
ExcludeSourceFromCodeCoverage(Exclude3, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\DataBase\\*.*"); // 排除数据库类
//ExcludeSourceFromCodeCoverage(Exclude4, L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\*.*"); // 排除VS系统库
ExcludeSourceFromCodeCoverage(Exclude4, L"C:\\Program Files (x86)\\*.*"); // 排除VS系统库
ExcludeSourceFromCodeCoverage(Exclude5, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\gsl\\*.*"); // 排除gsl库

ExcludeSourceFromCodeCoverage(Exclude6, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\Poten Daily\\*.*"); // 排除PotenDaily Market.此市场不再使用
ExcludeSourceFromCodeCoverage(Exclude7, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\Crweber\\*.*"); // 排除Crweber.com.此市场不再使用
ExcludeSourceFromCodeCoverage(Exclude8, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\AmericaMarket\\DataBase\\*.*"); // 排除WorldMarket的数据库类

// 排除无需测试的单个文件
ExcludeSourceFromCodeCoverage(Exclude101, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.cpp");
ExcludeSourceFromCodeCoverage(Exclude102, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.h");
ExcludeSourceFromCodeCoverage(Exclude103, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysisDoc.cpp");
ExcludeSourceFromCodeCoverage(Exclude104, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.h");
ExcludeSourceFromCodeCoverage(Exclude105, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.cpp");
ExcludeSourceFromCodeCoverage(Exclude106, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.h");
ExcludeSourceFromCodeCoverage(Exclude107, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.cpp");
ExcludeSourceFromCodeCoverage(Exclude108, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.h");

//  排除单个类或单个类函数
ExcludeFromCodeCoverage(CMainFrame1, L"CMainFrame::AssertValid");
ExcludeFromCodeCoverage(CMainFrame2, L"CMainFrame::Dump");
ExcludeFromCodeCoverage(CMainFrame3, L"CMainFrame::GetBaseClass");
ExcludeFromCodeCoverage(CMainFrame4, L"CMainFrame::GetMessageMap");
ExcludeFromCodeCoverage(CMainFrame5, L"CMainFrame::GetRuntimeClass");
ExcludeFromCodeCoverage(CMainFrame6, L"CMainFrame::GetThisClass");
ExcludeFromCodeCoverage(CMainFrame7, L"CMainFrame::GetThisMessageMap");
ExcludeFromCodeCoverage(CMainFrame8, L"CMainFrame::LoadFrame");
ExcludeFromCodeCoverage(CMainFrame9, L"CMainFrame::OnCreate");
ExcludeFromCodeCoverage(CMainFrame10, L"CMainFrame::OnToolbarCreateNew");
ExcludeFromCodeCoverage(CMainFrame11, L"CMainFrame::OnViewCustomize");
ExcludeFromCodeCoverage(CMainFrame12, L"CMainFrame::OnWindowManager");
ExcludeFromCodeCoverage(CMainFrame13, L"CMainFrame::PreCreateWindow");
ExcludeFromCodeCoverage(CMainFrame14, L"CMainFrame::SysCallCmdUIEnable");
ExcludeFromCodeCoverage(CMainFrame15, L"CMainFrame::SysCallCmdUISetCheck");
ExcludeFromCodeCoverage(CMainFrame16, L"CMainFrame::SysCallInvalidate");
ExcludeFromCodeCoverage(CMainFrame17, L"CMainFrame::SysCallOnChar");
ExcludeFromCodeCoverage(CMainFrame18, L"CMainFrame::SysCallOnKeyUp");
ExcludeFromCodeCoverage(CMainFrame19, L"CMainFrame::SysCallOnTimer");
ExcludeFromCodeCoverage(CMainFrame20, L"CMainFrame::SysCallPreTranslateMessage");
ExcludeFromCodeCoverage(CMainFrame21, L"CMainFrame::SysCallSendMessage");
ExcludeFromCodeCoverage(CMainFrame22, L"CMainFrame::SysCallSetPaneText");
ExcludeFromCodeCoverage(CMainFrame23, L"CMainFrame::SetDockingWindowIcons");
ExcludeFromCodeCoverage(CMainFrame26, L"CMainFrame::CreateDockingWindows");

ExcludeFromCodeCoverage(CStockAnalysisView1, L"CStockAnalysisView::AssertValid");
ExcludeFromCodeCoverage(CStockAnalysisView2, L"CStockAnalysisView::CreateObject");
ExcludeFromCodeCoverage(CStockAnalysisView3, L"CStockAnalysisView::Dump");
ExcludeFromCodeCoverage(CStockAnalysisView4, L"CStockAnalysisView::GetBaseClass");
ExcludeFromCodeCoverage(CStockAnalysisView5, L"CStockAnalysisView::GetDocument");
ExcludeFromCodeCoverage(CStockAnalysisView6, L"CStockAnalysisView::GetMessageMap");
ExcludeFromCodeCoverage(CStockAnalysisView7, L"CStockAnalysisView::GetRuntimeClass");
ExcludeFromCodeCoverage(CStockAnalysisView8, L"CStockAnalysisView::GetThisClass");
ExcludeFromCodeCoverage(CStockAnalysisView9, L"CStockAnalysisView::GetThisMessageMap");
ExcludeFromCodeCoverage(CStockAnalysisView10, L"CStockAnalysisView::OnBeginPrinting");
ExcludeFromCodeCoverage(CStockAnalysisView11, L"CStockAnalysisView::OnContextMenu");
ExcludeFromCodeCoverage(CStockAnalysisView12, L"CStockAnalysisView::OnEndPrinting");
ExcludeFromCodeCoverage(CStockAnalysisView13, L"CStockAnalysisView::OnFilePrintPreview");
ExcludeFromCodeCoverage(CStockAnalysisView14, L"CStockAnalysisView::OnPreparePrinting");
ExcludeFromCodeCoverage(CStockAnalysisView15, L"CStockAnalysisView::PreCreateWindow");
ExcludeFromCodeCoverage(CStockAnalysisView16, L"CStockAnalysisView::SysCallCmdUIEnable");
ExcludeFromCodeCoverage(CStockAnalysisView17, L"CStockAnalysisView::SysCallCmdUISetCheck");
ExcludeFromCodeCoverage(CStockAnalysisView18, L"CStockAnalysisView::SysCallFillSolidRect");
ExcludeFromCodeCoverage(CStockAnalysisView19, L"CStockAnalysisView::SysCallGetTextExtent");
ExcludeFromCodeCoverage(CStockAnalysisView20, L"CStockAnalysisView::SysCallLineTo");
ExcludeFromCodeCoverage(CStockAnalysisView21, L"CStockAnalysisView::SysCallMoveTo");
ExcludeFromCodeCoverage(CStockAnalysisView22, L"CStockAnalysisView::SysCallOnSize");
ExcludeFromCodeCoverage(CStockAnalysisView23, L"CStockAnalysisView::SysCallSelectObject");
ExcludeFromCodeCoverage(CStockAnalysisView24, L"CStockAnalysisView::SysCallTextOut");
ExcludeFromCodeCoverage(CStockAnalysisView25, L"CStockAnalysisView::SysCallGetClientRect");
ExcludeFromCodeCoverage(CStockAnalysisView26, L"CStockAnalysisView::OnCreate");
ExcludeFromCodeCoverage(CStockAnalysisView27, L"CStockAnalysisView::ShowAttackBuySell");
ExcludeFromCodeCoverage(CStockAnalysisView28, L"CStockAnalysisView::ShowBuySell");
ExcludeFromCodeCoverage(CStockAnalysisView29, L"CStockAnalysisView::ShowCurrentTransactionInfo");
ExcludeFromCodeCoverage(CStockAnalysisView30, L"CStockAnalysisView::ShowGuadan");
ExcludeFromCodeCoverage(CStockAnalysisView31, L"CStockAnalysisView::ShowRealtimeData");
ExcludeFromCodeCoverage(CStockAnalysisView32, L"CStockAnalysisView::ShowRealtimeGuadan");
ExcludeFromCodeCoverage(CStockAnalysisView33, L"CStockAnalysisView::ShowStockDayLine");
ExcludeFromCodeCoverage(CStockAnalysisView34, L"CStockAnalysisView::OnDraw");
ExcludeFromCodeCoverage(CStockAnalysisView35, L"CStockAnalysisView::OnRButtonUp");
ExcludeFromCodeCoverage(CStockAnalysisView36, L"CStockAnalysisView::Show");

ExcludeFromCodeCoverage(CVirtualMarket1, L"CVirtualMarket::AssertValid");
ExcludeFromCodeCoverage(CVirtualMarket2, L"CVirtualMarket::Dump");
ExcludeFromCodeCoverage(CVirtualMarket3, L"CVirtualMarket::ResetMarket");

ExcludeFromCodeCoverage(CChinaMarket1, L"CChinaMarket::AssertValid");
ExcludeFromCodeCoverage(CChinaMarket2, L"CChinaMarket::Dump");
ExcludeFromCodeCoverage(CChinaMarket3, L"CChinaMarket::RunningThreadAppendChoicedStockDB");
ExcludeFromCodeCoverage(CChinaMarket4, L"CChinaMarket::RunningThreadCalculateRS");
ExcludeFromCodeCoverage(CChinaMarket5, L"CChinaMarket::RunningThreadCalculateThisDaysRS");
ExcludeFromCodeCoverage(CChinaMarket6, L"CChinaMarket::RunningThreadChoice10RSStrong1StockSet");
ExcludeFromCodeCoverage(CChinaMarket7, L"CChinaMarket::RunningThreadChoice10RSStrong2StockSet");
ExcludeFromCodeCoverage(CChinaMarket8, L"CChinaMarket::RunningThreadChoice10RSStrongStockSet");
ExcludeFromCodeCoverage(CChinaMarket9, L"CChinaMarket::RunningThreadLoadDayLine");
ExcludeFromCodeCoverage(CChinaMarket10, L"CChinaMarket::RunningThreadProcessTodayStock");
ExcludeFromCodeCoverage(CChinaMarket11, L"CChinaMarket::RunningThreadSaveChoicedRTData");
ExcludeFromCodeCoverage(CChinaMarket12, L"CChinaMarket::RunningThreadSaveDayLineOfOneStock");
ExcludeFromCodeCoverage(CChinaMarket13, L"CChinaMarket::RunningThreadSaveTempRTData");
ExcludeFromCodeCoverage(CChinaMarket14, L"CChinaMarket::RunningThreadUpdateOptionDB");
ExcludeFromCodeCoverage(CChinaMarket15, L"CChinaMarket::RunningThreadUpdateStockCodeDB");
ExcludeFromCodeCoverage(CChinaMarket16, L"CChinaMarket::RunningThreadCalculate10RSStrongStock");
ExcludeFromCodeCoverage(CChinaMarket17, L"CChinaMarket::RunningThreadCalculate10RSStrong1Stock");
ExcludeFromCodeCoverage(CChinaMarket18, L"CChinaMarket::RunningThreadCalculate10RSStrong2Stock");
ExcludeFromCodeCoverage(CChinaMarket19, L"CChinaMarket::RunningThreadBuildDayLineRS");
ExcludeFromCodeCoverage(CChinaMarket20, L"CChinaMarket::RunningThreadBuildDayLineRSOfDate");
ExcludeFromCodeCoverage(CChinaMarket21, L"CChinaMarket::RunningThreadBuildWeekLine");
ExcludeFromCodeCoverage(CChinaMarket22, L"CChinaMarket::RunningThreadBuildWeekLineOfStock");
ExcludeFromCodeCoverage(CChinaMarket23, L"CChinaMarket::RunningThreadBuildWeekLineRS");
ExcludeFromCodeCoverage(CChinaMarket24, L"CChinaMarket::RunningThreadBuildWeekLineRSOfDate");
ExcludeFromCodeCoverage(CChinaMarket25, L"CChinaMarket::RunningThreadBuildWeekLineOfCurrentWeek");
ExcludeFromCodeCoverage(CChinaMarket26, L"CChinaMarket::RunningThreadBuildCurrentWeekWeekLineTable");
ExcludeFromCodeCoverage(CChinaMarket27, L"CChinaMarket::RunningThreadLoadWeekLine");
ExcludeFromCodeCoverage(CChinaMarket28, L"CChinaMarket::RunningThreadSaveDayLineBasicInfoOfStock");
ExcludeFromCodeCoverage(CChinaMarket29, L"CChinaMarket::RunningThreadSaveStockSection");
ExcludeFromCodeCoverage(CChinaMarket30, L"CChinaMarket::RunningThreadUpdateStockCodeDB");
ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::RunningThreadUpdateOptionDB");

ExcludeFromCodeCoverage(CChinaStock1, L"CChinaStock::AssertValid");
ExcludeFromCodeCoverage(CChinaStock2, L"CChinaStock::Dump");
ExcludeFromCodeCoverage(CChinaStock3, L"CChinaStock::Get1DaysRS");
ExcludeFromCodeCoverage(CChinaStock4, L"CChinaStock::Get3DaysRS");
ExcludeFromCodeCoverage(CChinaStock5, L"CChinaStock::Get5DaysRS");
ExcludeFromCodeCoverage(CChinaStock6, L"CChinaStock::Get10DaysRS");
ExcludeFromCodeCoverage(CChinaStock7, L"CChinaStock::Get30DaysRS");
ExcludeFromCodeCoverage(CChinaStock8, L"CChinaStock::Get60DaysRS");
ExcludeFromCodeCoverage(CChinaStock9, L"CChinaStock::Get120DaysRS");
ExcludeFromCodeCoverage(CChinaStock10, L"CChinaStock::GetRSIndex1Day");
ExcludeFromCodeCoverage(CChinaStock11, L"CChinaStock::GetRSLogarithm1Day");
ExcludeFromCodeCoverage(CChinaStock12, L"CChinaStock::ReportGuadan");
ExcludeFromCodeCoverage(CChinaStock14, L"CChinaStock::ShowDayLine");
ExcludeFromCodeCoverage(CChinaStock15, L"CChinaStock::Calculate10RSStrong1StockSet");
ExcludeFromCodeCoverage(CChinaStock16, L"CChinaStock::Calculate10RSStrong2StockSet");
ExcludeFromCodeCoverage(CChinaStock17, L"CChinaStock::Calculate10RSStrongStockSet");

ExcludeFromCodeCoverage(CPotenDailyBriefingMarket1, L"CPotenDailyBriefingMarket::RunningThreadSavePotenData");
ExcludeFromCodeCoverage(CPotenDailyBriefingMarket2, L"CPotenDailyBriefingMarket::ShedulingTask");

ExcludeFromCodeCoverage(CCrweberIndexMarket1, L"CCrweberIndexMarket::RunningThreadMaintainDB");
ExcludeFromCodeCoverage(CCrweberIndexMarket2, L"CCrweberIndexMarket::RunningThreadSaveCrweberDB");
ExcludeFromCodeCoverage(CCrweberIndexMarket3, L"CCrweberIndexMarket::ShedulingTaskPer1Hour");
ExcludeFromCodeCoverage(CCrweberIndexMarket4, L"CCrweberIndexMarket::ShedulingTaskPerSecond");
ExcludeFromCodeCoverage(CCrweberIndexMarket5, L"CCrweberIndexMarket::ShedulingTask");

ExcludeFromCodeCoverage(CWorldMarket1, L"CWorldMarket::RunningthreadUpdateDayLneStartEndDate");
ExcludeFromCodeCoverage(CWorldMarket2, L"CWorldMarket::RunningThreadUpdateDayLineDB");
ExcludeFromCodeCoverage(CWorldMarket3, L"CWorldMarket::RunningThreadUpdateStockProfileDB");
ExcludeFromCodeCoverage(CWorldMarket4, L"CWorldMarket::RunningThreadUpdateForexDayLineDB");
ExcludeFromCodeCoverage(CWorldMarket5, L"CWorldMarket::RunningThreadUpdateForexSymbolDB");
ExcludeFromCodeCoverage(CWorldMarket6, L"CWorldMarket::RunningThreadUpdateCountryListDB");
ExcludeFromCodeCoverage(CWorldMarket7, L"CWorldMarket::RunningThreadUpdateEPSSurpriseDB");
ExcludeFromCodeCoverage(CWorldMarket8, L"CWorldMarket::RunningThreadUpdateTiingoStockDB");
ExcludeFromCodeCoverage(CWorldMarket9, L"CWorldMarket::RunningThreadUpdateTiingoIndustry");
ExcludeFromCodeCoverage(CWorldMarket10, L"CWorldMarket::RunningThreadUpdateSICIndustry");
ExcludeFromCodeCoverage(CWorldMarket11, L"CWorldMarket::RunningThreadUpdateNaicsIndustry");
ExcludeFromCodeCoverage(CWorldMarket12, L"CWorldMarket::RunningThreadUpdateInsiderTransactionDB");
ExcludeFromCodeCoverage(CWorldMarket13, L"CWorldMarket::RunningThreadUpdateForexExchangeDB");
ExcludeFromCodeCoverage(CWorldMarket14, L"CWorldMarket::RunningThreadUpdateEconomicCalendarDB");

ExcludeFromCodeCoverage(CCrweberIndex1, L"CCrweberIndex::ReadData");
ExcludeFromCodeCoverage(CCrweberIndex2, L"CCrweberIndex::RunningMaintainDB");

ExcludeFromCodeCoverage(CRTData1, L"CWebRTData::AssertValid");
ExcludeFromCodeCoverage(CRTData2, L"CWebRTData::Dump");

ExcludeFromCodeCoverage(CSystemMessage2, L"CSystemMessage::DisplayCancelBuy");
ExcludeFromCodeCoverage(CSystemMessage3, L"CSystemMessage::DisplayCancelSell");
ExcludeFromCodeCoverage(CSystemMessage4, L"CSystemMessage::DisplayDayLineInfo");
ExcludeFromCodeCoverage(CSystemMessage5, L"CSystemMessage::DisplayInformation");
ExcludeFromCodeCoverage(CSystemMessage6, L"CSystemMessage::DisplayInnerSystemInformation");
ExcludeFromCodeCoverage(CSystemMessage7, L"CSystemMessage::DisplayTrace2");
ExcludeFromCodeCoverage(CSystemMessage8, L"CSystemMessage::DisplayTransaction");

ExcludeFromCodeCoverage(CNeteaseRTWebInquiry1, L"CNeteaseRTWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CTengxunRTWebInquiry1, L"CTengxunRTWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CSinaRTWebInquiry1, L"CSinaRTWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CCrweberIndexWebInquiry1, L"CCrweberIndexWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CNeteaseDayLineWebInquiry1, L"CNeteaseDayLineWebInquiry::StartReadingThread");

ExcludeFromCodeCoverage(CPotenDailyBriefingWebInquiry1, L"CPotenDailyBriefingWebInquiry::StartReadingThread");

ExcludeFromCodeCoverage(CSystemDeque1, L"CSystemDeque::SysCallOutputListAddString");

ExcludeFromCodeCoverage(CWebInquiry1, L"CWebInquiry::Initialize");
ExcludeFromCodeCoverage(CWebInquiry2, L"CWebInquiry::~CWebInquirer");

ExcludeFromCodeCoverage(CWebInquirer1, L"CWebInquirer::Initialize");

ExcludeFromCodeCoverage(CVirtualWebInquiry1, L"CVirtualWebInquiry::PrepareNextInquiringStr");
ExcludeFromCodeCoverage(CVirtualWebInquiry2, L"CVirtualWebInquiry::GetNextInquiringMiddleStr");
ExcludeFromCodeCoverage(CVirtualWebInquiry3, L"CVirtualWebInquiry::ReadWebFileOneTime");
ExcludeFromCodeCoverage(CVirtualWebInquiry4, L"CVirtualWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CTiingoWebInquiry1, L"CTiingoWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CFinnhubWebInquiry1, L"CFinnhubWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CQuandlWebInquiry1, L"CQuandlWebInquiry::StartReadingThread");

ExcludeFromCodeCoverage(CChinaStockHistoryDataContainer1, L"CChinaStockHistoryDataContainer::ShowData");

#pragma managed(pop)

#include"EnvironmentSetUp.h"

using namespace StockAnalysisTest;

int main(int argc, char* argv[]) {
  //_CrtSetBreakAlloc(1760377);
  //_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_MAP_ALLOC);
  _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & ~_CRTDBG_ALLOC_MEM_DF);

  ::testing::InitGoogleTest(&argc, argv);
  // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);

  ASSERT(!gl_fNormalMode);
  ASSERT(gl_fTestMode);

  //_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF);
  _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

  return RUN_ALL_TESTS();
}