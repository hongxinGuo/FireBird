#include"pch.h"
#include"globedef.h"

#include"VirtualHistoryData.h"

namespace StockAnalysisTest {
  class CVirtualHistoryDataTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CVirtualHistoryDataTest, TestGetDate) {
    ASSERT_FALSE(gl_fNormalMode);
    CVirtualHistoryData historyData;
    historyData.SetDate(__CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(historyData.GetFormatedMarketDate(), __CHINA_MARKET_BEGIN_DATE__);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetTime) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetFormatedMarketDate(), 0);
    historyData.SetTime(100100100);
    EXPECT_EQ(historyData.GetFormatedMarketTime(), 100100100);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetMarketString) {
    CVirtualHistoryData historyData;
    EXPECT_STREQ(historyData.GetMarketString(), _T(""));
    historyData.SetMarketString(_T("浦东银行1"));
    EXPECT_STREQ(historyData.GetMarketString(), _T("浦东银行1"));
  }

  TEST_F(CVirtualHistoryDataTest, TestGetStockSymbol) {
    CVirtualHistoryData historyData;
    EXPECT_STREQ(historyData.GetStockSymbol(), _T(""));
    historyData.SetStockSymbol(_T("600000.SS"));
    EXPECT_STREQ(historyData.GetStockSymbol(), _T("600000.SS"));
  }

  TEST_F(CVirtualHistoryDataTest, TestGetStockName) {
    CVirtualHistoryData historyData;
    EXPECT_STREQ(historyData.GetStockName(), _T(""));
    historyData.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(historyData.GetStockName(), _T("浦东银行"));
  }

  TEST_F(CVirtualHistoryDataTest, TestGetLastClose) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetLastClose(), 0);
    historyData.SetLastClose(10100);
    EXPECT_EQ(historyData.GetLastClose(), 10100);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetOpen) {
    ASSERT_FALSE(gl_fNormalMode);
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetOpen(), 0);
    historyData.SetOpen(11100);
    EXPECT_EQ(historyData.GetOpen(), 11100);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetHigh) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetHigh(), 0);
    historyData.SetHigh(11111);
    EXPECT_EQ(historyData.GetHigh(), 11111);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetLow) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetLow(), 0);
    historyData.SetLow(22222);
    EXPECT_EQ(historyData.GetLow(), 22222);
    historyData.SetLow(_T("12345"));
    EXPECT_EQ(historyData.GetLow(), 12345);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetClose) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetClose(), 0);
    historyData.SetClose(33333);
    EXPECT_EQ(historyData.GetClose(), 33333);
    historyData.SetClose(_T("12345"));
    EXPECT_EQ(historyData.GetClose(), 12345);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetUpDown) {
    CVirtualHistoryData historyData;
    EXPECT_DOUBLE_EQ(historyData.GetUpDown(), 0);
    historyData.SetUpDown(10.020);
    EXPECT_DOUBLE_EQ(historyData.GetUpDown(), 10.020);
    historyData.SetUpDown(_T("12.345"));
    EXPECT_DOUBLE_EQ(historyData.GetUpDown(), 12.345);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetUpDownRate) {
    CVirtualHistoryData historyData;
    EXPECT_DOUBLE_EQ(historyData.GetUpDownRate(), 0);
    historyData.SetUpDownRate(40.40);
    EXPECT_DOUBLE_EQ(historyData.GetUpDownRate(), 40.40);
    historyData.SetUpDownRate(_T("12.345"));
    EXPECT_DOUBLE_EQ(historyData.GetUpDownRate(), 12.345);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetChangeHandRate) {
    ASSERT_FALSE(gl_fNormalMode);
    CVirtualHistoryData historyData;
    EXPECT_DOUBLE_EQ(historyData.GetChangeHandRate(), 0);
    historyData.SetChangeHandRate(30.30);
    EXPECT_DOUBLE_EQ(historyData.GetChangeHandRate(), 30.30);
    historyData.SetChangeHandRate(123456.7890);
    EXPECT_DOUBLE_EQ(historyData.GetChangeHandRate(), 123456.7890);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetVolume) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetVolume(), 0);
    historyData.SetVolume(100100100100100100);
    EXPECT_EQ(historyData.GetVolume(), 100100100100100100);
    historyData.SetVolume(12345678910);
    EXPECT_EQ(historyData.GetVolume(), 12345678910);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetAmount) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetAmount(), 0);
    historyData.SetAmount(200200200200200200);
    EXPECT_EQ(historyData.GetAmount(), 200200200200200200);
    historyData.SetAmount(123456789);
    EXPECT_EQ(historyData.GetAmount(), 123456789);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetTotalValue) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetTotalValue(), 0);
    CString str = _T("3.13e+11");
    historyData.SetTotalValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(historyData.GetTotalValue()), 3.13e+11);
    historyData.SetTotalValue(12345678901);
    EXPECT_EQ(historyData.GetTotalValue(), 12345678901);
  }

  TEST_F(CVirtualHistoryDataTest, TestGetCurrentValue) {
    CVirtualHistoryData historyData;
    EXPECT_EQ(historyData.GetCurrentValue(), 0);
    CString str = _T("3.15e+11");
    historyData.SetCurrentValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(historyData.GetCurrentValue()), 3.15e+11);
    historyData.SetCurrentValue(1234567890);
    EXPECT_EQ(historyData.GetCurrentValue(), 1234567890);
  }
}