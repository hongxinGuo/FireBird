#include"pch.h"
#include"WebInquirer.h"

#include"RTData.h"

static CPotenDailyBriefingWebInquiry m_PotenDailyBriefingWebData; // poten.com上的每日油运指数

namespace StockAnalysisTest {
  TEST(PotenDailyBriefingWebDataTest, TestInitialize) {
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringStringPrefix(), _T("http://energy.poten.com/poten-daily-briefing-webpage-"));
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_EQ(m_PotenDailyBriefingWebData.GetInquiringDay(), 20180411);
  }

  TEST(PotenDailyBriefingWebDataTest, TestGetInquiringDay) {
    m_PotenDailyBriefingWebData.SetInquiringDay(19901212);
    EXPECT_EQ(m_PotenDailyBriefingWebData.GetInquiringDay(), 19901212);
  }

  TEST(PotenDailyBriefingWebDataTest, TestPrepareInquiringStr) {
    m_PotenDailyBriefingWebData.SetInquiringDay(19900101);
    EXPECT_TRUE(m_PotenDailyBriefingWebData.PrepareNextInquiringStr());
    CString str = _T("http://energy.poten.com/poten-daily-briefing-webpage-");
    str += _T("04/11/2018");
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringString(), str);
  }
}