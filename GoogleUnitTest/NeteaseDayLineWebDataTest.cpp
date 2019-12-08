#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"NeteaseDayLineWebData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(NeteaseDayLineWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_NeteaseDayLineWebData.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(gl_NeteaseDayLineWebData.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
    EXPECT_FALSE(gl_NeteaseDayLineWebData.IsReadingSucceed());
  }

  TEST(NeteaseDayLineWebDataTest, TestGetInquiringStr) {
    CString str = _T("abc");
    EXPECT_EQ(gl_NeteaseDayLineWebData.GetInquiringStr(str), 0);
    EXPECT_STREQ(str, _T(""));
  }

  TEST(NeteaseDayLineWebDataTest, TestReportDataError) {
    if (gl_NeteaseDayLineWebData.IsReportStatus()) {
      gl_NeteaseDayLineWebData.ReportDataError();
      EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("网易日线历史数据有误"));
    }
  }

  TEST(NeteaseDayLineWebDataTest, TestReportCommunicationError) {
    if (gl_NeteaseDayLineWebData.IsReportStatus()) {
      gl_NeteaseDayLineWebData.ReportCommunicationError();
      EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file ：http://quotes.money.163.com"));
    }
  }
}