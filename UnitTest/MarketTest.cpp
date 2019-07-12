#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"Stock.h"

using namespace std;
#include<memory>


namespace StockAnalysisTest {

  class MarketTest : public::testing::Test
  {
  protected:
    void SetUp(void) override {
    }

    void TearDown(void) override {
      // clearup
    }    
  };
  
  TEST_F(MarketTest, TestIsAStock) {
    CStockPtr pstock = make_shared<CStock>();
    pstock->SetStockCode(_T("sh600000"));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(pstock));
    pstock->SetStockCode(_T("ss600000"));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(pstock));
    pstock->SetStockCode(_T("sz000001"));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(pstock));
    pstock->SetStockCode(_T("sz10001"));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(pstock));
  }

  TEST_F(MarketTest, TestIsAStock2) {
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(_T("sh600000")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz600000")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("ss000001")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("az000001")));
    EXPECT_FALSE(gl_ChinaStockMarket.IsAStock(_T("sz200001")));
    EXPECT_TRUE(gl_ChinaStockMarket.IsAStock(_T("sz000001")));
  }

  TEST_F(MarketTest, TestGetShowStock) {
    CStockPtr pStock = make_shared<CStock>();
    EXPECT_EQ(gl_ChinaStockMarket.GetShowStock(), nullptr);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    gl_ChinaStockMarket.SetShowStock(pStock);
    EXPECT_TRUE(gl_ChinaStockMarket.GetShowStock(), pStock);
    EXPECT_TRUE(gl_ChinaStockMarket.IsCurrentStockChanged());
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());
    gl_ChinaStockMarket.SetShowStock(pStock);
    EXPECT_FALSE(gl_ChinaStockMarket.IsCurrentStockChanged());

  }

  TEST_F(MarketTest, TestMarketReady) {
    EXPECT_FALSE(gl_ChinaStockMarket.MarketReady());
    gl_ChinaStockMarket.SetMarketReadyFlag(true);
    EXPECT_TRUE(gl_ChinaStockMarket.MarketReady());
    gl_ChinaStockMarket.SetMarketReadyFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.MarketReady());
  }
  
  TEST_F(MarketTest, TestIsTodayStockCompiled) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockCompiled());
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsTodayStockCompiled());
    gl_ChinaStockMarket.SetTodayStockCompiledFlag(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsTodayStockCompiled());
  }

  TEST_F(MarketTest, TestIsLoadSelectedStock) {
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(true);
    EXPECT_TRUE(gl_ChinaStockMarket.IsLoadSelectedStock());
    gl_ChinaStockMarket.SetLoadSelectedStock(false);
    EXPECT_FALSE(gl_ChinaStockMarket.IsLoadSelectedStock());
  }

  TEST_F(MarketTest, TestGetCurrentDownLoadingStockCodeStr) {
    EXPECT_STREQ(gl_ChinaStockMarket.GetCurrentDownLoadingStockCodeStr(), _T(""));
    gl_ChinaStockMarket.SetCurrentDownLoadingStockCodeStr(_T("abcd"));
    EXPECT_STREQ(gl_ChinaStockMarket.GetCurrentDownLoadingStockCodeStr(), _T("abcd"));
  }
  TEST_F(MarketTest, TestGetRelativeStartDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900101);
    gl_ChinaStockMarket.SetRelativeStrongStartDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongStartDay(), 19900103);
  }

  TEST_F(MarketTest, TestGetRelativeEndDay) {
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900101);
    gl_ChinaStockMarket.SetRelativeStrongEndDay(19900103);
    EXPECT_EQ(gl_ChinaStockMarket.GetRelativeStrongEndDay(), 19900103);
  }



}