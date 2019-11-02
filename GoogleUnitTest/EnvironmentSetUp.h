#pragma once

#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"Stock.h"

#include"SetStockCode.h"

using namespace testing;

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
  public:
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) {
      // ���ù�Ʊ��״̬������װ��ʵ��״̬��
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