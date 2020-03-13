#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"MockMainFrm.h"
using namespace Testing;
using namespace ::testing;

namespace StockAnalysisTest {
  CMockMainFrame* s_pMainFrame;
  class CMainFrameTest : public ::testing::Test {
  public:
    static void SetUpTestSuite(void) {
      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
      s_pMainFrame = new CMockMainFrame;
    }
    static void TearDownTestSuite(void) {
      gl_pChinaStockMarket->SetResetMarket(true);
      gl_pCrweberIndexMarket->SetResetMarket(true);
      gl_pPotenDailyBriefingMarket->SetResetMarket(true);
      gl_pChinaStockMarket->SetUpdateStockCodeDB(false); // 这里使用了实际的数据库，故而不允许更新
      gl_pChinaStockMarket->SetUpdateOptionDB(false); // 这里使用了实际的数据库，故而不允许更新
      delete s_pMainFrame;
      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
    }
    virtual void SetUp(void) override {
    }
    virtual void TearDown(void) override {
    }
  public:
  };

  TEST_F(CMainFrameTest, TestIsNeedResetMarket) {
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pPotenDailyBriefingMarket->SetResetMarket(false);
    gl_pCrweberIndexMarket->SetResetMarket(false);
    EXPECT_FALSE(s_pMainFrame->IsNeedResetMarket());
    gl_pChinaStockMarket->SetResetMarket(true);
    EXPECT_TRUE(s_pMainFrame->IsNeedResetMarket());
  }

  TEST_F(CMainFrameTest, TestUpdateStatus) {
    InSequence seq;
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(4, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(5, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(6, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(7, _))
      .Times(1);
    s_pMainFrame->UpdateStatus();

    gl_pChinaStockMarket->SetCurrentEditStockChanged(true);
    gl_pChinaStockMarket->SetCurrentStockChanged(true);
    gl_pChinaStockMarket->SetCurrentStock(_T("sh600000"));
    InSequence seq1;
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(2, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(3, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(1, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(4, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(5, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(6, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(7, _))
      .Times(1);
    s_pMainFrame->UpdateStatus();
  }

  TEST_F(CMainFrameTest, TestOnSysCommand) {
    EXPECT_FALSE(gl_ExitingSystem);
    gl_ExitingSystem = false;
    EXPECT_CALL(*s_pMainFrame, SysCallOnSysCommand)
      .Times(1);
    s_pMainFrame->OnSysCommand(SC_CLOSE, 0);
    EXPECT_TRUE(gl_ExitingSystem);
    gl_ExitingSystem = false;
    EXPECT_CALL(*s_pMainFrame, SysCallOnSysCommand)
      .Times(1);
    s_pMainFrame->OnSysCommand(0xFFFF ^ SC_CLOSE, 0);
    EXPECT_FALSE(gl_ExitingSystem);
    gl_ExitingSystem = false;
  }

  TEST_F(CMainFrameTest, TestOnCalculateTodayRelativeStrong) {
    EXPECT_CALL(*s_pMainFrame, CalculateTodayRelativeStrong)
      .Times(1);
    s_pMainFrame->OnCalculateTodayRelativeStrong();
  }

  TEST_F(CMainFrameTest, TestOnProcessTodayStock) {
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(*s_pMainFrame, ProcessTodayStock)
      .Times(1);
    s_pMainFrame->OnProcessTodayStock();
  }

  TEST_F(CMainFrameTest, TestOnUpdateProcessTodayStock) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetSystemReady(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateProcessTodayStock(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateProcessTodayStock(&cmdUI);
  }

  TEST_F(CMainFrameTest, TestOnUpdateCalculateTodayRelativeStrong) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRelativeStrong(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRelativeStrong(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRelativeStrong(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false));
    s_pMainFrame->OnUpdateCalculateTodayRelativeStrong(&cmdUI);

    gl_ThreadStatus.SetCalculatingDayLineRS(false);
  }

  TEST_F(CMainFrameTest, TestOnChar) {
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('s', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('s', 1, 1);
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('h', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('h', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 2);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('6', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('6', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 3);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 4);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('1', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('1', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('8', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('8', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x008, 1, 1))
      .Times(1);
    s_pMainFrame->OnChar(0x008, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('8', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('8', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('7', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('7', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 7);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('2', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('2', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 8);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x00d, 1, 1))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallInvalidate())
      .Times(1);
    s_pMainFrame->OnChar(0x00d, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetStockCode(), _T("sh601872"));
  }

  TEST_F(CMainFrameTest, TestOnChar2) {
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x008, 1, 1))
      .Times(1);
    s_pMainFrame->OnChar(0x008, 1, 1);
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('s', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('s', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 1);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('z', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('z', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 2);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 3);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('2', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('2', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 4);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('3', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('3', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('4', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('4', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('5', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('5', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 7);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('9', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('9', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 8);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    gl_pChinaStockMarket->SetCurrentStock(_T("sh600604"));
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x00d, 1, 1))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallInvalidate())
      .Times(0);
    s_pMainFrame->OnChar(0x00d, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetStockCode(), _T("sh600604"));
  }

  TEST_F(CMainFrameTest, TestOnKeyUp) {
    gl_pChinaStockMarket->SetCurrentStock(_T("sh600000"));
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(34, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(34, 1, 1);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetStockCode(), _T("sh600001"));
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(33, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(33, 1, 1);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetStockCode(), _T("sh600000"));
    gl_pChinaStockMarket->GetCurrentStock()->SetChoiced(false);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoiceStockSize(), 0);
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(45, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(45, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->GetCurrentStock()->IsChoiced());
    EXPECT_EQ(gl_pChinaStockMarket->GetChoiceStockSize(), 1);
  }

  TEST_F(CMainFrameTest, TestOnBuildResetMarket) {
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pPotenDailyBriefingMarket->SetResetMarket(false);
    gl_pCrweberIndexMarket->SetResetMarket(false);
    s_pMainFrame->OnBuildResetMarket();
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsResetMarket());
  }

  TEST_F(CMainFrameTest, TestOnUpdateRebuildDayLineRS) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->__TEST_SetMarketTime((long)83001);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDaylineRS(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetMarketTime((long)92959);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDaylineRS(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetMarketTime((long)83000);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDaylineRS(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetMarketTime((long)83000);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDaylineRS(&cmdUI);
  }

  TEST_F(CMainFrameTest, TestOnUpdateRecordRTData) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetRecordRTData(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateRecordRTData(&cmdUI);
    gl_pChinaStockMarket->SetRecordRTData(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRecordRTData(&cmdUI);
  }

  TEST_F(CMainFrameTest, TestOnAbortBuildingRS) {
    gl_fExitingCalculatingRS = false;
    s_pMainFrame->OnAbortBuindingRS();
    EXPECT_TRUE(gl_fExitingCalculatingRS);

    gl_fExitingCalculatingRS = false;
  }

  TEST_F(CMainFrameTest, TestOnUpdateAbortBuindingRS) {
    CCmdUI cmdUI;
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateAbortBuindingRS(&cmdUI);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateAbortBuindingRS(&cmdUI);

    gl_ThreadStatus.SetCalculatingDayLineRS(false);
  }

  TEST_F(CMainFrameTest, TestOnRecordRTData) {
    gl_pChinaStockMarket->SetRecordRTData(false);
    s_pMainFrame->OnRecordRTData();
    EXPECT_TRUE(gl_pChinaStockMarket->IsRecordingRTData());
    s_pMainFrame->OnRecordRTData();
    EXPECT_FALSE(gl_pChinaStockMarket->IsRecordingRTData());
  }
}