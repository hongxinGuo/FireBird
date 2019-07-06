#include"stdafx.h"

#include"pch.h"

#include"Stock.h"

namespace StockAnalysisTest {
  TEST(StockTest, TestIsChoiced) {
    CStock stock;
    stock.SetChoicedFlag(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoicedFlag(false);
    EXPECT_FALSE(stock.IsChoiced());
  }

  TEST(StockTest, TestIsMinLineUpdated) {
    CStock stock;
    stock.SetMinLineUpdated(true);
    EXPECT_TRUE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(false);
    EXPECT_FALSE(stock.IsMinLineUpdated());
  }
}