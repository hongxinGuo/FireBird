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
      // ��ʼ����Ʊ��
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        if (setStockCode.m_IPOed == __STOCK_IPOED__) {
          CStockPtr pStock = make_shared<CStock>();
          pStock->SetActive(true);
          pStock->SetStockCode(setStockCode.m_StockCode);
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