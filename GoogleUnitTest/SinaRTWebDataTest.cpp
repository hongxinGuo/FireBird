#include"stdafx.h"
#include"pch.h"

#include"Market.h"

#include"SinaWebRTData.h"

using namespace std;
#include<atomic>

namespace StockAnalysisTest {
  TEST(SinaWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringStringSuffix(), _T(""));
  }

  TEST(SinaWebRTDataTest, TestStartReadingThread) {
    EXPECT_FALSE(gl_SinaWebRTData.IsReadingWebData());
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 0);
    // 线程无法测试，故只测试初始状态。
  }

  TEST(SinaWebRTDataTest, TestGetInquiringStr) {
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = gl_SinaWebRTData.GetNextInquiringStr(900, false);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T("sh600000,"));
  }

  TEST(SinaWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(gl_SinaWebRTData.ReportStatus(1));
  }
}

// 由于基类CWebData为虚类，无法直接生成实例，故而基类的非虚拟函数在此测试
namespace StockAnalysisTest {
  TEST(CWebDataTest, TestIsReadingWebData) {
    EXPECT_FALSE(gl_SinaWebRTData.IsReadingWebData());
    gl_SinaWebRTData.SetReadingWebData(true);
    EXPECT_TRUE(gl_SinaWebRTData.IsReadingWebData());
    gl_SinaWebRTData.SetReadingWebData(false);
    EXPECT_FALSE(gl_SinaWebRTData.IsReadingWebData());
  }

  TEST(CWebDataTest, TestGetByteReaded) {
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 0);
    gl_SinaWebRTData.SetByteReaded(10000);
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 10000);
    gl_SinaWebRTData.AddByteReaded(10000);
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 20000);
  }

  TEST(CWebDataTest, TestGetInquiringStr) {
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T(""));
    gl_SinaWebRTData.SetInquiringString(_T("abcdefg"));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T("abcdefg"));
    gl_SinaWebRTData.AppendInquiringString(_T("hijk"));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T("abcdefghijk"));
    gl_SinaWebRTData.CreateTotalInquiringString(_T("dcba"));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T("http://hq.sinajs.cn/list=dcba"));
  }

  TEST(CWebDataTest, TestIncreaseCurentPos) {
    EXPECT_EQ(gl_SinaWebRTData.GetCurrentPos(), 0);
    gl_SinaWebRTData.IncreaseCurrentPos();
    EXPECT_EQ(gl_SinaWebRTData.GetCurrentPos(), 1);
    gl_SinaWebRTData.IncreaseCurrentPos(100);
    EXPECT_EQ(gl_SinaWebRTData.GetCurrentPos(), 101);
  }
}