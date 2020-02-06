#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"PotenDailyBriefingMarket.h"

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
namespace StockAnalysisTest {
  class CPotenDailyBriefingMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestCase() { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestCase() {
      gl_PotenDailyBriefingMarket.SetDatabaseLoaded(false);
      gl_PotenDailyBriefingMarket.SetPermitResetMarket(true);
      gl_PotenDailyBriefingMarket.SetReadyToRun(true);
      gl_PotenDailyBriefingMarket.SetResetMarket(true);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
      EXPECT_EQ(gl_PotenDailyBriefingMarket.GetNewestUpdateDay(), 20180411);
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CPotenDailyBriefingMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetNewestUpdateDay(), 20180411);
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
}