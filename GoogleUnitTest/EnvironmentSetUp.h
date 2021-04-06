#pragma once
#include"pch.h"

#ifndef __GOOGLEMOCK__
#error _T("������ϵͳ����ʹ��GOOGLE MOCK")
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
  // ��������ʱ�������Mock������Ϊȫ�ֱ������ڲ���ϵͳ�˳�ʱ������,���������ڲ�����Ϣ�����з��ֹ���
  CMockWorldMarketPtr gl_pMockWorldMarket;
  CMockChinaMarketPtr gl_pMockChinaMarket;
  CMockMainFrame* gl_pMockMainFrame;

  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
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

      // ����������ȡ����ʹ��Mock�࣬�Է�ֹ����ʵ�ʵ������ȡ������
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

      // ����ȫ������ָ��Ϊʵ����
      gl_pChinaMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      gl_pWorldMarket = make_shared<CWorldMarket>();
      gl_pChinaMarket->ResetMarket();
      gl_pWorldMarket->ResetMarket();

      //_CrtMemCheckpoint(&state);

      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pWorldMarket); // ������Ʊ�г�
      gl_vMarketPtr.push_back(gl_pChinaMarket); // �й���Ʊ�г�
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com�ṩ��ÿ�պ���ָ��
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com�ṩ��ÿ�պ���ָ��

      gl_pMockChinaMarket = make_shared<CMockChinaMarket>();
      gl_pMockChinaMarket->ResetMarket();
      gl_pMockWorldMarket = make_shared<CMockWorldMarket>(); // �ڴ����ɣ���ȫ��TearDown�Ÿ�ֵnullptr.�������׿���������Ϣ
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
      // ����Ҫ���⽫�⼸��Mock��������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Expxplorer���ڣ��еõ����Խ����
      EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
      // ��������ָ�룬�Բ����Ƿ����û����Ե�Mock��
      gl_pMockChinaMarket = nullptr;
      gl_pMockWorldMarket = nullptr;

      if (CMFCVisualManager::GetInstance() != NULL) {
        delete CMFCVisualManager::GetInstance(); // ������MainFrameʱ��������һ���Ӿ����������ʶ��ڴ�ɾ��֮��
      }

      for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
        auto pStock = gl_pChinaMarket->GetStock(i);
        if (pStock->IsUpdateProfileDB()) {
          int i = 0;
          i++;
        }
        pStock->SetUpdateProfileDB(false); // gl_pMockMainFrameʹ����������gl_pChinaMarket,�˴����ô˱�ʶ����ֹ�ṹgl_pMockMainFrameʱ�������ݿ⡣
      }
      delete gl_pMockMainFrame;
      EXPECT_TRUE(gl_fExitingSystem) << "MainFrame����ʱ���ô˱�ʶ";
      gl_fExitingSystem = false;

      //_CrtMemDumpAllObjectsSince(&state);

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