#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"TengxunWebRTData.h"

using namespace std;

namespace StockAnalysisTest {
  TEST(TengxunWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_TengxunWebRTData.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
    EXPECT_STREQ(gl_TengxunWebRTData.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(gl_TengxunWebRTData.IsNeedProcessingCurrentWebData());
    EXPECT_FALSE(gl_TengxunWebRTData.IsReadingSucceed());
  }

  TEST(TengxunWebRTDataTest, TestReadPrefix) {
    CString str = _T("v_pv_none_match=\"1\";\n");
    gl_TengxunWebRTData.ResetCurrentPos();
    gl_TengxunWebRTData.__TESTSetBuffer(str.GetBuffer(), 21);
    gl_TengxunWebRTData.ReadPrefix();
    EXPECT_EQ(gl_TengxunWebRTData.GetCurrentPosPtr(), gl_TengxunWebRTData.GetBufferAddr() + 21);
  }

  TEST(TengxunWebRTDataTest, TestReportDataError) {
    gl_TengxunWebRTData.ReportDataError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("腾讯实时数据有误"));
  }

  TEST(TengxunWebRTDataTest, TestReportCommunicationError) {
    gl_TengxunWebRTData.ReportCommunicationError();
    EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file ：http://qt.gtimg.cn"));
  }
}