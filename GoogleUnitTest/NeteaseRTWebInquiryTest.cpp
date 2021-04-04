#include"pch.h"

#include"globedef.h"

#include"MockNeteaseRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace testing;

namespace StockAnalysisTest {
  class CNeteaseRTWebInquiryTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pChinaMarket->CalculateTime();
      gl_pChinaMarket->SetNeteaseRTDataInquiringIndex(0);

      EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaMarket->SetSystemReady(false);
      gl_pChinaMarket->SetNeteaseRTDataInquiringIndex(0);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
    CMockNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // 新浪实时数据采集
  };

  TEST_F(CNeteaseRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringPrefix(), _T("http://api.money.126.net/data/feed/"));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetConnectionName(), _T("NeteaseRT"));
    EXPECT_EQ(m_NeteaseRTWebInquiry.GetInquiringNumber(), 700) << _T("网易默认值");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetWebData) {
    m_NeteaseRTWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_NeteaseRTWebInquiry.GetWebData());
    m_NeteaseRTWebInquiry.SetReadingWebData(false);
    gl_pChinaMarket->SetSystemReady(true);
    EXPECT_CALL(m_NeteaseRTWebInquiry, StartReadingThread)
      .Times(1);
    m_NeteaseRTWebInquiry.GetWebData();
    EXPECT_TRUE(m_NeteaseRTWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseRTWebInquiry.ReportStatus(1));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestPrepareNextInquiringStr) {
    gl_pChinaMarket->SetSystemReady(true);
    EXPECT_TRUE(m_NeteaseRTWebInquiry.PrepareNextInquiringStr());
    CString str = m_NeteaseRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
  }
}