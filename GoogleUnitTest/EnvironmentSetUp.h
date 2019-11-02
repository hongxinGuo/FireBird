#pragma once

#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"Stock.h"

#include"SetStockCode.h"

using namespace testing;

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) {
      // 重置股票池状态（因已装入实际状态）
      for (auto pStock : gl_ChinaStockMarket.m_vChinaMarketAStock) {
        pStock->SetDayLineEndDay(-1);
        pStock->SetDayLineNeedUpdate(true);
      }
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_ChinaStockMarket.GetTotalActiveStock(), 0);
    }

    virtual void TearDown(void) {
    }
  };
}