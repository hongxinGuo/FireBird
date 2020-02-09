#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"

#include"NeteaseDayLineWebInquiry.h"

using namespace std;

static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebData; // 网易日线历史数据

namespace StockAnalysisTest {
  TEST(NeteaseWebDayLineDataTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
  }

  TEST(NeteaseWebDayLineDataTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseDayLineWebData.ReportStatus(1));
  }

  TEST(NeteaseWebDayLineDataTest, TestSetDownLoadingStockCode) {
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T(""));
    m_NeteaseDayLineWebData.SetDownLoadingStockCode(_T("1000001"));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T("sz000001"));
    m_NeteaseDayLineWebData.SetDownLoadingStockCode(_T("0600001"));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T("sh600001"));
    m_NeteaseDayLineWebData.SetDownLoadingStockCode(_T("2600001"));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T("2600001"));
  }
}