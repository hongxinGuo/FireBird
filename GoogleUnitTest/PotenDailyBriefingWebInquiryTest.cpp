#include"pch.h"
#include"WebInquirer.h"

#include"WebRTData.h"

#include"MockPotenDailyBriefingWebInquiry.h"

using namespace testing;

namespace StockAnalysisTest {
  class CPotenDailyBriefingWebInquiryTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void TearDown(void) override {
      // clearup
      m_PotenDailyBriefingWebInquiry.SetInquiringDate(20180411);
      m_PotenDailyBriefingWebInquiry.SetReadingWebData(false);
    }

    CMockPotenDailyBriefingWebInquiry m_PotenDailyBriefingWebInquiry; // poten.com上的每日油运指数
  };

  TEST_F(CPotenDailyBriefingWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_PotenDailyBriefingWebInquiry.GetInquiringStringPrefix(), _T("http://energy.poten.com/poten-daily-briefing-webpage-"));
    EXPECT_STREQ(m_PotenDailyBriefingWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_EQ(m_PotenDailyBriefingWebInquiry.GetInquiringDate(), 20180411);
  }

  TEST_F(CPotenDailyBriefingWebInquiryTest, TestGetWebData) {
    m_PotenDailyBriefingWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_PotenDailyBriefingWebInquiry.GetWebData());
    m_PotenDailyBriefingWebInquiry.SetReadingWebData(false);
    EXPECT_CALL(m_PotenDailyBriefingWebInquiry, StartReadingThread)
      .Times(1);
    m_PotenDailyBriefingWebInquiry.GetWebData();
    EXPECT_TRUE(m_PotenDailyBriefingWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CPotenDailyBriefingWebInquiryTest, TestGetInquiringDay) {
    m_PotenDailyBriefingWebInquiry.SetInquiringDate(19901212);
    EXPECT_EQ(m_PotenDailyBriefingWebInquiry.GetInquiringDate(), 19901212);
  }

  TEST_F(CPotenDailyBriefingWebInquiryTest, TestPrepareInquiringStr) {
    m_PotenDailyBriefingWebInquiry.SetInquiringDate(19900101);
    EXPECT_TRUE(m_PotenDailyBriefingWebInquiry.PrepareNextInquiringStr());
    CString str = _T("http://energy.poten.com/poten-daily-briefing-webpage-");
    str += _T("04/11/2018");
    EXPECT_STREQ(m_PotenDailyBriefingWebInquiry.GetInquiringString(), str);
  }
}