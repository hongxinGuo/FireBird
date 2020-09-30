#pragma once

#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"MockSinaRTWebInquiry.h"
#include"MockTengxunRTWebInquiry.h"
#include"MockNeteaseRTWebInquiry.h"
#include"MockNeteaseDayLineWebInquiry.h"
#include"MockPotenDailyBriefingWebInquiry.h"
#include"MockChinaMarket.h"
using namespace testing;

#include"SetStockCode.h"
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
      gl_pChinaStockMarket = make_shared<CChinaMarket>();
      gl_pCrweberIndexMarket = make_shared<CCrweberIndexMarket>();
      gl_pPotenDailyBriefingMarket = make_shared<CPotenDailyBriefingMarket>();
      EXPECT_EQ(gl_vMarketPtr.size(), 0);
      gl_vMarketPtr.push_back(gl_pChinaStockMarket); // �й���Ʊ�г�
      gl_vMarketPtr.push_back(gl_pPotenDailyBriefingMarket); // poten.com�ṩ��ÿ�պ���ָ��
      gl_vMarketPtr.push_back(gl_pCrweberIndexMarket); // Crweber.com�ṩ��ÿ�պ���ָ��
#ifdef __GOOGLEMOCK__
      gl_pSinaRTWebInquiry = make_shared<CMockSinaRTWebInquiry>();
      gl_pTengxunRTWebInquiry = make_shared<CMockTengxunRTWebInquiry>();
      gl_pNeteaseRTWebInquiry = make_shared<CMockNeteaseRTWebInquiry>();
      gl_pNeteaseDLWebInquiry = make_shared<CMockNeteaseDLWebInquiry>();
      gl_pNeteaseDLWebInquirySecond = make_shared<CMockNeteaseDLWebInquiry>();
      gl_pNeteaseDLWebInquiryThird = make_shared<CMockNeteaseDLWebInquiry>();
      gl_pNeteaseDLWebInquiryFourth = make_shared<CMockNeteaseDLWebInquiry>();
      gl_pPotenDailyBriefingWebInquiry = make_shared<CMockPotenDailyBriefingWebInquiry>();
      gl_pCrweberIndexWebInquiry = make_shared<CMockCrweberIndexWebInquiry>();
#else
      gl_pSinaRTWebInquiry = make_shared<CSinaRTWebInquiry>();
      gl_pTengxunRTWebInquiry = make_shared<CTengxunRTWebInquiry>();
      gl_pNeteaseRTWebInquiry = make_shared<CNeteaseRTWebInquiry>();
      gl_pNeteaseDLWebInquiry = make_shared<CNeteaseDLWebInquiry>();
      gl_pNeteaseDLWebInquirySecond = make_shared<CNeteaseDLWebInquiry>();
      gl_pNeteaseDLWebInquiryThird = make_shared<CNeteaseDLWebInquiry>();
      gl_pNeteaseDLWebInquiryFourth = make_shared<CNeteaseDLWebInquiry>();
      gl_pPotenDailyBriefingWebInquiry = make_shared<CPotenDailyBriefingWebInquiry>();
      gl_pCrweberIndexWebInquiry = make_shared<CCrweberIndexWebInquiry>();
#endif

      CChinaStockPtr pStock = nullptr;
      // ���ù�Ʊ��״̬������װ��ʵ��״̬��
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        pStock = gl_pChinaStockMarket->GetStock(i);
        pStock->SetDLEndDate(-1);
        EXPECT_TRUE(pStock->IsDLNeedUpdate());
        //if (!pStock->IsDLNeedUpdate()) pStock->SetDLNeedUpdate(true);
      }
      // ��ʼ����Ծ��Ʊ��ʶ
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_fNormalMode);
      CSetStockCode setStockCode;
      setStockCode.Open();
      while (!setStockCode.IsEOF()) {
        pStock = gl_pChinaStockMarket->GetStock(setStockCode.m_StockCode);
        EXPECT_FALSE(pStock->IsActive());
        pStock->SetIPOStatus(setStockCode.m_IPOStatus);
        pStock->SetMarket(setStockCode.m_StockType);
        pStock->SetStockCode(setStockCode.m_StockCode);
        CString str = setStockCode.m_StockName; // ��str�м����һ�£��Ϳ��Զ�ȡUniCode��ʽ��m_StockName�ˡ�
        pStock->SetStockName(str);
        pStock->SetDLStartDate(setStockCode.m_DLStartDate);
        if (pStock->GetDLEndDate() < setStockCode.m_DLEndDate) { // ��ʱһ����Ʊ���ж����¼������������Ϊ׼��
          pStock->SetDLEndDate(setStockCode.m_DLEndDate);
        }
        if (setStockCode.m_IPOStatus == __STOCK_IPOED__) {
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
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
      gl_pSinaRTWebInquiry = nullptr;
      gl_pTengxunRTWebInquiry = nullptr;
      gl_pNeteaseRTWebInquiry = nullptr;
      gl_pNeteaseDLWebInquiry = nullptr; // ����������ʷ����
      gl_pNeteaseDLWebInquirySecond = nullptr; // ����������ʷ����
      gl_pNeteaseDLWebInquiryThird = nullptr; // ����������ʷ����
      gl_pNeteaseDLWebInquiryFourth = nullptr; // ����������ʷ����
      gl_pPotenDailyBriefingWebInquiry = nullptr;
      gl_pCrweberIndexWebInquiry = nullptr;

      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedProcessNumber(), 0);
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
      while (gl_ThreadStatus.GetNumberOfRunningThread() > 0) Sleep(1);
      gl_vMarketPtr.clear();
      gl_pChinaStockMarket = nullptr;
      gl_pCrweberIndexMarket = nullptr;
      gl_pPotenDailyBriefingMarket = nullptr;
    }
  };
}