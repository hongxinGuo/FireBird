#include"stdafx.h"
#include"pch.h"
#include"WebInquirer.h"

#include"RTData.h"

static CCrweberIndexWebInquiry m_CrweberIndexWebData; // crweber.com上的每日油运指数

namespace StockAnalysisTest {
  TEST(CrweberIndexWebDataTest, TestInitialize) {
    EXPECT_STREQ(m_CrweberIndexWebData.GetInquiringStringPrefix(), _T("http://www.crweber.com"));
    EXPECT_STREQ(m_CrweberIndexWebData.GetInquiringStringSuffix(), _T(""));
  }

  TEST(CrweberIndexWebDataTest, TestPrepareInquiringStr) {
    EXPECT_TRUE(m_CrweberIndexWebData.PrepareNextInquiringStr());
    CString str = m_CrweberIndexWebData.GetInquiringString();
    EXPECT_STREQ(str, _T("http://www.crweber.com"));
  }
}