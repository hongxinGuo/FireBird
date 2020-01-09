#include"stdafx.h"
#include"pch.h"

#include"Market.h"

#include"SinaWebRTData.h"

using namespace std;
#include<atomic>

namespace StockAnalysisTest {
  TEST(SinaWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_WebDataInquirer.m_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(gl_WebDataInquirer.m_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
  }

  TEST(SinaWebRTDataTest, TestStartReadingThread) {
    EXPECT_FALSE(gl_WebDataInquirer.m_SinaRTWebData.IsReadingWebData());
    EXPECT_EQ(gl_WebDataInquirer.m_SinaRTWebData.GetByteReaded(), 0);
    // 线程无法测试，故只测试初始状态。
  }

  TEST(SinaWebRTDataTest, TestGetInquiringStr) {
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = gl_WebDataInquirer.m_SinaRTWebData.GetNextInquiringStr(900, false);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T("sh600000,"));
  }

  TEST(SinaWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(gl_WebDataInquirer.m_SinaRTWebData.ReportStatus(1));
  }
}

// 由于基类CWebData为虚类，无法直接生成实例，故而基类的非虚拟函数在此测试
namespace StockAnalysisTest {
  TEST(CWebDataTest, TestIsReadingWebData) {
    EXPECT_FALSE(gl_WebDataInquirer.m_SinaRTWebData.IsReadingWebData());
    gl_WebDataInquirer.m_SinaRTWebData.SetReadingWebData(true);
    EXPECT_TRUE(gl_WebDataInquirer.m_SinaRTWebData.IsReadingWebData());
    gl_WebDataInquirer.m_SinaRTWebData.SetReadingWebData(false);
    EXPECT_FALSE(gl_WebDataInquirer.m_SinaRTWebData.IsReadingWebData());
  }

  TEST(CWebDataTest, TestGetByteReaded) {
    EXPECT_EQ(gl_WebDataInquirer.m_SinaRTWebData.GetByteReaded(), 0);
    gl_WebDataInquirer.m_SinaRTWebData.SetByteReaded(10000);
    EXPECT_EQ(gl_WebDataInquirer.m_SinaRTWebData.GetByteReaded(), 10000);
    gl_WebDataInquirer.m_SinaRTWebData.AddByteReaded(10000);
    EXPECT_EQ(gl_WebDataInquirer.m_SinaRTWebData.GetByteReaded(), 20000);
  }

  TEST(CWebDataTest, TestGetInquiringStr) {
    EXPECT_STREQ(gl_WebDataInquirer.m_SinaRTWebData.GetInquiringString(), _T(""));
    gl_WebDataInquirer.m_SinaRTWebData.SetInquiringString(_T("abcdefg"));
    EXPECT_STREQ(gl_WebDataInquirer.m_SinaRTWebData.GetInquiringString(), _T("abcdefg"));
    gl_WebDataInquirer.m_SinaRTWebData.AppendInquiringString(_T("hijk"));
    EXPECT_STREQ(gl_WebDataInquirer.m_SinaRTWebData.GetInquiringString(), _T("abcdefghijk"));
    gl_WebDataInquirer.m_SinaRTWebData.CreateTotalInquiringString(_T("dcba"));
    EXPECT_STREQ(gl_WebDataInquirer.m_SinaRTWebData.GetInquiringString(), _T("http://hq.sinajs.cn/list=dcba"));
  }
}