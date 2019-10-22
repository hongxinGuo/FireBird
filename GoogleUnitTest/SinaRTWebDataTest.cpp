#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"SinaRTWebData.h"

using namespace testing;
using namespace std;

namespace StockAnalysisTest {
  TEST(SinaRTWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
  }

  TEST(SinaRTWebDataTest, TestReportDataError) {
    gl_SinaRTWebData.ReportDataError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("新浪实时数据有误"));
  }

  TEST(SinaRTWebDataTest, TestReportCommunicationError) {
    gl_SinaRTWebData.ReportCommunicationError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file ：hq.sinajs.cn"));
  }
}