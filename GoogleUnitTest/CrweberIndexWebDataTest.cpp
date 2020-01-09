#include"stdafx.h"
#include"pch.h"
#include"Market.h"

#include"RTData.h"

namespace StockAnalysisTest {
  TEST(CrweberIndexWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_WebDataInquirer.m_CrweberIndexWebData.GetInquiringStringPrefix(), _T("http://www.crweber.com"));
    EXPECT_STREQ(gl_WebDataInquirer.m_CrweberIndexWebData.GetInquiringStringSuffix(), _T(""));
  }
}