#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"TengxunRTWebInquiry.h"

using namespace std;

static CTengxunRTWebInquiry m_TengxunRTWebInquiry; // 腾讯实时数据采集

namespace StockAnalysisTest {
  class CTengxunWebRTDataTest : public ::testing::Test {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      gl_ChinaStockMarket.ResetTengxunRTDataInquiringIndex();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ChinaStockMarket.ResetTengxunRTDataInquiringIndex();
    }
  public:
  };

  TEST_F(CTengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetConnection(), _T("TengxunRT"));
  }

  TEST_F(CTengxunWebRTDataTest, TestGetNextInquiryStr) {
    gl_ChinaStockMarket.SetSystemReady(true);
    CString str = m_TengxunRTWebInquiry.GetNextInquiringMiddleStr(1, false);
    EXPECT_STREQ(str, _T("sh600000"));
  }

  TEST_F(CTengxunWebRTDataTest, TestPrepareNextInquiringStr) {
    gl_ChinaStockMarket.SetSystemReady(true);
    EXPECT_TRUE(m_TengxunRTWebInquiry.PrepareNextInquiringStr());
    CString str = m_TengxunRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(21), _T("http://qt.gtimg.cn/q="));
    gl_ChinaStockMarket.SetSystemReady(false);
  }

  TEST_F(CTengxunWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(m_TengxunRTWebInquiry.ReportStatus(1));
  }
}