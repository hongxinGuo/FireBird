#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"
#include"TengxunRTWebInquiry.h"

using namespace std;

static CTengxunRTWebInquiry m_TengxunRTWebInquiry; // 腾讯实时数据采集

namespace StockAnalysisTest {
  TEST(TengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_TengxunRTWebInquiry.GetConnection(), _T("TengxunRT"));
  }

  TEST(TengxunWebRTDataTest, TestGetNextInquiryStr) {
    CString str = m_TengxunRTWebInquiry.GetNextInquiringMiddleStr(1, false);
    EXPECT_STREQ(str, _T("sh600000"));
  }

  TEST(TengxunWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(m_TengxunRTWebInquiry.ReportStatus(1));
  }
}