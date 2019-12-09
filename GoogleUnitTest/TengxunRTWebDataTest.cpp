#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"TengxunWebRTData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(TengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_TengxunWebRTData.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(gl_TengxunWebRTData.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(gl_TengxunWebRTData.IsReadingSucceed());
  }
}