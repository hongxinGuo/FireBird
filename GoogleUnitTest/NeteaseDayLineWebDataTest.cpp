#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"NeteaseWebDayLineData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(NeteaseWebDayLineDataTest, TestInitialize) {
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(gl_NeteaseWebDayLineData.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
    EXPECT_FALSE(gl_NeteaseWebDayLineData.IsReadingSucceed());
  }

  TEST(NeteaseWebDayLineDataTest, TestGetInquiringStr) {
    CString str = _T("abc");
    EXPECT_EQ(gl_NeteaseWebDayLineData.GetInquiringStr(str), 0);
    EXPECT_STREQ(str, _T(""));
  }
}