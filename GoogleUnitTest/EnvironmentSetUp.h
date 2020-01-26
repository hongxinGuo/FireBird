#pragma once

#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"Stock.h"

#include"SetStockCode.h"

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
  public:
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) {
      CStockPtr pStock = nullptr;
      // ���ù�Ʊ��״̬������װ��ʵ��״̬��
      for (int i = 0; i < gl_ChinaStockMarket.GetTotalStock(); i++) {
        pStock = gl_ChinaStockMarket.GetStock(i);
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
        CStockPtr pStock = gl_ChinaStockMarket.GetStock(setStockCode.m_StockCode);
        EXPECT_FALSE(pStock->IsActive());
        pStock->SetIPOStatus(setStockCode.m_IPOed);
        pStock->SetMarket(setStockCode.m_StockType);
        pStock->SetStockCode(setStockCode.m_StockCode);
        CString str = setStockCode.m_StockName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
        pStock->SetStockName(str);
        pStock->SetDayLineStartDay(setStockCode.m_DayLineStartDay);
        if (pStock->GetDayLineEndDay() < setStockCode.m_DayLineEndDay) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
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