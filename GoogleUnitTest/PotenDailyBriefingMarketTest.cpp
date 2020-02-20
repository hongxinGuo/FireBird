#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"PotenDailyBriefingMarket.h"
#include"WebInquirer.h"

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
namespace StockAnalysisTest {
  class CPotenDailyBriefingMarketTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
      EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
      gl_PotenDailyBriefingMarket.ClearDatabase();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_PotenDailyBriefingMarket.SetDatabaseLoaded(false);
      gl_PotenDailyBriefingMarket.SetPermitResetMarket(true);
      gl_PotenDailyBriefingMarket.SetReadyToRun(true);
      gl_PotenDailyBriefingMarket.SetResetMarket(true);
      gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(20180411);
      gl_PotenDailyBriefingMarket.ClearDatabase();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
    }
  };

  TEST_F(CPotenDailyBriefingMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskResetMarket) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(85959);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(93001);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(90000);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true); // 重置此标识
    gl_PotenDailyBriefingMarket.TaskResetMarket(93000);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());

    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true); // 重置此标识
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsResetMarket) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetResetMarket(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetResetMarket(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSetCurrentInquiringDay) {
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(20190101);
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20190101);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestResetMarket) {
    gl_PotenDailyBriefingMarket.ResetMarket();
    CString str = gl_systemMessage.PopInformationMessage();
    CString strLeft = str.Left(29);
    EXPECT_STREQ(strLeft, _T("重置poten.com于美东标准时间："));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestLoadDatabase) {
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetDatabaseSize(), 0);
    gl_PotenDailyBriefingMarket.LoadDatabase();
    EXPECT_GT(gl_PotenDailyBriefingMarket.GetDatabaseSize(), 0);
    EXPECT_GT(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerMinute) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.SchedulingTaskPerMinute(60, 10000));
  }
}