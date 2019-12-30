#include"stdafx.h"
#include"pch.h"

#include"Market.h"

#include"NeteaseWebDayLineData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(NeteaseWebDayLineDataTest, TestInitialize) {
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
  }

  TEST(NeteaseWebDayLineDataTest, TestGetInquiringStr) {
    CString str = _T("abc");
    EXPECT_EQ(gl_NeteaseWebDayLineData.GetInquiringStr(str), 0);
    EXPECT_STREQ(str, _T(""));
  }

  TEST(NeteaseWebDayLineDataTest, TestSetDownLoadingStockCode) {
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetDownLoadingStockCode(), _T(""));
    gl_NeteaseWebDayLineData.SetDownLoadingStockCode(_T("1000001"));
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetDownLoadingStockCode(), _T("sz000001"));
    gl_NeteaseWebDayLineData.SetDownLoadingStockCode(_T("0600001"));
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetDownLoadingStockCode(), _T("sh600001"));
    gl_NeteaseWebDayLineData.SetDownLoadingStockCode(_T("2600001"));
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetDownLoadingStockCode(), _T("2600001"));
  }
}