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
  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
  public:
    TestEnvironment(void) {
    }
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) override {
      // ����ȫ������ָ��Ϊʵ����
      gl_pChinaStakeMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      gl_pWorldMarket = make_shared<CWorldMarket>();
      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pWorldMarket); // ������Ʊ�г�
      gl_vMarketPtr.push_back(gl_pChinaStakeMarket); // �й���Ʊ�г�
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com�ṩ��ÿ�պ���ָ��
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com�ṩ��ÿ�պ���ָ��
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
      // ���ù�Ʊ��״̬������װ��ʵ��״̬��
      for (int i = 0; i < gl_pChinaStakeMarket->GetTotalStake(); i++) {
        pStake = gl_pChinaStakeMarket->GetStock(i);
        pStake->SetDayLineEndDate(-1);
        EXPECT_TRUE(pStake->IsDayLineNeedUpdate());
        //if (!pStake->IsDayLineNeedUpdate()) pStake->SetDayLineNeedUpdate(true);
      }
      // ��ʼ����Ծ��Ʊ��ʶ
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_fNormalMode);
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        pStake = gl_pChinaStakeMarket->GetStock(setStockCode.m_StockCode);
        pStake->SetIPOStatus(setStockCode.m_IPOStatus);
        pStake->SetMarket(setStockCode.m_StockType);
        pStake->SetStakeCode(setStockCode.m_StockCode);
        CString str = setStockCode.m_StockName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
        pStake->SetStakeName(str);
        pStake->SetDayLineStartDate(setStockCode.m_DayLineStartDate);
        if (pStake->GetDayLineEndDate() < setStockCode.m_DayLineEndDate) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
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
      // ����Ҫ���⽫�⼸��Mock��������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Expxplorer���ڣ��еõ����Խ����
      EXPECT_FALSE(gl_pChinaStakeMarket->IsCurrentStockChanged());
      EXPECT_EQ(gl_pChinaStakeMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStakeMarket->GetTotalStock());
      gl_pSinaRTWebInquiry = nullptr;
      gl_pSinaStakeRTWebInquiry = nullptr;
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