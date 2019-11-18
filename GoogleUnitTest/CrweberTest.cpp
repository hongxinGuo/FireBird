#include "pch.h"

#include"crweberIndex.h"

namespace StockAnalysisTest {
  TEST(CrweberTest, TestInitialize) {
    CCrweberIndex Index;
    EXPECT_FALSE(Index.IsTodayUpdated());
    Index.SetNewestDataTime(20191101);
    EXPECT_EQ(Index.GetCurrentDataTime(), 20191101);
    EXPECT_DOUBLE_EQ(Index.GetTD1(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD2(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD3C(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD5(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD6(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD7(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD8(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD9(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD12(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD15(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD19(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD20(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTD21(), 0);
    EXPECT_DOUBLE_EQ(Index.GetVLCC_USGSPORT(), 0);
    EXPECT_DOUBLE_EQ(Index.GetSUEZMAX_CBSUSG(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC1(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC2(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC3(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC4(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC5(), 0);
    EXPECT_DOUBLE_EQ(Index.GetTC14(), 0);
    EXPECT_DOUBLE_EQ(Index.GetCPP_USGCBS(), 0);
    EXPECT_DOUBLE_EQ(Index.GetVLCC_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetSUEZMAX_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetAFRAMAX_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetMR_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetHANDY_1YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetVLCC_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetSUEZMAX_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetAFRAMAX_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetMR_3YEAR(), 0);
    EXPECT_DOUBLE_EQ(Index.GetHANDY_3YEAR(), 0);
  }
}