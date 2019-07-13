#include"stdafx.h"
#include"pch.h"

#include"StockID.h"

namespace StockAnalysisTest {
  TEST(StockIDTest, TestGetMarket) {
    CStockID id;
    EXPECT_EQ(id.GetMarket(), 0);
    id.SetMarket(1);
    EXPECT_EQ(id.GetMarket(), 1);
  }

  TEST(StockIDTest, TestGetStockCode) {
    CStockID id;
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    id.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(id.GetStockCode(), _T("sh600000"));
  }

  TEST(StockIDTest, TestGetStockName) {
    CStockID id;
    EXPECT_STREQ(id.GetStockName(), _T(""));
    id.SetStockName(_T("sh600000"));
    EXPECT_STREQ(id.GetStockName(), _T("sh600000"));
  }
  TEST(StockIDTest, TestGetIndex) {
    CStockID id;
    EXPECT_EQ(id.GetIndex(), -1);
    id.SetIndex(10101);
    EXPECT_EQ(id.GetIndex(), 10101);
  }
  TEST(StockIDTest, TestGetDayLineStartDay) {
    CStockID id;
    EXPECT_EQ(id.GetDayLineStartDay(), 19900101);
    id.SetDayLineStartDay(20120202);
    EXPECT_EQ(id.GetDayLineStartDay(), 20120202);
  }
  TEST(StockIDTest, TestGetDayLineEndDay) {
    CStockID id;
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    id.SetDayLineEndDay(19980101);
    EXPECT_EQ(id.GetDayLineEndDay(), 19980101);
  }

  TEST(StockIDTest, TestGetNewestDayLineDay) {
    CStockID id;
    EXPECT_EQ(id.GetNewestDayLineDay(), 19900101);
    id.SetNewestDayLineDay(20010102);
    EXPECT_EQ(id.GetNewestDayLineDay(), 20010102);
  }

  TEST(StockIDTest, TestGetIPOStatus) {
    CStockID id;
    EXPECT_EQ(id.GetIPOStatus(), 128);
    id.SetIPOStatus(255);
    EXPECT_EQ(id.GetIPOStatus(), 255);
  }

  TEST(StockIDTest, TestIsActive) {
    CStockID id;
    EXPECT_FALSE(id.IsActive());
    id.SetActive(true);
    EXPECT_TRUE(id.IsActive());
    id.SetActive(false);
    EXPECT_FALSE(id.IsActive());
  }

  TEST(StockIDTest, TestIsDayLineNeedUpdate) {
    CStockID id;
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(false);
    EXPECT_FALSE (id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(true);
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
  }

  TEST(StockIDTest, TestIsInquiringOnce) {
    CStockID id;
    EXPECT_FALSE(id.IsInquiringOnce());
    id.SetInquiringOnce(true);
    EXPECT_TRUE(id.IsInquiringOnce());
    id.SetInquiringOnce(false);
    EXPECT_FALSE(id.IsInquiringOnce());
  }

}