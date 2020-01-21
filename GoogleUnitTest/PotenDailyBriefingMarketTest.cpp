#include"pch.h"

#include"globedef.h"
#include"Stock.h"
#include"PotenDailyBriefingMarket.h"

namespace StockAnalysisTest {
  TEST(CPotenDailyBriefingTest, TestInitialize) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetNewestUpdateDay(), 20180411);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetSystem());
  }

  TEST(CPotenDailyBriefingTest, TestTaskResetSystem) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetSystem());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetSystem());
    gl_PotenDailyBriefingMarket.TaskResetSystem(92459);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetSystem());
    gl_PotenDailyBriefingMarket.TaskResetSystem(93001);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetSystem());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetSystem());
    gl_PotenDailyBriefingMarket.TaskResetSystem(92500);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetSystem());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetSystem());

    gl_PotenDailyBriefingMarket.SetPermitResetSystem(true); // 重置此标识
  }

  TEST(CPotenDailybriefingTest, TestIsReadyToRun) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
    gl_PotenDailyBriefingMarket.SetReadyToRun(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsReadyToRun());
    gl_PotenDailyBriefingMarket.SetReadyToRun(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
  }

  TEST(CPotenDailybriefingTest, TestIsPermitResetSystem) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetSystem());
    gl_PotenDailyBriefingMarket.SetPermitResetSystem(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetSystem());
    gl_PotenDailyBriefingMarket.SetPermitResetSystem(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetSystem());
  }

  TEST(CPotenDailybriefingTest, TestIsResetSystem) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetSystem());
    gl_PotenDailyBriefingMarket.SetResetSystem(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsResetSystem());
    gl_PotenDailyBriefingMarket.SetResetSystem(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetSystem());
  }
}