#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"TengxunRTWebInquiry.h"

using namespace std;

static CTengxunRTWebInquiry m_TengxunRTWebData; // ��Ѷʵʱ���ݲɼ�

namespace StockAnalysisTest {
  TEST(TengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(m_TengxunRTWebData.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(m_TengxunRTWebData.GetInquiringStringSuffix(), _T(""));
  }
}