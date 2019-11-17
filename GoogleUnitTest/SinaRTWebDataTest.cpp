#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"SinaRTWebData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(SinaRTWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(gl_SinaRTWebData.IsNeedProcessingCurrentWebData());
    EXPECT_TRUE(gl_SinaRTWebData.IsReadingSucceed());
  }

  TEST(SinaRTWebDataTest, TestReportDataError) {
    gl_SinaRTWebData.ReportDataError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("新浪实时数据有误"));
  }

  TEST(SinaRTWebDataTest, TestReportCommunicationError) {
    gl_SinaRTWebData.ReportCommunicationError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file ：hq.sinajs.cn"));
  }

  TEST(SinaRTWebDataTest, TestStartReadingThread) {
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingWebData());
    EXPECT_FALSE(gl_SinaRTWebData.IsWebDataReceived());
    EXPECT_EQ(gl_SinaRTWebData.GetByteReaded(), 0);
    EXPECT_TRUE(gl_SinaRTWebData.IsReadingSucceed());
    // 线程无法测试，故只测试初始状态。
  }
}