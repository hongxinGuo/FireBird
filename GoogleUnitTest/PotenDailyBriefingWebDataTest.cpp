#include"pch.h"
#include"Market.h"

#include"RTData.h"

static CPotenDailyBriefingWebData m_PotenDailyBriefingWebData; // poten.com上的每日油运指数

namespace StockAnalysisTest {
  TEST(PotenDailyBriefingWebDataTest, TestInitialize) {
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringStringPrefix(), _T("http://energy.poten.com/poten-daily-briefing-webpage-"));
    EXPECT_STREQ(m_PotenDailyBriefingWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_EQ(m_PotenDailyBriefingWebData.GetInquiringDay(), 20180411);
  }
}