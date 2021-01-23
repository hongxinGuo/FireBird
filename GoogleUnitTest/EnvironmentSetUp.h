#pragma once

#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStake.h"

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
  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    TestEnvironment(void) {
    }
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) override {
      // 下列全局智能指针为实际类
      gl_pChinaStakeMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      gl_pWorldMarket = make_shared<CWorldMarket>();
      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pWorldMarket); // 美国股票市场
      gl_vMarketPtr.push_back(gl_pChinaStakeMarket); // 中国股票市场
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com提供的每日航运指数
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com提供的每日航运指数
#ifdef __GOOGLEMOCK__
      gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
      gl_pSinaStakeRTWebInquiry = make_shared<CMockSinaStakeRTWebInquiry>();
      gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
      gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
      gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
      gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
      gl_pNeteaseDayLineWebInquiry = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiry2 = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiry3 = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pNeteaseDayLineWebInquiry4 = make_shared<CMockNeteaseDayLineWebInquiry>();
      gl_pFinnhubWebInquiry = make_shared<CMockFinnhubWebInquiry>();
      gl_pTiingoWebInquiry = make_shared<CMockTiingoWebInquiry>();
      gl_pQuandlWebInquiry = make_shared<CMockQuandlWebInquiry>();
#else
      gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
      gl_pSinaStakeRTWebInquiry = make_shared<CSinaStakeRTWebInquiry>();
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

      CChinaStakePtr pStake = nullptr;
      // 重置股票池状态（因已装入实际状态）
      for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStake(); i++) {
        pStake = gl_pChinaStakeMarket->GetStock(i);
        pStake->SetDayLineEndDate(-1);
        EXPECT_TRUE(pStake->IsDayLineNeedUpdate());
        //if (!pStake->IsDayLineNeedUpdate()) pStake->SetDayLineNeedUpdate(true);
      }
      // 初始化活跃股票标识
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_fNormalMode);
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        pStake = gl_pChinaStakeMarket->GetStock(setStockCode.m_StockCode);
        pStake->SetIPOStatus(setStockCode.m_IPOStatus);
        pStake->SetMarket(setStockCode.m_StockType);
        pStake->SetStakeCode(setStockCode.m_StockCode);
        CString str = setStockCode.m_StockName; // 用str中间过渡一下，就可以读取UniCode制式的m_StockName了。
        pStake->SetStakeName(str);
        pStake->SetDayLineStartDate(setStockCode.m_DayLineStartDate);
        if (pStake->GetDayLineEndDate() < setStockCode.m_DayLineEndDate) { // 有时一个股票会有多个记录，以最后的日期为准。
          pStake->SetDayLineEndDate(setStockCode.m_DayLineEndDate);
        }
        if (setStockCode.m_IPOStatus == __STAKE_IPOED__) {
          pStake->SetActive(true);
          gl_pChinaStakeMarket->IncreaseActiveStockNumber();
        }
        else {
          //EXPECT_FALSE(pStake->IsActive());
        }
        setStockCode.MoveNext();
      }
      setStockCode.Close();
      EXPECT_GT(gl_pChinaStakeMarket->GetTotalActiveStock(), 0);
      gl_pChinaStakeMarket->SetSystemReady(true);
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
    }

    virtual void TearDown(void) override {
      // 这里要故意将这几个Mock变量设置为nullptr，这样就能够在测试输出窗口（不是Test Expxplorer窗口）中得到测试结果。
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStakeMarket->GetTotalStock());
      gl_pSinaRTWebInquiry = nullptr;
      gl_pSinaStakeRTWebInquiry = nullptr;
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

      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedProcessNumber(), 0);
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
      while (gl_ThreadStatus.GetNumberOfRunningThread() > 0) Sleep(1);
      gl_vMarketPtr.clear();
      gl_pWorldMarket = nullptr;
      gl_pChinaStakeMarket = nullptr;
      gl_pCrweberIndexMarket = nullptr;
      gl_pPotenDailyBriefingMarket = nullptr;
      gl_pWorldMarket = nullptr;
    }
  };
}