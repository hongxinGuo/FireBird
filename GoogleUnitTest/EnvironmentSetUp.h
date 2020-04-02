#pragma once

#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockChinaMarket.h"
using namespace Testing;

#include"SetStockCode.h"
#include"WebInquirer.h"

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    TestEnvironment(void) {
      // 下列全局智能指针为实际类
      gl_pChinaStockMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pChinaStockMarket); // 中国股票市场
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com提供的每日航运指数
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com提供的每日航运指数

      gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
      gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
#ifdef __GOOGLEMOCK__
      gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquirySecond = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiryThird = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiryFourth = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiryFifth = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquirySixth = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
      gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
#else
      gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquirySecond = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiryThird = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiryFourth = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiryFifth = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquirySixth = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
      gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
#endif
    }

    virtual ~TestEnvironment() {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr);
      while (gl_ThreadStatus.GetNumberOfRunningThread() > 0) Sleep(1);
      gl_vMarketPtr.clear();
      gl_pChinaStockMarket = nullptr;
      gl_pCrweberIndexMarket = nullptr;
      gl_pPotenDailyBriefingMarket = nullptr;
    }

    virtual void SetUp(void) override {
      CChinaStockPtr pStock = nullptr;
      // 重置股票池状态（因已装入实际状态）
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        pStock = gl_pChinaStockMarket->GetStock(i);
        pStock->SetDayLineEndDay(-1);
        EXPECT_TRUE(pStock->IsDayLineNeedUpdate());
        //if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }
      // 初始化活跃股票标识
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_fNormalMode);
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        pStock = gl_pChinaStockMarket->GetStock(setStockCode.m_StockCode);
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
          gl_pChinaStockMarket->IncreaseActiveStockNumber();
        }
        setStockCode.MoveNext();
      }
      setStockCode.Close();
      EXPECT_GT(gl_pChinaStockMarket->GetTotalActiveStock(), 0);
      gl_pChinaStockMarket->SetSystemReady(true);
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    }

    virtual void TearDown(void) override {
      // 这里要故意将这几个Mock变量设置为nullptr，这样就能够在测试输出窗口（不是Test Expxplorer窗口）中得到测试结果。
      gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquirySecond = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquiryThird = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquiryFourth = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquiryFifth = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquirySixth = nullptr; // 网易日线历史数据
      gl_pPotenDailyBriefingWebInquiry = nullptr;
      gl_pCrweberIndexWebInquiry = nullptr;

      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
    }
  };
}