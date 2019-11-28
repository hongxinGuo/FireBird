#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"SinaRTWebData.h"

using namespace std;
#include<atomic>

namespace StockAnalysisTest {
  TEST(SinaRTWebDataTest, TestInitialize) {
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(gl_SinaRTWebData.IsNeedProcessingCurrentWebData());
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingSucceed());
  }

  TEST(SinaRTWebDataTest, TestReportDataError) {
    if (gl_SinaRTWebData.IsReportStatus()) {
      gl_SinaRTWebData.ReportDataError();
      EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("新浪实时数据有误"));
    }
  }

  TEST(SinaRTWebDataTest, TestReportCommunicationError) {
    if (gl_NeteaseDayLineWebData.IsReportStatus()) {
      gl_SinaRTWebData.ReportCommunicationError();
      EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Error reading http file ：hq.sinajs.cn"));
    }
  }

  TEST(SinaRTWebDataTest, TestStartReadingThread) {
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingWebData());
    EXPECT_FALSE(gl_SinaRTWebData.IsWebDataReceived());
    EXPECT_EQ(gl_SinaRTWebData.GetByteReaded(), 0);
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingSucceed());
    // 线程无法测试，故只测试初始状态。
  }
}

// 由于基类CWebData为虚类，无法直接生成实例，故而基类的非虚拟函数在此测试
namespace StockAnalysis {
  TEST(CWebDataTest, TestIsReadingSucceed) {
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingSucceed());
    gl_SinaRTWebData.SetReadingSucceed(true);
    EXPECT_TRUE(gl_SinaRTWebData.IsReadingSucceed());
    gl_SinaRTWebData.SetReadingSucceed(false);
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingSucceed());
  }

  TEST(CWebDataTest, TestIsWebDataReceived) {
    EXPECT_FALSE(gl_SinaRTWebData.IsWebDataReceived());
    gl_SinaRTWebData.SetWebDataReceived(true);
    EXPECT_TRUE(gl_SinaRTWebData.IsWebDataReceived());
    gl_SinaRTWebData.SetWebDataReceived(false);
    EXPECT_FALSE(gl_SinaRTWebData.IsWebDataReceived());
  }

  TEST(CWebDataTest, TestIsReadingWebData) {
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingWebData());
    gl_SinaRTWebData.SetReadingWebData(true);
    EXPECT_TRUE(gl_SinaRTWebData.IsReadingWebData());
    gl_SinaRTWebData.SetReadingWebData(false);
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingWebData());
  }

  TEST(CWebDataTest, TestGetByteReaded) {
    EXPECT_EQ(gl_SinaRTWebData.GetByteReaded(), 0);
    gl_SinaRTWebData.SetByteReaded(10000);
    EXPECT_EQ(gl_SinaRTWebData.GetByteReaded(), 10000);
    gl_SinaRTWebData.AddByteReaded(10000);
    EXPECT_EQ(gl_SinaRTWebData.GetByteReaded(), 20000);
  }

  TEST(CWebDataTest, TestGetInquiringStr) {
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringString(), _T(""));
    gl_SinaRTWebData.SetInquiringString(_T("abcdefg"));
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringString(), _T("abcdefg"));
    gl_SinaRTWebData.AppendInquiringString(_T("hijk"));
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringString(), _T("abcdefghijk"));
    gl_SinaRTWebData.CreateTotalInquiringString(_T("dcba"));
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringString(), _T("http://hq.sinajs.cn/list=dcba"));
  }

  TEST(CWebDataTest, TestIncreaseCurentPos) {
    char* pAddr = gl_SinaRTWebData.GetCurrentPosPtr();

    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPos(), 0);
    gl_SinaRTWebData.IncreaseCurrentPos();
    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPos(), 1);
    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPosPtr(), pAddr + 1);
    gl_SinaRTWebData.IncreaseCurrentPos(100);
    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPos(), 101);
    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPosPtr(), pAddr + 101);
  }
}