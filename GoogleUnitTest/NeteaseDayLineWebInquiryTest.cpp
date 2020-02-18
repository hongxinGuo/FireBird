#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"

#include"NeteaseDayLineWebInquiry.h"

using namespace std;

static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebData; // 网易日线历史数据

namespace StockAnalysisTest {
  class CNeteaseWebDayLineDataTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_ChinaStockMarket.CalculateTime();
      gl_ChinaStockMarket.ResetNeteaseDayLineDataInquiringIndex();
      m_NeteaseDayLineWebData.ResetDownLoadingStockCode();
      EXPECT_TRUE(gl_ChinaStockMarket.IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ChinaStockMarket.SetResetMarket(true);
      gl_ChinaStockMarket.ResetNeteaseDayLineDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      gl_ChinaStockMarket.SetSystemReady(false);
      gl_ChinaStockMarket.SetCurrentStockChanged(false);
      m_NeteaseDayLineWebData.ResetDownLoadingStockCode();
    }
  };

  TEST_F(CNeteaseWebDayLineDataTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetConnection(), _T("NeteaseDayLine"));
  }

  TEST_F(CNeteaseWebDayLineDataTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseDayLineWebData.ReportStatus(1));
  }

  TEST_F(CNeteaseWebDayLineDataTest, TestSetDownLoadingStockCode) {
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T(""));
    m_NeteaseDayLineWebData.SetDownLoadingStockCode(_T("1000001"));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T("sz000001"));
    m_NeteaseDayLineWebData.SetDownLoadingStockCode(_T("0600001"));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T("sh600001"));
    m_NeteaseDayLineWebData.SetDownLoadingStockCode(_T("2600001"));
    EXPECT_STREQ(m_NeteaseDayLineWebData.GetDownLoadingStockCode(), _T("2600001"));
  }

  TEST_F(CNeteaseWebDayLineDataTest, TestPrepareNextInquiringStr) {
    CString str;
    gl_ChinaStockMarket.SetSystemReady(true);
    for (int i = 0; i < 4; i++) {
      if (m_NeteaseDayLineWebData.PrepareNextInquiringStr()) {
        str = m_NeteaseDayLineWebData.GetInquiringString();
        EXPECT_STREQ(str.Left(54), _T("http://quotes.money.163.com/service/chddata.html?code="));
      }
      else EXPECT_EQ(str.GetLength(), 0);
    }
    gl_ChinaStockMarket.SetSystemReady(false);
  }
}