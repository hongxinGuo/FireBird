#pragma once
#include"pch.h"

#ifndef __GOOGLEMOCK__
#error _T("本测试系统必须使用GOOGLE MOCK")
#endif

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockWorldMarket.h"
#include"MockChinaMarket.h"

#include"MockMainFrm.h"

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
  CMockMainFrame* gl_pMockMainFrame;

  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    TestEnvironment(void) {
      // _CrtMemCheckpoint(&state);
    }
    virtual ~TestEnvironment() {
      // _CrtMemDumpAllObjectsSince(&state);
    }
    _CrtMemState state;

    virtual void SetUp(void) override {
      ASSERT_TRUE(gl_fTestMode);
      ASSERT_FALSE(gl_fNormalMode);

      // 下列网络提取器皆使用Mock类，以防止出现实际的网络读取动作。
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

      // 下列全局智能指针为实际类
      gl_pChinaMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      gl_pWorldMarket = make_shared<CWorldMarket>();
      gl_pChinaMarket->ResetMarket();
      gl_pWorldMarket->ResetMarket();

      //_CrtMemCheckpoint(&state);

      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pWorldMarket); // 美国股票市场
      gl_vMarketPtr.push_back(gl_pChinaMarket); // 中国股票市场
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com提供的每日航运指数
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com提供的每日航运指数

      gl_pMockChinaMarket = make_shared<CMockChinaMarket>();
      gl_pMockChinaMarket->ResetMarket();
      gl_pMockWorldMarket = make_shared<CMockWorldMarket>(); // 在此生成，在全局TearDown才赋值nullptr.这样容易看到错误信息
      gl_pMockWorldMarket->ResetMarket();

      EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL);//
      gl_pMockMainFrame = new CMockMainFrame;
      EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL);//

      for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
        auto pStock = gl_pChinaMarket->GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }
      EXPECT_GT(gl_pChinaMarket->GetTotalStock(), 4800);
      EXPECT_TRUE(gl_pChinaMarket->TooManyStockDayLineNeedUpdate());
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
      gl_pChinaMarket->SetSystemReady(true);
      EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // 这里要故意将这几个Mock变量设置为nullptr，这样就能够在测试输出窗口（不是Test Expxplorer窗口）中得到测试结果。
      EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
      // 重置以下指针，以测试是否存在没有配对的Mock。
      gl_pMockChinaMarket = nullptr;
      gl_pMockWorldMarket = nullptr;

      if (CMFCVisualManager::GetInstance() != NULL) {
        delete CMFCVisualManager::GetInstance(); // 在生成MainFrame时，会生成一个视觉管理器。故而在此删除之。
      }

      for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
        auto pStock = gl_pChinaMarket->GetStock(i);
        if (pStock->IsUpdateProfileDB()) {
          int i = 0;
          i++;
        }
        pStock->SetUpdateProfileDB(false); // gl_pMockMainFrame使用了真正的gl_pChinaMarket,此处重置此标识，防止结构gl_pMockMainFrame时更新数据库。
      }
      delete gl_pMockMainFrame;
      EXPECT_TRUE(gl_fExitingSystem) << "MainFrame析构时设置此标识";
      gl_fExitingSystem = false;

      //_CrtMemDumpAllObjectsSince(&state);

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

      EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedProcessNumber(), 0);
      while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
      while (gl_ThreadStatus.IsWebInquiringThreadRunning()) Sleep(1);
      gl_pWorldMarket = nullptr;
      gl_pChinaMarket = nullptr;
      gl_pCrweberIndexMarket = nullptr;
      gl_pPotenDailyBriefingMarket = nullptr;
      gl_vMarketPtr.clear();

      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }
  };
}