#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"TengxunRTWebData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(TengxunRTWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_TengxunRTWebData.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(gl_TengxunRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(gl_TengxunRTWebData.IsNeedProcessingCurrentWebData());
    EXPECT_FALSE(gl_TengxunRTWebData.IsReadingSucceed());
  }

  TEST(TengxunRTWebDataTest, TestReadPrefix) {
    CString str = _T("v_pv_none_match=\"1\";\n");
    gl_TengxunRTWebData.ResetCurrentPos();
    gl_TengxunRTWebData.__TESTSetBuffer(str.GetBuffer(), 21);
    gl_TengxunRTWebData.ReadPrefix();
    EXPECT_EQ(gl_TengxunRTWebData.GetCurrentPosPtr(), gl_TengxunRTWebData.GetBufferAddr() + 21);
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