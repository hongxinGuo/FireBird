#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"MockNeteaseRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace Testing;

static CMockNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // 新浪实时数据采集

namespace StockAnalysisTest {
  class CNeteaseRTWebInquiryTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();

      EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStockMarket->SetSystemReady(false);
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
  };

  TEST_F(CNeteaseRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringPrefix(), _T("http://api.money.126.net/data/feed/"));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetConnection(), _T("NeteaseRT"));
    EXPECT_EQ(m_NeteaseRTWebInquiry.GetInquiringNumber(), 600) << _T("网易默认值");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetWebData) {
    m_NeteaseRTWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_NeteaseRTWebInquiry.GetWebData());
    m_NeteaseRTWebInquiry.SetReadingWebData(false);
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(m_NeteaseRTWebInquiry, StartReadingThread)
      .Times(1);
    m_NeteaseRTWebInquiry.GetWebData();
    EXPECT_TRUE(m_NeteaseRTWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseRTWebInquiry.ReportStatus(1));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetNextInquiryMiddleStr) {
    gl_pChinaStockMarket->SetSystemReady(true);
    CString str = m_NeteaseRTWebInquiry.GetNextInquiringMiddleStr(1, false); // 得到第一个股票字符串
    EXPECT_STREQ(str, _T("0600000"));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestPrepareNextInquiringStr) {
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_TRUE(m_NeteaseRTWebInquiry.PrepareNextInquiringStr());
    CString str = m_NeteaseRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
  }
}