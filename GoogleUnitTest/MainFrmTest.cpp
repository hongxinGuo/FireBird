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
}