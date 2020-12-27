#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"MockQuandlWebInquiry.h"

using namespace std;
using namespace testing;

namespace StockAnalysisTest {
  static bool m_fSystemStatus;
  class CQuandlWebInquiryTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      m_fSystemStatus = gl_pAmericaMarket->IsSystemReady();
      gl_pAmericaMarket->SetSystemReady(true); // Quandl引擎必须等待系统初始化后才可使用。
    }

    static void TearDownTestSuite(void) {
      gl_pAmericaMarket->SetSystemReady(m_fSystemStatus);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pAmericaMarket->CalculateTime();
      EXPECT_TRUE(gl_pAmericaMarket->IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_TRUE(gl_pAmericaMarket->IsResetMarket());
      gl_pAmericaMarket->SetResetMarket(true);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
    CMockQuandlWebInquiry m_QuandlWebInquiry; // 网易日线历史数据
  };
  TEST_F(CQuandlWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_QuandlWebInquiry.GetInquiringStringPrefix(), _T(""));
    EXPECT_STREQ(m_QuandlWebInquiry.GetInquiringStringSuffix(), _T("&api_key=zBMXMyoTyiy_N3pMb3ex"));
    EXPECT_STREQ(m_QuandlWebInquiry.GetConnection(), _T("Quandl"));
  }

  TEST_F(CQuandlWebInquiryTest, TestGetWebData) {
    m_QuandlWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_QuandlWebInquiry.GetWebData());
    m_QuandlWebInquiry.SetReadingWebData(false);
    gl_pAmericaMarket->SetSystemReady(true);
    EXPECT_CALL(m_QuandlWebInquiry, StartReadingThread)
      .Times(1);
    m_QuandlWebInquiry.GetWebData();
    EXPECT_TRUE(m_QuandlWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CQuandlWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_QuandlWebInquiry.ReportStatus(1));
  }

  TEST_F(CQuandlWebInquiryTest, TestPrepareNextInquiringStr) {
    CString str;
    gl_pAmericaMarket->SetSystemReady(true);
    for (int i = 0; i < 4; i++) {
      if (m_QuandlWebInquiry.PrepareNextInquiringStr()) {
        str = m_QuandlWebInquiry.GetInquiringString();
        EXPECT_STREQ(str.Right(29), _T("&api_key=zBMXMyoTyiy_N3pMb3ex"));
      }
      else EXPECT_EQ(str.GetLength(), 0);
    }
    gl_pAmericaMarket->SetSystemReady(false);
  }
}