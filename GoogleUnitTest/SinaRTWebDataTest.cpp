#include"stdafx.h"
#include"pch.h"

#include"Market.h"

#include"SinaWebRTData.h"

using namespace std;
#include<atomic>

static CSinaRTWebData m_SinaRTWebData; // 新浪实时数据采集

namespace StockAnalysisTest {
  TEST(SinaWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
  }

  TEST(SinaWebRTDataTest, TestStartReadingThread) {
    EXPECT_FALSE(m_SinaRTWebData.IsReadingWebData());
    EXPECT_EQ(m_SinaRTWebData.GetByteReaded(), 0);
    // 线程无法测试，故只测试初始状态。
  }

  TEST(SinaWebRTDataTest, TestGetInquiringStr) {
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = m_SinaRTWebData.GetNextInquiringStr(900, false);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T("sh600000,"));
  }

  TEST(SinaWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(m_SinaRTWebData.ReportStatus(1));
  }
}

// 由于基类CWebData为虚类，无法直接生成实例，故而基类的非虚拟函数在此测试
namespace StockAnalysisTest {
  TEST(CWebDataTest, TestIsReadingWebData) {
    EXPECT_FALSE(m_SinaRTWebData.IsReadingWebData());
    m_SinaRTWebData.SetReadingWebData(true);
    EXPECT_TRUE(m_SinaRTWebData.IsReadingWebData());
    m_SinaRTWebData.SetReadingWebData(false);
    EXPECT_FALSE(m_SinaRTWebData.IsReadingWebData());
  }

  TEST(CWebDataTest, TestGetByteReaded) {
    EXPECT_EQ(m_SinaRTWebData.GetByteReaded(), 0);
    m_SinaRTWebData.SetByteReaded(10000);
    EXPECT_EQ(m_SinaRTWebData.GetByteReaded(), 10000);
    m_SinaRTWebData.AddByteReaded(10000);
    EXPECT_EQ(m_SinaRTWebData.GetByteReaded(), 20000);
  }

  TEST(CWebDataTest, TestGetInquiringStr) {
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringString(), _T(""));
    m_SinaRTWebData.SetInquiringString(_T("abcdefg"));
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringString(), _T("abcdefg"));
    m_SinaRTWebData.AppendInquiringString(_T("hijk"));
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringString(), _T("abcdefghijk"));
    m_SinaRTWebData.CreateTotalInquiringString(_T("dcba"));
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringString(), _T("http://hq.sinajs.cn/list=dcba"));
  }

  TEST(CWebDataTest, TestIncreaseCurentPos) {
    EXPECT_EQ(m_SinaRTWebData.GetCurrentPos(), 0);
    m_SinaRTWebData.IncreaseCurrentPos();
    EXPECT_EQ(m_SinaRTWebData.GetCurrentPosPtr(), m_SinaRTWebData.GetBufferAddr() + 1);
    EXPECT_EQ(m_SinaRTWebData.GetCurrentPos(), 1);
    m_SinaRTWebData.IncreaseCurrentPos(100);
    EXPECT_EQ(m_SinaRTWebData.GetCurrentPos(), 101);
    EXPECT_EQ(m_SinaRTWebData.GetCurrentPosPtr(), m_SinaRTWebData.GetBufferAddr() + 101);
  }
}