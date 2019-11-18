#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"RTData.h"

namespace StockAnalysisTest {
  TEST(CrweberIndexWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_CrweberIndexWebData.GetInquiringStringPrefix(), _T("http://www.crweber.com"));
    EXPECT_STREQ(gl_CrweberIndexWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_TRUE(gl_CrweberIndexWebData.IsCreateOnce());
    EXPECT_FALSE(gl_CrweberIndexWebData.IsNeedProcessingCurrentWebData());
    EXPECT_FALSE(gl_CrweberIndexWebData.IsReadingSucceed());
  }

  TEST(CrweberIndexWebDataTest, TestReportDataError) {
    gl_CrweberIndexWebData.ReportDataError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("crweber.com data error"));
  }

  TEST(CrweberIndexWebDataTest, TestReportCommunicationError) {
    gl_CrweberIndexWebData.ReportCommunicationError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file : http://www.crweber.com"));
  }

  TEST(CrweberIndexWebDataTest, TestGetInquireString) {
    CString str = _T("");
    int i = gl_CrweberIndexWebData.GetInquiringStr(str);
    EXPECT_STREQ(str, _T(""));
    EXPECT_EQ(i, 0);
  }
}