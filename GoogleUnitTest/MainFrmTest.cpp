#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"MockMainFrm.h"
using namespace Testing;

namespace StockAnalysisTest {
  CMockMainFrame* s_pMainFrame;
  class CMainFrameTest : public ::testing::Test {
  public:
    static void SetUpTestSuite(void) {
      gl_fNormalMode = true;
      gl_fTestMode = false;
      s_pMainFrame = new CMockMainFrame;
    }
    static void TearDownTestSuite(void) {
      gl_pChinaStockMarket->SetResetMarket(true);
      gl_pCrweberIndexMarket->SetResetMarket(true);
      gl_pPotenDailyBriefingMarket->SetResetMarket(true);
      gl_pChinaStockMarket->SetUpdateStockCodeDB(false); // 这里使用了实际的数据库，故而不允许更新
      gl_pChinaStockMarket->SetUpdateOptionDB(false); // 这里使用了实际的数据库，故而不允许更新
      delete s_pMainFrame;
      gl_fNormalMode = false;
      gl_fTestMode = true;
    }
    virtual void SetUp(void) override {
    }
    virtual void TearDown(void) override {
      gl_fNormalMode = false;
      gl_fTestMode = true;
    }
  public:
  };

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
}