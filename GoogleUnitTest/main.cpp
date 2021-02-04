////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
/////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#ifndef _MBCS
#error _T("本系统使用多字节字符集")
#endif

// 排除外部代码
#include<CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)
ExcludeSourceFromCodeCoverage(Exclude1, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.*"); // 排除GoogleTest
ExcludeSourceFromCodeCoverage(Exclude2, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\Packages\\*.*"); // 排除扩展程序包
ExcludeSourceFromCodeCoverage(Exclude3, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\DataBase\\*.*"); // 排除数据库类
//ExcludeSourceFromCodeCoverage(Exclude4, L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\*.*"); // 排除VS系统库
ExcludeSourceFromCodeCoverage(Exclude4, L"C:\\Program Files (x86)\\*.*"); // 排除VS系统库
// 排除无需测试的单个文件
ExcludeSourceFromCodeCoverage(Exclude11, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.cpp");
ExcludeSourceFromCodeCoverage(Exclude12, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.h");
ExcludeSourceFromCodeCoverage(Exclude13, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysisDoc.cpp");
ExcludeSourceFromCodeCoverage(Exclude14, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\ChildFrm.h");
ExcludeSourceFromCodeCoverage(Exclude15, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.cpp");
ExcludeSourceFromCodeCoverage(Exclude16, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\StockAnalysis.h");
ExcludeSourceFromCodeCoverage(Exclude17, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.cpp");
ExcludeSourceFromCodeCoverage(Exclude18, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\OutputWnd.h");

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

ExcludeFromCodeCoverage(CCrweberIndex1, L"CCrweberIndex::ReadData");
ExcludeFromCodeCoverage(CCrweberIndex2, L"CCrweberIndex::RunningMaintainDB");

ExcludeFromCodeCoverage(CVirtualWebInquiry1, L"CVirtualWebInquiry::GetNextInquiringMiddleStr");
ExcludeFromCodeCoverage(CVirtualWebInquiry2, L"CVirtualWebInquiry::PrepareNextInquiringStr");
ExcludeFromCodeCoverage(CVirtualWebInquiry3, L"CVirtualWebInquiry::ReadWebFile");
ExcludeFromCodeCoverage(CVirtualWebInquiry4, L"CVirtualWebInquiry::StartReadingThread");

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

#pragma managed(pop)

#include"EnvironmentSetUp.h"

using namespace StockAnalysisTest;

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
  ::testing::AddGlobalTestEnvironment(new TestEnvironment);

  EXPECT_FALSE(gl_fNormalMode);
  EXPECT_TRUE(gl_fTestMode);

  return RUN_ALL_TESTS();
}