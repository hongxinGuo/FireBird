#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"NeteaseDayLineWebData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(NeteaseDayLineWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_NeteaseDayLineWebData.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(gl_NeteaseDayLineWebData.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
    EXPECT_TRUE(gl_NeteaseDayLineWebData.IsNeedProcessingCurrentWebData());
    EXPECT_TRUE(gl_NeteaseDayLineWebData.IsReadingSucceed());
  }

  TEST(NeteaseDayLineWebDataTest, TestReportDataError) {
    gl_NeteaseDayLineWebData.ReportDataError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("网易日线历史数据有误"));
  }

  TEST(NeteaseDayLineWebDataTest, TestReportCommunicationError) {
    gl_NeteaseDayLineWebData.ReportCommunicationError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file ：http://quotes.money.163.com"));
  }

  TEST(NeteaseDayLineWebDataTest, TestGetInquiringStr) {
    CString str = _T("abc");
    EXPECT_EQ(gl_NeteaseDayLineWebData.GetInquiringStr(str), 0);
    EXPECT_STREQ(str, _T(""));
  }
}