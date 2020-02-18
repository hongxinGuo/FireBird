#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"

#include"NeteaseRTWebInquiry.h"

using namespace std;
#include<atomic>

static CNeteaseRTWebInquiry m_NeteaseRTWebData; // 新浪实时数据采集

namespace StockAnalysisTest {
  class CNeteaseRTWebInquiryTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_ChinaStockMarket.CalculateTime();
      gl_ChinaStockMarket.ResetNeteaseRTDataInquiringIndex();

      EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ChinaStockMarket.SetSystemReady(false);
      gl_ChinaStockMarket.ResetNeteaseRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
  };

  TEST_F(CNeteaseRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseRTWebData.GetInquiringStringPrefix(), _T("http://api.money.126.net/data/feed/"));
    EXPECT_STREQ(m_NeteaseRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_NeteaseRTWebData.GetConnection(), _T("NeteaseRT"));
    EXPECT_EQ(m_NeteaseRTWebData.GetInquiringNumber(), 600) << _T("网易默认值");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseRTWebData.ReportStatus(1));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetNextInquiryMiddleStr) {
    gl_ChinaStockMarket.SetSystemReady(true);
    CString str = m_NeteaseRTWebData.GetNextInquiringMiddleStr(1, false); // 得到第一个股票字符串
    EXPECT_STREQ(str, _T("0600000"));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestPrepareNextInquiringStr) {
    gl_ChinaStockMarket.SetSystemReady(true);
    EXPECT_TRUE(m_NeteaseRTWebData.PrepareNextInquiringStr());
    CString str = m_NeteaseRTWebData.GetInquiringString();
    EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
  }
}