#include"stdafx.h"
#include"pch.h"
#include"Market.h"

#include"RTData.h"

static CCrweberIndexWebData m_CrweberIndexWebData; // crweber.com�ϵ�ÿ������ָ��

namespace StockAnalysisTest {
  TEST(CrweberIndexWebDataTest, TestInitialize) {
    EXPECT_STREQ(m_CrweberIndexWebData.GetInquiringStringPrefix(), _T("http://www.crweber.com"));
    EXPECT_STREQ(m_CrweberIndexWebData.GetInquiringStringSuffix(), _T(""));
  }
}