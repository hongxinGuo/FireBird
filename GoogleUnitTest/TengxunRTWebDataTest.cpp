#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"TengxunRTWebData.h"

using namespace testing;
using namespace std;

namespace StockAnalysisTest {
  TEST(TengxunRTWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_TengxunRTWebData.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(gl_TengxunRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_TRUE(gl_TengxunRTWebData.IsCreateOnce());
    EXPECT_TRUE(gl_TengxunRTWebData.IsNeedProcessingCurrentWebData());
    EXPECT_TRUE(gl_TengxunRTWebData.IsReadingSucceed());
  }

  TEST(TengxunRTWebDataTest, TestReportDataError) {
    gl_TengxunRTWebData.ReportDataError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("腾讯实时数据有误"));
  }

  TEST(TengxunRTWebDataTest, TestReportCommunicationError) {
    gl_TengxunRTWebData.ReportCommunicationError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file ：http://qt.gtimg.cn"));
  }
}