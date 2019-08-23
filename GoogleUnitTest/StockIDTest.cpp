#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"StockID.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(StockIDTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CStockID id;
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetIndex(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    EXPECT_FALSE(id.IsActive());
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
    EXPECT_FALSE(id.IsInquiringOnce());
    id.SetMarket(1);
    id.SetStockCode(_T("abcde"));
    id.SetStockName(_T("dcba"));
    id.SetIndex(1);
    id.SetDayLineEndDay(20020202);
    id.SetIPOStatus(0);
    id.SetActive(true);
    id.SetDayLineNeedUpdate(false);
    id.SetInquiringOnce(true);
    EXPECT_EQ(id.GetMarket(), 1);
    EXPECT_STREQ(id.GetStockCode(), _T("abcde"));
    EXPECT_STREQ(id.GetStockName(), _T("dcba"));
    EXPECT_EQ(id.GetIndex(), 1);
    EXPECT_EQ(id.GetDayLineEndDay(), 20020202);
    EXPECT_EQ(id.GetIPOStatus(), 0);
    EXPECT_TRUE(id.IsActive());
    EXPECT_FALSE(id.IsDayLineNeedUpdate());
    EXPECT_TRUE(id.IsInquiringOnce());
    id.Reset();
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetIndex(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    EXPECT_FALSE(id.IsActive());
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
    EXPECT_FALSE(id.IsInquiringOnce());

  }
  TEST(StockIDTest, TestGetMarket) {
    CStockID id;
    EXPECT_EQ(id.GetMarket(), 0);
    id.SetMarket(__SHANGHAI_MARKET__);
    EXPECT_EQ(id.GetMarket(), __SHANGHAI_MARKET__);
    id.SetMarket(__SHENZHEN_MARKET__);
    EXPECT_EQ(id.GetMarket(), __SHENZHEN_MARKET__);
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

  TEST(StockIDTest, TestGetDayLineEndDay) {
    CStockID id;
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    id.SetDayLineEndDay(19980101);
    EXPECT_EQ(id.GetDayLineEndDay(), 19980101);
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