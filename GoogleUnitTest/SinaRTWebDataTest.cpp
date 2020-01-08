#include"stdafx.h"
#include"pch.h"

#include"Market.h"

#include"SinaWebRTData.h"

using namespace std;
#include<atomic>

namespace StockAnalysisTest {
  TEST(SinaWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(gl_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
  }

  TEST(SinaWebRTDataTest, TestStartReadingThread) {
    EXPECT_FALSE(gl_SinaRTWebData.IsReadingWebData());
    EXPECT_EQ(gl_SinaRTWebData.GetByteReaded(), 0);
    // 线程无法测试，故只测试初始状态。
  }

  TEST(SinaWebRTDataTest, TestGetInquiringStr) {
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = gl_SinaRTWebData.GetNextInquiringStr(900, false);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T("sh600000,"));
  }

  TEST(SinaWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(gl_SinaRTWebData.ReportStatus(1));
  }
}

// 由于基类CWebData为虚类，无法直接生成实例，故而基类的非虚拟函数在此测试
namespace StockAnalysisTest {
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

  TEST(CWebDataTest, TestTransferWebDataToBuffer) {
    gl_SinaRTWebData.__TESTSetBuffer(_T("abcdefg"));

    vector<char> buffer;
    buffer.resize(8);
    gl_SinaRTWebData.TransferWebDataToBuffer(buffer);
    EXPECT_EQ(buffer.at(0), 'a');
    EXPECT_EQ(buffer.at(1), 'b');
    EXPECT_EQ(buffer.at(2), 'c');
    EXPECT_EQ(buffer.at(3), 'd');
    EXPECT_EQ(buffer.at(4), 'e');
    EXPECT_EQ(buffer.at(5), 'f');
    EXPECT_EQ(buffer.at(6), 'g');
    EXPECT_EQ(buffer.at(7), 0x000);
    EXPECT_EQ(gl_SinaRTWebData.GetByteReaded(), 7);
    gl_SinaRTWebData.__TESTSetBuffer(_T(""));
    gl_SinaRTWebData.TransferWebDataToBuffer(buffer);
    EXPECT_EQ(buffer.at(0), 0x000);
  }

  TEST(CWebDataTest, TestIncreaseCurentPos) {
    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPos(), 0);
    gl_SinaRTWebData.IncreaseCurrentPos();
    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPos(), 1);
    gl_SinaRTWebData.IncreaseCurrentPos(100);
    EXPECT_EQ(gl_SinaRTWebData.GetCurrentPos(), 101);
  }
}