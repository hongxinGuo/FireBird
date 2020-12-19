#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"MockMainFrm.h"
using namespace testing;
using namespace ::testing;

namespace StockAnalysisTest {
  static CMockMainFrame* s_pMainFrame;
  class CMockMainFrameTest : public ::testing::Test {
  public:
    static void SetUpTestSuite(void) {
      EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL);//
      s_pMainFrame = new CMockMainFrame;
      EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL);//

      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_EQ(gl_vMarketPtr.size(), 3);
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
      gl_pChinaStakeMarket->ResetCurrentStock();
      gl_pChinaStakeMarket->SetResetMarket(true);
      gl_pCrweberIndexMarket->SetResetMarket(true);
      gl_pPotenDailyBriefingMarket->SetResetMarket(true);
      gl_pChinaStakeMarket->SetCurrentStockChanged(false);
      gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
      gl_pChinaStakeMarket->SetUpdateStakeCodeDB(false); // 这里使用了实际的数据库，故而不允许更新
      gl_pChinaStakeMarket->SetUpdateOptionDB(false); // 这里使用了实际的数据库，故而不允许更新
      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_EQ(gl_vMarketPtr.size(), 3);
      if (CMFCVisualManager::GetInstance() != NULL) {
        delete CMFCVisualManager::GetInstance(); // 在生成MainFrame时，会生成一个视觉管理器。故而在此删除之。
      }
      delete s_pMainFrame;
    }
    virtual void SetUp(void) override {
      gl_fExitingSystem = false;
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    }
    virtual void TearDown(void) override {
      gl_pChinaStakeMarket->ResetCurrentStock();
      gl_pChinaStakeMarket->SetCurrentStockChanged(false);
      gl_pChinaStakeMarket->ClearChoiceStockContainer();
      gl_ThreadStatus.SetCalculatingDayLineRS(false);
      gl_fExitingSystem = false;
    }
  };

  TEST_F(CMockMainFrameTest, TestCreateMarketContainer) {
    EXPECT_TRUE(s_pMainFrame->CreateMarketContainer());
    EXPECT_EQ(gl_vMarketPtr.size(), 8);
    CVirtualMarketPtr pMarket;
    pMarket = gl_vMarketPtr.at(7);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pCrweberIndexMarket->GetMarketID());
    pMarket = gl_vMarketPtr.at(6);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pPotenDailyBriefingMarket->GetMarketID());
    pMarket = gl_vMarketPtr.at(5);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pChinaStakeMarket->GetMarketID());
    pMarket = gl_vMarketPtr.at(4);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pAmericaMarket->GetMarketID());
    EXPECT_EQ(gl_vMarketPtr.size(), 4);
  }

  TEST_F(CMockMainFrameTest, TestIsNeedResetMarket) {
    gl_pAmericaMarket->SetResetMarket(false);
    gl_pChinaStakeMarket->SetResetMarket(false);
    gl_pPotenDailyBriefingMarket->SetResetMarket(false);
    gl_pCrweberIndexMarket->SetResetMarket(false);
    EXPECT_FALSE(s_pMainFrame->IsNeedResetMarket());
    gl_pChinaStakeMarket->SetResetMarket(true);
    EXPECT_TRUE(s_pMainFrame->IsNeedResetMarket());
  }

  TEST_F(CMockMainFrameTest, TestUpdateStatus) {
    InSequence seq;
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    gl_pChinaStakeMarket->SetCurrentStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(4, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(5, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(6, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(7, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(8, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(10, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(11, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(12, _))
      .Times(1);
    s_pMainFrame->UpdateStatus();

    gl_pChinaStakeMarket->SetCurrentEditStockChanged(true);
    gl_pChinaStakeMarket->SetCurrentStockChanged(true);
    gl_pChinaStakeMarket->SetCurrentStock(_T("sh600000"));
    InSequence seq1;
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(2, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(3, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(4, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(5, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(1, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(6, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(7, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(8, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(10, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(11, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(12, _))
      .Times(1);
    s_pMainFrame->UpdateStatus();
  }

  TEST_F(CMockMainFrameTest, TestOnSysCommand) {
    gl_fExitingSystem = false;
    EXPECT_CALL(*s_pMainFrame, SysCallOnSysCommand)
      .Times(1);
    s_pMainFrame->OnSysCommand(SC_CLOSE, 0);
    EXPECT_TRUE(gl_fExitingSystem);
    gl_fExitingSystem = false;
    EXPECT_CALL(*s_pMainFrame, SysCallOnSysCommand)
      .Times(1);
    s_pMainFrame->OnSysCommand(0xFFFF ^ SC_CLOSE, 0);
    EXPECT_FALSE(gl_fExitingSystem);
    gl_fExitingSystem = false;
  }

  TEST_F(CMockMainFrameTest, TestOnCalculateTodayRS) {
    EXPECT_CALL(*s_pMainFrame, CalculateTodayRS)
      .Times(1);
    s_pMainFrame->OnCalculateTodayRS();
  }

  TEST_F(CMockMainFrameTest, TestOnProcessTodayStock) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    EXPECT_CALL(*s_pMainFrame, ProcessTodayStock)
      .Times(1);
    s_pMainFrame->OnProcessTodayStock();
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateProcessTodayStock) {
    CCmdUI cmdUI;
    gl_pChinaStakeMarket->SetSystemReady(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateProcessTodayStock(&cmdUI);
    gl_pChinaStakeMarket->SetSystemReady(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateProcessTodayStock(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateCalculateTodayRS) {
    CCmdUI cmdUI;
    gl_pChinaStakeMarket->SetSystemReady(false);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
    gl_pChinaStakeMarket->SetSystemReady(false);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
    gl_pChinaStakeMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false));
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestPreTranslateMessage) {
    MSG msg;
    msg.message = WM_KEYDOWN;
    EXPECT_CALL(*s_pMainFrame, SysCallPreTranslateMessage(&msg))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSendMessage(_, _, _))
      .Times(0);
    s_pMainFrame->PreTranslateMessage(&msg);

    msg.message = WM_KEYUP;
    msg.wParam = 201;
    msg.lParam = 102;
    EXPECT_CALL(*s_pMainFrame, SysCallPreTranslateMessage(&msg))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSendMessage(WM_KEYUP, 201, 102))
      .Times(1);
    s_pMainFrame->PreTranslateMessage(&msg);

    msg.message = WM_CHAR;
    msg.wParam = 200;
    msg.lParam = 100;
    EXPECT_CALL(*s_pMainFrame, SysCallPreTranslateMessage(&msg))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSendMessage(WM_CHAR, 200, 100))
      .Times(1);
    s_pMainFrame->PreTranslateMessage(&msg);
  }

  TEST_F(CMockMainFrameTest, TestOnChar) {
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('s', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('s', 1, 1);
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('h', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('h', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 2);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('6', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('6', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 3);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 4);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('1', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('1', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('8', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('8', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x008, 1, 1))
      .Times(1);
    s_pMainFrame->OnChar(0x008, 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('8', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('8', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('7', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('7', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 7);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('2', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('2', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 8);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x00d, 1, 1))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallInvalidate())
      .Times(1);
    s_pMainFrame->OnChar(0x00d, 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_STREQ(gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode(), _T("sh601872"));
  }

  TEST_F(CMockMainFrameTest, TestOnChar2) {
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x008, 1, 1))
      .Times(1);
    s_pMainFrame->OnChar(0x008, 1, 1);
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('s', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('s', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 1);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('z', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('z', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 2);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 3);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('2', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('2', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 4);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('3', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('3', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('4', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('4', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('5', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('5', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 7);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('9', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('9', 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 8);
    gl_pChinaStakeMarket->SetCurrentEditStockChanged(false);
    gl_pChinaStakeMarket->SetCurrentStock(_T("sh600604"));
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x00d, 1, 1))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallInvalidate())
      .Times(0);
    s_pMainFrame->OnChar(0x00d, 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_STREQ(gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode(), _T("sh600604"));
  }

  TEST_F(CMockMainFrameTest, TestOnKeyUp) {
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    gl_pChinaStakeMarket->SetCurrentStock(_T("sh600000"));
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(34, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(34, 1, 1);
    EXPECT_STREQ(gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode(), _T("sh600001"));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentStockChanged());

    gl_pChinaStakeMarket->SetCurrentStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(33, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(33, 1, 1);
    EXPECT_STREQ(gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode(), _T("sh600000"));
    EXPECT_TRUE(gl_pChinaStakeMarket->IsCurrentStockChanged());

    gl_pChinaStakeMarket->SetCurrentStockChanged(false);
    gl_pChinaStakeMarket->GetCurrentStock()->SetChoiced(false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 0);
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(45, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(45, 1, 1);
    EXPECT_TRUE(gl_pChinaStakeMarket->GetCurrentStock()->IsChoiced());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUpdateChoicedStockDB());
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 1);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());

    gl_pChinaStakeMarket->SetCurrentStockChanged(false);
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 1);
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(46, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(46, 1, 1);
    EXPECT_FALSE(gl_pChinaStakeMarket->GetCurrentStock()->IsChoiced());
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUpdateChoicedStockDB());
    EXPECT_EQ(gl_pChinaStakeMarket->GetChoicedStockSize(), 0);
    EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());

    // 恢复原状
    gl_pChinaStakeMarket->ClearChoiceStockContainer();
    gl_pChinaStakeMarket->SetUpdateChoicedStockDB(false);
  }

  TEST_F(CMockMainFrameTest, TestOnBuildResetMarket) {
    gl_pChinaStakeMarket->SetResetMarket(false);
    gl_pPotenDailyBriefingMarket->SetResetMarket(false);
    gl_pCrweberIndexMarket->SetResetMarket(false);
    s_pMainFrame->OnBuildResetMarket();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsResetMarket());
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateRebuildDayLineRS) {
    CCmdUI cmdUI;
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime((long)83001);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime((long)92959);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime((long)83000);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime((long)83000);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateRecordRTData) {
    CCmdUI cmdUI;
    gl_pChinaStakeMarket->SetRecordRTData(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateRecordRTData(&cmdUI);
    gl_pChinaStakeMarket->SetRecordRTData(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRecordRTData(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnAbortBuildingRS) {
    gl_fExitingCalculatingRS = false;
    s_pMainFrame->OnAbortBuindingRS();
    EXPECT_TRUE(gl_fExitingCalculatingRS);

    gl_fExitingCalculatingRS = false;
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateAbortBuindingRS) {
    CCmdUI cmdUI;
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateAbortBuindingRS(&cmdUI);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateAbortBuindingRS(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnRecordRTData) {
    gl_pChinaStakeMarket->SetRecordRTData(false);
    s_pMainFrame->OnRecordRTData();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsRecordingRTData());
    s_pMainFrame->OnRecordRTData();
    EXPECT_FALSE(gl_pChinaStakeMarket->IsRecordingRTData());
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS1) {
    CCmdUI cmdUI;
    long dayLineSave = gl_pChinaStakeMarket->GetDayLineNeedSaveNumber();
    long dayLineUpdate = gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber();

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(gl_pChinaStakeMarket->GetTotalStock());
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(dayLineUpdate);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(dayLineSave);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS2) {
    CCmdUI cmdUI;
    long dayLineSave = gl_pChinaStakeMarket->GetDayLineNeedSaveNumber();
    long dayLineUpdate = gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber();

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(gl_pChinaStakeMarket->GetTotalStock());
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(dayLineUpdate);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(dayLineSave);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS) {
    CCmdUI cmdUI;
    long dayLineSave = gl_pChinaStakeMarket->GetDayLineNeedSaveNumber();
    long dayLineUpdate = gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber();

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(gl_pChinaStakeMarket->GetTotalStock());
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(0);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(0);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

    gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(dayLineUpdate);
    gl_pChinaStakeMarket->SetDayLineNeedSaveNumber(dayLineSave);
  }

  TEST_F(CMockMainFrameTest, TestOnUsingRealtimeDataServer) {
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    s_pMainFrame->OnUsingNeteaseRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataServer());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    s_pMainFrame->OnUsingNeteaseRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataServer());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    s_pMainFrame->OnUsingSinaRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataServer());
    s_pMainFrame->OnUsingSinaRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStakeMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStakeMarket->IsUsingNeteaseRTDataServer());
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateUsingNeteaseRealtimeDataServer) {
    CCmdUI cmdUI;
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
    gl_pChinaStakeMarket->SetUsingSinaRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateUsingSinaRealtimeDataServer) {
    CCmdUI cmdUI;
    gl_pChinaStakeMarket->SetUsingSinaRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);
    gl_pChinaStakeMarket->SetUsingNeteaseRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);

    //恢复初态
    gl_pChinaStakeMarket->SetUsingSinaRTDataServer();
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateBuildCurrentWeekLine) {
    CCmdUI cmdUI;
    long lTime = gl_pChinaStakeMarket->GetFormatedMarketTime();
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime(150959);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime(151000);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime(151001);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
    gl_pChinaStakeMarket->__TEST_SetFormatedMarketTime(lTime);
  }
}