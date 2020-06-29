////////////////////////////////////////////////////////////////////////////////////
//
//
// 由于需要预先设定全局初始化变量，故而不使用系统提供的main()函数，而是自己定义。
// 主要增加一项AddGlobalTestEnvironment()函数调用，用于设定全局环境。此函数可以多次使用。
//
/////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

// 排除外部代码
#include<CodeCoverage/CodeCoverage.h>
#pragma managed(push, off)
ExcludeSourceFromCodeCoverage(Exclude1, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\GoogleUnitTest\\*.*"); // 排除GoogleTest
ExcludeSourceFromCodeCoverage(Exclude2, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\Packages\\*.*"); // 排除扩展程序包
ExcludeSourceFromCodeCoverage(Exclude3, L"C:\\users\\hxguo\\source\\repos\\StockAnalysis\\StockAnalysis\\source\\DataBase\\*.*"); // 排除数据库类
ExcludeSourceFromCodeCoverage(Exclude4, L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\*.*"); // 排除VS系统库
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

ExcludeFromCodeCoverage(CVirtualMarket1, L"CVirtualMarket::AssertValid");
ExcludeFromCodeCoverage(CVirtualMarket2, L"CVirtualMarket::Dump");
ExcludeFromCodeCoverage(CVirtualMarket3, L"CVirtualMarket::ResetMarket");

ExcludeFromCodeCoverage(CChinaMarket1, L"CChinaMarket::AssertValid");
ExcludeFromCodeCoverage(CChinaMarket2, L"CChinaMarket::Dump");
ExcludeFromCodeCoverage(CChinaMarket3, L"CChinaMarket::RunningThreadAppendChoicedStockDB");
ExcludeFromCodeCoverage(CChinaMarket4, L"CChinaMarket::RunningThreadCalculateRelativeStrong");
ExcludeFromCodeCoverage(CChinaMarket5, L"CChinaMarket::RunningThreadCalculateThisDayRS");
ExcludeFromCodeCoverage(CChinaMarket6, L"CChinaMarket::RunningThreadChoice10RSStrong1StockSet");
ExcludeFromCodeCoverage(CChinaMarket7, L"CChinaMarket::RunningThreadChoice10RSStrong2StockSet");
ExcludeFromCodeCoverage(CChinaMarket8, L"CChinaMarket::RunningThreadChoice10RSStrongStockSet");
ExcludeFromCodeCoverage(CChinaMarket9, L"CChinaMarket::RunningThreadLoadDayLine");
ExcludeFromCodeCoverage(CChinaMarket10, L"CChinaMarket::RunningThreadProcessTodayStock");
ExcludeFromCodeCoverage(CChinaMarket11, L"CChinaMarket::RunningThreadSaveChoicedRTData");
ExcludeFromCodeCoverage(CChinaMarket12, L"CChinaMarket::RunningThreadSaveDayLineOfOneStock");
ExcludeFromCodeCoverage(CChinaMarket13, L"CChinaMarket::RunningThreadSaveTempRTData");
ExcludeFromCodeCoverage(CChinaMarket14, L"CChinaMarket::RunningThreadUpdateOptionDB");

ExcludeFromCodeCoverage(CChinaStock1, L"CChinaStock::AssertValid");
ExcludeFromCodeCoverage(CChinaStock2, L"CChinaStock::Dump");

ExcludeFromCodeCoverage(CVirtualWebInquiry1, L"CVirtualWebInquiry::GetNextInquiringMiddleStr");
ExcludeFromCodeCoverage(CVirtualWebInquiry2, L"CVirtualWebInquiry::PrepareNextInquiringStr");
ExcludeFromCodeCoverage(CVirtualWebInquiry3, L"CVirtualWebInquiry::ReadWebFile");
ExcludeFromCodeCoverage(CVirtualWebInquiry4, L"CVirtualWebInquiry::StartReadingThread");

ExcludeFromCodeCoverage(CRTData1, L"CRTData::AssertValid");
ExcludeFromCodeCoverage(CRTData2, L"CRTData::Dump");

ExcludeFromCodeCoverage(CNeteaseRTWebInquiry1, L"CNeteaseRTWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CTengxunRTWebInquiry1, L"CTengxunRTWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CSinaRTWebInquiry1, L"CSinaRTWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CCrweberIndexWebInquiry1, L"CCrweberIndexWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CNeteaseDayLineWebInquiry1, L"CNeteaseDayLineWebInquiry::StartReadingThread");

ExcludeFromCodeCoverage(CPotenDailyBriefingWebInquiry1, L"CPotenDailyBriefingWebInquiry::StartReadingThread");
ExcludeFromCodeCoverage(CPotenDailyBriefingWebInquiry2, L"CPotenDailyBriefingWebInquiry::RunningThreadSavePotenData");

ExcludeFromCodeCoverage(CSystemDeque1, L"CSystemDeque::SysCallOutputListAddString");

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