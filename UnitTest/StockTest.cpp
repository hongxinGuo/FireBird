#include"stdafx.h"

#include"pch.h"

#include"Stock.h"

namespace StockAnalysisTest {
  TEST(StockTest, TestIsChoiced) {
    CStock stock;
    EXPECT_FALSE(stock.IsChoiced());
    stock.SetChoicedFlag(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoicedFlag(false);
    EXPECT_FALSE(stock.IsChoiced());
  }

  TEST(StockTest, TestIsMinLineUpdated) {
    CStock stock;
    EXPECT_FALSE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(true);
    EXPECT_TRUE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(false);
    EXPECT_FALSE(stock.IsMinLineUpdated());
  }

  TEST(StockTest, TestIsDayLineUpdated) {
    CStock stock;
    EXPECT_FALSE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(true);
    EXPECT_TRUE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(false);
    EXPECT_FALSE(stock.IsDayLineUpdated());
  }

  TEST(StockTest, TestIsDayLineLoaded) {
    CStock stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(true);
    EXPECT_TRUE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(false);
    EXPECT_FALSE(stock.IsDayLineLoaded());
  }

  TEST(StockTest, TestIsStartCalculating) {
    CStock stock;
    EXPECT_FALSE(stock.IsStartCalculating());
    EXPECT_FALSE(stock.SetStartCalculating(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stock.IsStartCalculating());
    EXPECT_TRUE(stock.SetStartCalculating(true));
    EXPECT_TRUE(stock.IsStartCalculating());
    EXPECT_FALSE(stock.SetStartCalculating(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stock.IsStartCalculating());
  }

  TEST(StockTest, TestIsDayNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CStock stock;
    stock.SetDayLineNeedSavingFlag(true);
    EXPECT_TRUE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSavingFlag(false);
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
  }

  TEST(StockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CStockRTDataPtr pData = make_shared<CStockRTData>();
    pData->m_iStockCode = 600000;
    CStock stock;
    EXPECT_EQ(stock.GetRTDataDequeSize(), 0);
    stock.PushRTStockData(pData);
    EXPECT_EQ(stock.GetRTDataDequeSize(), 1);
    CStockRTDataPtr pData2 = stock.PopRTStockData();
    EXPECT_EQ(pData2->m_iStockCode, 600000);
    EXPECT_EQ(stock.GetRTDataDequeSize(), 0);
  }
}