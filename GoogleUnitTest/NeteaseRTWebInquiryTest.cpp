#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"

#include"NeteaseRTWebInquiry.h"

using namespace std;
#include<atomic>

static CNeteaseRTWebInquiry m_NeteaseRTWebData; // 新浪实时数据采集

namespace StockAnalysisTest {
  TEST(CNeteaseRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseRTWebData.GetInquiringStringPrefix(), _T("http://api.money.126.net/data/feed/"));
    EXPECT_STREQ(m_NeteaseRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_NeteaseRTWebData.GetConnection(), _T("NeteaseRT"));
  }

  TEST(CNeteaseRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseRTWebData.ReportStatus(1));
  }

  TEST(CNeteaseRTWebInquiryTest, TestGetNextInquiryStr) {
    CString str = m_NeteaseRTWebData.GetNextInquiringStr(1, false); // 得到第一个股票字符串
    EXPECT_STREQ(str, _T("0600000"));
  }
}