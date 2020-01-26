#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"Stock.h"

#include"SetStockCode.h"

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) {
      CStockPtr pStock = nullptr;
      // 重置股票池状态（因已装入实际状态）
      for (int i = 0; i < gl_ChinaStockMarket.GetTotalStock(); i++) {
        pStock = gl_ChinaStockMarket.GetStock(i);
        pStock->SetDayLineEndDay(-1);
        EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
        //if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }
      // 初始话活跃股票标识
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_fNormalMode);
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        CStockPtr pStock = gl_ChinaStockMarket.GetStock(setStockCode.m_StockCode);
        EXPECT_FALSE(pStock->IsActive());
        pStock->SetIPOStatus(setStockCode.m_IPOed);
        pStock->SetMarket(setStockCode.m_StockType);
        pStock->SetStockCode(setStockCode.m_StockCode);
        CString str = setStockCode.m_StockName; // 用str中间过渡一下，就可以读取UniCode制式的m_StockName了。
        pStock->SetStockName(str);
        pStock->SetDayLineStartDay(setStockCode.m_DayLineStartDay);
        if (pStock->GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // 有时一个股票会有多个记录，以最后的日期为准。
          pStock->SetDayLineEndDay(setStockCode.m_DayLineEndDay);
        }
        if (setStockCode.m_IPOed == __STOCK_IPOED__) {
          pStock->SetActive(true);
          gl_ChinaStockMarket.IncreaseActiveStockNumber();
        }
        setStockCode.MoveNext();
      }
      setStockCode.Close();
      EXPECT_GT(gl_ChinaStockMarket.GetTotalActiveStock(), 0);
    }

    virtual void TearDown(void) {
    }
  };
}