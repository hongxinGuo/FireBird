#pragma once
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockWorldMarket.h"
#include"MockChinaMarket.h"

#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockFinnhubWebInquiry.h"
#include"MockTiingoWebInquiry.h"
#include"MockQuandlWebInquiry.h"
#include"MockChinaMarket.h"
using namespace testing;

#include"WebInquirer.h"

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  // 构造析构时开销大的Mock类声明为全局变量，在测试系统退出时才析构,这样容易在测试信息窗口中发现故障
  CMockWorldMarketPtr gl_pMockWorldMarket;
  CMockChinaMarketPtr gl_pMockChinaMarket;

  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    TestEnvironment(void) {
    }
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) override {
      // 下列全局智能指针为实际类
      gl_pChinaStockMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      gl_pWorldMarket = make_shared<CWorldMarket>();
      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pWorldMarket); // 美国股票市场
      gl_vMarketPtr.push_back(gl_pChinaStockMarket); // 中国股票市场
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com提供的每日航运指数
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com提供的每日航运指数
#ifdef __GOOGLEMOCK__
      gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
      gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
      gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
      gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
      gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
      gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiry2 = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pFinnhubWebInquiry = make_shared<CMockFinnhubWebInquiry>();
      gl_pTiingoWebInquiry = make_shared<CMockTiingoWebInquiry>();
      gl_pQuandlWebInquiry = make_shared<CMockQuandlWebInquiry>();
#else
      gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
      gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
      gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
      gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
      gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
      gl_pNeteaseDayLineWebInquiry = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiry2 = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiry3 = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiry4 = make_shared<CNeteaseDayLineWebInquiry>();
      gl_pFinnhubWebInquiry = make_shared<CFinnhubWebInquiry>();
      gl_pTiingoWebInquiry = make_shared<CTiingoWebInquiry>();
      gl_pQuandlWebInquiry = make_shared<CQuandlWebInquiry>();
#endif

      ASSERT_TRUE(gl_fTestMode);
      ASSERT_FALSE(gl_fNormalMode);

      gl_pChinaStockMarket->LoadStockCodeDB(); // 初始化活跃股票标识. 目前此测试股票代码总数为4833.
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        auto pStock = gl_pChinaStockMarket->GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }
      EXPECT_GT(gl_pChinaStockMarket->GetTotalStock(), 4800);
      EXPECT_TRUE(gl_pChinaStockMarket->TooManyStockDayLineNeedUpdate());
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      gl_pChinaStockMarket->SetSystemReady(true);
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());

      gl_pWorldMarket->LoadStockDB(); // 初始化活跃股票标识. 目前此测试股票代码总数为2462,皆为上海交易所股票.
      gl_pWorldMarket->LoadCountryDB();
      gl_pWorldMarket->LoadWorldExchangeDB();
      gl_pWorldMarket->LoadForexExchange();
      gl_pWorldMarket->LoadForexSymbol();
      gl_pWorldMarket->LoadWorldChoicedStock();

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // 这里要故意将这几个Mock变量设置为nullptr，这样就能够在测试输出窗口（不是Test Expxplorer窗口）中得到测试结果。
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      // 重置以下指针，以测试是否存在没有配对的Mock。
      gl_pMockChinaMarket = nullptr;
      gl_pMockWorldMarket = nullptr;

      gl_pSinaRTWebInquiry = nullptr;
      gl_pTengxunRTWebInquiry = nullptr;
      gl_pNeteaseRTWebInquiry = nullptr;
      gl_pNeteaseDayLineWebInquiry = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquiry2 = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquiry3 = nullptr; // 网易日线历史数据
      gl_pNeteaseDayLineWebInquiry4 = nullptr; // 网易日线历史数据
      gl_pFinnhubWebInquiry = nullptr;
      gl_pPotenDailyBriefingWebInquiry = nullptr;
      gl_pCrweberIndexWebInquiry = nullptr;
      gl_pFinnhubWebInquiry = nullptr;
      gl_pTiingoWebInquiry = nullptr;
      gl_pQuandlWebInquiry = nullptr;

      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
      while (gl_ThreadStatus.IsWebInquiringThreadRunning()) Sleep(1);
      gl_vMarketPtr.clear();
      gl_pWorldMarket = nullptr;
      gl_pChinaStockMarket = nullptr;
      gl_pCrweberIndexMarket = nullptr;
      gl_pPotenDailyBriefingMarket = nullptr;
    }
  };
}