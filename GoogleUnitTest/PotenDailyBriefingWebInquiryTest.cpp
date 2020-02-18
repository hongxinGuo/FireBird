#include"pch.h"
#include"WebInquirer.h"

#include"RTData.h"

static CPotenDailyBriefingWebInquiry m_PotenDailyBriefingWebData; // poten.com上的每日油运指数

namespace StockAnalysisTest {
  class CPotenDailyBriefingWebDataTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void TearDown(void) override {
      // clearup
      m_PotenDailyBriefingWebData.SetInquiringDay(20180411);
    }
  };

  TEST_F(CPotenDailyBriefingWebDataTest, TestInitialize) {
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringStringPrefix(), _T("http://energy.poten.com/poten-daily-briefing-webpage-"));
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_EQ(m_PotenDailyBriefingWebData.GetInquiringDay(), 20180411);
  }

  TEST_F(CPotenDailyBriefingWebDataTest, TestGetInquiringDay) {
    m_PotenDailyBriefingWebData.SetInquiringDay(19901212);
    EXPECT_EQ(m_PotenDailyBriefingWebData.GetInquiringDay(), 19901212);
  }

  TEST_F(CPotenDailyBriefingWebDataTest, TestPrepareInquiringStr) {
    m_PotenDailyBriefingWebData.SetInquiringDay(19900101);
    EXPECT_TRUE(m_PotenDailyBriefingWebData.PrepareNextInquiringStr());
    CString str = _T("http://energy.poten.com/poten-daily-briefing-webpage-");
    str += _T("04/11/2018");
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringString(), str);
  }
}