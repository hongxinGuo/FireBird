#pragma once

#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"Stock.h"

#include"SetStockCode.h"

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
  public:
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) {
      // ���ù�Ʊ��״̬������װ��ʵ��״̬��
      for (auto pStock : gl_ChinaStockMarket.m_vChinaMarketAStock) {
        pStock->SetDayLineEndDay(-1);
        EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
        //if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }
      // ��ʼ����Ծ��Ʊ��ʶ
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_fNormalMode);
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(setStockCode.m_StockCode);
        EXPECT_FALSE(pStock->IsActive());
        pStock->SetIPOStatus(setStockCode.m_IPOed);
        pStock->SetMarket(setStockCode.m_StockType);
        pStock->SetStockCode(setStockCode.m_StockCode);
        pStock->SetDayLineStartDay(setStockCode.m_DayLineStartDay);
        pStock->SetDayLineEndDay(setStockCode.m_DayLineEndDay);
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