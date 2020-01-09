#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"TengxunWebRTData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(TengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_WebDataInquirer.m_TengxunRTWebData.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(gl_WebDataInquirer.m_TengxunRTWebData.GetInquiringStringSuffix(), _T(""));
  }
}