#include"stdafx.h"
#include"pch.h"

#include"Market.h"
#include"TengxunWebRTData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(TengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_TengxunWebRTData.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(gl_TengxunWebRTData.GetInquiringStringSuffix(), _T(""));
  }
}