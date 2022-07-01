////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
/////////////////////////////////////////////////////////////////////////////////////

// GoogleMock使用方法：
//
//   ON_CALL(mock_object, method_name(matchers...))
//     .With(multi_argument_matcher)  // Can be used at most once
//     .WillByDefault(action);        // Required
//
// where With() is optional and WillByDefault() must appear exactly
// once.
//
// EXPECT_CALL(mock_object, method_name(matchers...))
//   .With(multi_argument_matcher)  // Can be used at most once
//   .Times(cardinality)            // Can be used at most once
//   .InSequence(sequences...)      // Can be used any number of times
//   .After(expectations...)        // Can be used any number of times
//   .WillOnce(action)              // Can be used any number of times
//   .WillRepeatedly(action)        // Can be used at most once
//   .RetiresOnSaturation();        // Can be used at most once
//
// where all clauses are optional and WillOnce() can be repeated.
//

// 可以使用Virtual leak detector(vld)查找内存泄漏。
// 由于vld官方源码支持只到vs2015，故而需要手工修改源代码，才能使用VS2022编译。
// 且dbghelp.dll需要使用目录C:\Program Files\Microsoft Visual Studio\Shared\Common\VSPerfCollectionTools\vs2022中的那个版本
// 需要将vld\VS2022文件夹中的文件dbghelp.dll Microsoft.DTfW.DHL.manifest vld_x64.dll三个文件拷贝至执行文件夹(X64\Release)中
//
//#include"vld.h" // 需要检测内存泄漏时包括此头文件

#include"pch.h"

#if _MSVC_LANG > 201703
//#error _T("目前googleTest1.12不支持C17以上版本，故而必须使用C17及以下标准编译")
#endif //

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

#ifndef __GOOGLEMOCK__
#error _T("本系统必须使用GOOGLE MOCK")
#endif

// 排除外部代码
#include <CodeCoverage\CodeCoverage.h>
#pragma managed(push, off)
ExcludeSourceFromCodeCoverage(Exclude1, L"C:\\Program Files (x86)\\*.*"); // 排除VS2019系统库
ExcludeSourceFromCodeCoverage(Exclude3, _T("")); // 排除VS2019系统库
#if _MSVC_LANG <= 201703

ExcludeSourceFromCodeCoverage(Exclude2, L"C:\\Program Files\\*.*"); // 排除VS2022系统库

ExcludeSourceFromCodeCoverage(Exclude11, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.*"); // 排除GoogleUnitTest
ExcludeSourceFromCodeCoverage(Exclude12, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\Packages\\*.*"); // 排除扩展程序包
ExcludeSourceFromCodeCoverage(Exclude15, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\gsl\\*.*"); // 排除gsl库
ExcludeSourceFromCodeCoverage(Exclude16, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\googletest\\*.*"); // 排除GoogleTest
ExcludeSourceFromCodeCoverage(Exclude13, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\DataBase\\*.*"); // 排除数据库类
ExcludeSourceFromCodeCoverage(Exclude19, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\DataBase\\*.*"); // 排除所有的数据库类
ExcludeSourceFromCodeCoverage(Exclude20, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\ixwebsocket\\*.*"); // 排除ixWebSocket库
ExcludeSourceFromCodeCoverage(Exclude21, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\vld\\*.*"); // 排除vld库
ExcludeSourceFromCodeCoverage(Exclude22, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\OPenSSL\\*.*"); // 排除OpenSSL库
ExcludeSourceFromCodeCoverage(Exclude24, L"d:\\cfiles\\projects\\winssl\\*.*"); // 排除OpenSSL库

// 排除无需测试的单个文件
ExcludeSourceFromCodeCoverage(Exclude103, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysisDoc.cpp");
ExcludeSourceFromCodeCoverage(Exclude105, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.cpp");
ExcludeSourceFromCodeCoverage(Exclude106, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.h");
ExcludeSourceFromCodeCoverage(Exclude107, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.cpp");
ExcludeSourceFromCodeCoverage(Exclude108, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.h");

ExcludeSourceFromCodeCoverage(Exclude109, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\data class\\DataFinnhubWebSocket.cpp");
ExcludeSourceFromCodeCoverage(Exclude110, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\data class\\DataTiingoIEXWebSocket.cpp");
ExcludeSourceFromCodeCoverage(Exclude111, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\data class\\DataTiingoCryptoWebSocket.cpp");
ExcludeSourceFromCodeCoverage(Exclude112, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\data class\\DataTiingoForexWebSocket.cpp");
ExcludeSourceFromCodeCoverage(Exclude113, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\semaphore.h");

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
ExcludeFromCodeCoverage(CMainFrame27, L"CMainFrame::SysCallSetInnerSystemPaneText");
ExcludeFromCodeCoverage(CMainFrame28, L"CMainFrame::SysCallOnSysCommand");

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
ExcludeFromCodeCoverage(CStockAnalysisView37, L"CStockAnalysisView::SysCallBitBlt");
ExcludeFromCodeCoverage(CStockAnalysisView38, L"CStockAnalysisView::ShowStockHistoryDayLine");

ExcludeFromCodeCoverage(CVirtualMarket1, L"CVirtualMarket::AssertValid");
ExcludeFromCodeCoverage(CVirtualMarket2, L"CVirtualMarket::Dump");
ExcludeFromCodeCoverage(CVirtualMarket3, L"CVirtualMarket::ResetMarket");

ExcludeFromCodeCoverage(CChinaMarket1, L"CChinaMarket::AssertValid");
ExcludeFromCodeCoverage(CChinaMarket2, L"CChinaMarket::Dump");
ExcludeFromCodeCoverage(CChinaMarket3, L"CChinaMarket::CreatingThreadAppendChoicedStockDB");
ExcludeFromCodeCoverage(CChinaMarket4, L"CChinaMarket::CreatingThreadCalculateRS");
ExcludeFromCodeCoverage(CChinaMarket5, L"CChinaMarket::CreatingThreadCalculateThisDaysRS");
ExcludeFromCodeCoverage(CChinaMarket6, L"CChinaMarket::CreatingThreadChoice10RSStrong1StockSet");
ExcludeFromCodeCoverage(CChinaMarket7, L"CChinaMarket::CreatingThreadChoice10RSStrong2StockSet");
ExcludeFromCodeCoverage(CChinaMarket8, L"CChinaMarket::CreatingThreadChoice10RSStrongStockSet");
ExcludeFromCodeCoverage(CChinaMarket9, L"CChinaMarket::CreatingThreadLoadDayLine");
ExcludeFromCodeCoverage(CChinaMarket10, L"CChinaMarket::CreatingThreadProcessTodayStock");
ExcludeFromCodeCoverage(CChinaMarket11, L"CChinaMarket::CreatingThreadSaveChoicedRTData");
ExcludeFromCodeCoverage(CChinaMarket12, L"CChinaMarket::CreatingThreadSaveDayLineOfOneStock");
ExcludeFromCodeCoverage(CChinaMarket13, L"CChinaMarket::CreatingThreadSaveTempRTData");
ExcludeFromCodeCoverage(CChinaMarket14, L"CChinaMarket::CreatingThreadUpdateOptionDB");
ExcludeFromCodeCoverage(CChinaMarket15, L"CChinaMarket::CreatingThreadUpdateStockCodeDB");
ExcludeFromCodeCoverage(CChinaMarket16, L"CChinaMarket::CreatingThreadCalculate10RSStrongStock");
ExcludeFromCodeCoverage(CChinaMarket17, L"CChinaMarket::CreatingThreadCalculate10RSStrong1Stock");
ExcludeFromCodeCoverage(CChinaMarket18, L"CChinaMarket::CreatingThreadCalculate10RSStrong2Stock");
ExcludeFromCodeCoverage(CChinaMarket19, L"CChinaMarket::CreatingThreadBuildDayLineRS");
ExcludeFromCodeCoverage(CChinaMarket20, L"CChinaMarket::CreatingThreadBuildDayLineRSOfDate");
ExcludeFromCodeCoverage(CChinaMarket21, L"CChinaMarket::CreatingThreadBuildWeekLine");
ExcludeFromCodeCoverage(CChinaMarket22, L"CChinaMarket::CreatingThreadBuildWeekLineOfStock");
ExcludeFromCodeCoverage(CChinaMarket23, L"CChinaMarket::CreatingThreadBuildWeekLineRS");
ExcludeFromCodeCoverage(CChinaMarket24, L"CChinaMarket::CreatingThreadBuildWeekLineRSOfDate");
ExcludeFromCodeCoverage(CChinaMarket25, L"CChinaMarket::CreatingThreadBuildWeekLineOfCurrentWeek");
ExcludeFromCodeCoverage(CChinaMarket26, L"CChinaMarket::CreatingThreadBuildCurrentWeekWeekLineTable");
ExcludeFromCodeCoverage(CChinaMarket27, L"CChinaMarket::CreatingThreadLoadWeekLine");
ExcludeFromCodeCoverage(CChinaMarket28, L"CChinaMarket::CreatingThreadSaveDayLineBasicInfoOfStock");
ExcludeFromCodeCoverage(CChinaMarket29, L"CChinaMarket::CreatingThreadSaveStockSection");
ExcludeFromCodeCoverage(CChinaMarket30, L"CChinaMarket::CreatingThreadUpdateStockCodeDB");
ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::CreatingThreadUpdateOptionDB");
ExcludeFromCodeCoverage(CChinaMarket41, L"CChinaMarket::DeleteTodayTempDB");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");
//ExcludeFromCodeCoverage(CChinaMarket31, L"CChinaMarket::");

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

ExcludeFromCodeCoverage(CCrweberIndexMarket1, L"CCrweberIndexMarket::CreatingThreadMaintainDB");
ExcludeFromCodeCoverage(CCrweberIndexMarket2, L"CCrweberIndexMarket::CreatingThreadSaveCrweberDB");
ExcludeFromCodeCoverage(CCrweberIndexMarket3, L"CCrweberIndexMarket::ShedulingTaskPer1Hour");
ExcludeFromCodeCoverage(CCrweberIndexMarket4, L"CCrweberIndexMarket::ShedulingTaskPerSecond");
ExcludeFromCodeCoverage(CCrweberIndexMarket5, L"CCrweberIndexMarket::ShedulingTask");

ExcludeFromCodeCoverage(CWorldMarket1, L"CWorldMarket::RunningthreadUpdateDayLneStartEndDate");
ExcludeFromCodeCoverage(CWorldMarket2, L"CWorldMarket::CreatingThreadUpdateDayLineDB");
ExcludeFromCodeCoverage(CWorldMarket3, L"CWorldMarket::CreatingThreadUpdateStockProfileDB");
ExcludeFromCodeCoverage(CWorldMarket4, L"CWorldMarket::CreatingThreadUpdateForexDayLineDB");
ExcludeFromCodeCoverage(CWorldMarket5, L"CWorldMarket::CreatingThreadUpdateForexSymbolDB");
ExcludeFromCodeCoverage(CWorldMarket6, L"CWorldMarket::CreatingThreadUpdateCountryListDB");
ExcludeFromCodeCoverage(CWorldMarket7, L"CWorldMarket::CreatingThreadUpdateEPSSurpriseDB");
ExcludeFromCodeCoverage(CWorldMarket8, L"CWorldMarket::CreatingThreadUpdateTiingoStockDB");
ExcludeFromCodeCoverage(CWorldMarket9, L"CWorldMarket::CreatingThreadUpdateTiingoIndustry");
ExcludeFromCodeCoverage(CWorldMarket10, L"CWorldMarket::CreatingThreadUpdateSICIndustry");
ExcludeFromCodeCoverage(CWorldMarket11, L"CWorldMarket::CreatingThreadUpdateNaicsIndustry");
ExcludeFromCodeCoverage(CWorldMarket12, L"CWorldMarket::CreatingThreadUpdateInsiderTransactionDB");
ExcludeFromCodeCoverage(CWorldMarket13, L"CWorldMarket::CreatingThreadUpdateForexExchangeDB");
ExcludeFromCodeCoverage(CWorldMarket14, L"CWorldMarket::CreatingThreadUpdateEconomicCalendarDB");
ExcludeFromCodeCoverage(CWorldMarket15, L"CWorldMarket::CreatingThreadUpdateCryptoExchangeDB");
ExcludeFromCodeCoverage(CWorldMarket16, L"CWorldMarket::CreatingThreadUpdateCryptoSymbolDB");
ExcludeFromCodeCoverage(CWorldMarket17, L"CWorldMarket::CreatingThreadUpdateCryptoDayLineDB");

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

#endif
ExcludeFromCodeCoverage(CChinaStockHistoryDataContainer1, L"CVirtualDataHistoryCandleExtend::ShowData");

#pragma managed(pop)

#include"EnvironmentSetUp.h"

using namespace StockAnalysisTest;

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	// gtest takes ownership of the TestEnvironment ptr - we don't delete it.
	::testing::AddGlobalTestEnvironment(new TestEnvironment);

	ASSERT(!gl_systemStatus.IsNormalMode());

	return RUN_ALL_TESTS();
}