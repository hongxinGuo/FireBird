#include"pch.h"

#include"globedef.h"
#include"Stock.h"
#include"PotenDailyBriefingMarket.h"

namespace StockAnalysisTest {
  TEST(CPotenDailyBriefingTest, TestInitialize) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetNewestUpdateDay(), 20180411);
  }
}