#include"stdafx.h"
#include"pch.h"
#include"ChinaMarket.h"

#include"RTData.h"

static CCrweberIndexWebInquiry m_CrweberIndexWebData; // crweber.com上的每日油运指数

namespace StockAnalysisTest {
  TEST(CrweberIndexWebDataTest, TestInitialize) {
    EXPECT_STREQ(m_CrweberIndexWebData.GetInquiringStringPrefix(), _T("http://www.crweber.com"));
    EXPECT_STREQ(m_CrweberIndexWebData.GetInquiringStringSuffix(), _T(""));
  }
}