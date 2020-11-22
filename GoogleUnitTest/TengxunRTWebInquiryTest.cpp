#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"MockTengxunRTWebInquiry.h"

using namespace std;
using namespace testing;

namespace StockAnalysisTest {
  class CTengxunRTWebInquiryTest : public ::testing::Test {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      gl_pChinaStakeMarket->ResetTengxunRTDataInquiringIndex();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStakeMarket->SetSystemReady(false);
      gl_pChinaStakeMarket->ResetTengxunRTDataInquiringIndex();
    }
  public:
    CMockTengxunRTWebInquiry m_TengxunRTWebInquiry; // 腾讯实时数据采集
  };

  TEST_F(CTengxunRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetConnection(), _T("TengxunRT"));
    EXPECT_EQ(m_TengxunRTWebInquiry.GetInquiringNumber(), 900) << _T("腾讯默认值");
  }

  TEST_F(CTengxunRTWebInquiryTest, TestGetWebData) {
    m_TengxunRTWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_TengxunRTWebInquiry.GetWebData());
    m_TengxunRTWebInquiry.SetReadingWebData(false);
    gl_pChinaStakeMarket->SetSystemReady(true);
    EXPECT_CALL(m_TengxunRTWebInquiry, StartReadingThread)
      .Times(1);
    m_TengxunRTWebInquiry.GetWebData();
    EXPECT_TRUE(m_TengxunRTWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CTengxunRTWebInquiryTest, TestGetNextInquiryStr) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    CString str = m_TengxunRTWebInquiry.GetNextInquiringMiddleStr(1, false);
    EXPECT_STREQ(str, _T("sh600000"));
  }

  TEST_F(CTengxunRTWebInquiryTest, TestPrepareNextInquiringStr) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    EXPECT_TRUE(m_TengxunRTWebInquiry.PrepareNextInquiringStr());
    CString str = m_TengxunRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(21), _T("http://qt.gtimg.cn/q="));
    gl_pChinaStakeMarket->SetSystemReady(false);
  }

  TEST_F(CTengxunRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_TengxunRTWebInquiry.ReportStatus(1));
  }
}