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
  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
  public:
    TestEnvironment(void) {
      // ����ȫ������ָ��Ϊʵ����
      gl_pChinaStockMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pChinaStockMarket); // �й���Ʊ�г�
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com�ṩ��ÿ�պ���ָ��
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com�ṩ��ÿ�պ���ָ��

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
      // ���ù�Ʊ��״̬������װ��ʵ��״̬��
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        pStock = gl_pChinaStockMarket->GetStock(i);
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
        pStock = gl_pChinaStockMarket->GetStock(setStockCode.m_StockCode);
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
      // ����Ҫ���⽫�⼸��Mock��������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Expxplorer���ڣ��еõ����Խ����
      gl_pNeteaseDayLineWebInquiry = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquirySecond = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquiryThird = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquiryFourth = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquiryFifth = nullptr; // ����������ʷ����
      gl_pNeteaseDayLineWebInquirySixth = nullptr; // ����������ʷ����
      gl_pPotenDailyBriefingWebInquiry = nullptr;
      gl_pCrweberIndexWebInquiry = nullptr;

      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
    }
  };
}