#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"TengxunRTWebInquiry.h"

using namespace std;

static CTengxunRTWebInquiry m_TengxunRTWebInquiry; // ��Ѷʵʱ���ݲɼ�

namespace StockAnalysisTest {
  TEST(TengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringSuffix(), _T(""));
  }
}