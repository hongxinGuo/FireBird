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
      for (auto pStockID : gl_ChinaStockMarket.m_vChinaMarketAStock) {
        pStockID->SetDayLineEndDay(-1);
        pStockID->SetDayLineNeedUpdate(true);
      }
      // 初始化股票池
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        if (setStockCode.m_IPOed == __STOCK_IPOED__) {
          CStockPtr pStock = make_shared<CStock>();
          pStock->SetActive(true);
          pStock->SetStockCode(setStockCode.m_StockCode);
          //pStock->SetStockName(setStockCode.m_StockName); // 字符串制式不相符
          pStock->SetMarket(setStockCode.m_StockType);
          gl_ChinaStockMarket.AddStockToMarket(pStock);
        }
        setStockCode.MoveNext();
      }
      setStockCode.Close();
      EXPECT_GT(gl_ChinaStockMarket.GetTotalStock(), 0);
    }

    virtual void TearDown(void) {
    }
  };
}