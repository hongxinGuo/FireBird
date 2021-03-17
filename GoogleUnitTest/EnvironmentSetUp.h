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
  // ��������ʱ�������Mock������Ϊȫ�ֱ������ڲ���ϵͳ�˳�ʱ������,���������ڲ�����Ϣ�����з��ֹ���
  CMockWorldMarketPtr gl_pMockWorldMarket;
  CMockChinaMarketPtr gl_pMockChinaMarket;

  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
  public:
    TestEnvironment(void) {
    }
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) override {
      // ����ȫ������ָ��Ϊʵ����
      gl_pChinaStockMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      gl_pWorldMarket = make_shared<CWorldMarket>();
      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pWorldMarket); // ������Ʊ�г�
      gl_vMarketPtr.push_back(gl_pChinaStockMarket); // �й���Ʊ�г�
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com�ṩ��ÿ�պ���ָ��
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com�ṩ��ÿ�պ���ָ��
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

      gl_pChinaStockMarket->LoadStockCodeDB(); // ��ʼ����Ծ��Ʊ��ʶ. Ŀǰ�˲��Թ�Ʊ��������Ϊ4833.
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        auto pStock = gl_pChinaStockMarket->GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }
      EXPECT_GT(gl_pChinaStockMarket->GetTotalStock(), 4800);
      EXPECT_TRUE(gl_pChinaStockMarket->TooManyStockDayLineNeedUpdate());
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      gl_pChinaStockMarket->SetSystemReady(true);
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());

      gl_pWorldMarket->LoadStockDB(); // ��ʼ����Ծ��Ʊ��ʶ. Ŀǰ�˲��Թ�Ʊ��������Ϊ2462,��Ϊ�Ϻ���������Ʊ.
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
      // ����Ҫ���⽫�⼸��Mock��������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Expxplorer���ڣ��еõ����Խ����
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      // ��������ָ�룬�Բ����Ƿ����û����Ե�Mock��
      gl_pMockChinaMarket = nullptr;
      gl_pMockWorldMarket = nullptr;

      gl_pSinaRTWebInquiry = nullptr;
      gl_pTengxunRTWebInquiry = nullptr;
      gl_pNeteaseRTWebInquiry = nullptr;
      gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquiry2 = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquiry3 = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquiry4 = nullptr; // ����������ʷ����
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