#include"pch.h"
#include"WebInquirer.h"

#include"WebRTData.h"

#include"MockCrweberIndexWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CrweberIndexWebInquiryTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void TearDown(void) override {
      // clearup
      m_CrweberIndexWebInquiry.SetReadingWebData(false);
    }

    CMockCrweberIndexWebInquiry m_CrweberIndexWebInquiry; // poten.com上的每日油运指数
  };
  TEST_F(CrweberIndexWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_CrweberIndexWebInquiry.GetInquiringStringPrefix(), _T("http://www.crweber.com"));
    EXPECT_STREQ(m_CrweberIndexWebInquiry.GetInquiringStringSuffix(), _T(""));
  }

  TEST_F(CrweberIndexWebInquiryTest, TestGetWebData) {
    m_CrweberIndexWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_CrweberIndexWebInquiry.GetWebData());
    m_CrweberIndexWebInquiry.SetReadingWebData(false);
    EXPECT_CALL(m_CrweberIndexWebInquiry, StartReadingThread)
      .Times(1);
    m_CrweberIndexWebInquiry.GetWebData();
    EXPECT_TRUE(m_CrweberIndexWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CrweberIndexWebInquiryTest, TestPrepareInquiringStr) {
    EXPECT_TRUE(m_CrweberIndexWebInquiry.PrepareNextInquiringStr());
    CString str = m_CrweberIndexWebInquiry.GetInquiringString();
    EXPECT_STREQ(str, _T("http://www.crweber.com"));
  }
}