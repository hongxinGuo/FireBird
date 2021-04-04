#include"pch.h"

#include"globedef.h"

#include"MockNeteaseDayLineWebInquiry.h"

using namespace std;
using namespace testing;

namespace StockAnalysisTest {
  class CNeteaseDayLineWebInquiryTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    }

    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pChinaMarket->CalculateTime();
      gl_pChinaMarket->SetNeteaseDayLineDataInquiringIndex(0);
      m_NeteaseDayLineWebInquiry.ResetDownLoadingStockCode();
      EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
      gl_pChinaMarket->SetResetMarket(true);
      gl_pChinaMarket->SetNeteaseDayLineDataInquiringIndex(0);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      gl_pChinaMarket->SetSystemReady(false);
      gl_pChinaMarket->SetCurrentStockChanged(false);
      m_NeteaseDayLineWebInquiry.ResetDownLoadingStockCode();
      for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaMarket->GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
      }
      EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    }
    CMockNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据
  };

  TEST_F(CNeteaseDayLineWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseDayLineWebInquiry.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(m_NeteaseDayLineWebInquiry.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
    EXPECT_STREQ(m_NeteaseDayLineWebInquiry.GetConnectionName(), _T("NeteaseDayLine"));
  }

  TEST_F(CNeteaseDayLineWebInquiryTest, TestGetWebData) {
    EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    m_NeteaseDayLineWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_NeteaseDayLineWebInquiry.GetWebData());
    m_NeteaseDayLineWebInquiry.SetReadingWebData(false);
    gl_pChinaMarket->SetSystemReady(true);
    EXPECT_CALL(m_NeteaseDayLineWebInquiry, StartReadingThread)
      .Times(1);
    m_NeteaseDayLineWebInquiry.GetWebData();
    EXPECT_TRUE(m_NeteaseDayLineWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CNeteaseDayLineWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseDayLineWebInquiry.ReportStatus(1));
  }

  TEST_F(CNeteaseDayLineWebInquiryTest, TestSetDownLoadingStockCode) {
    EXPECT_STREQ(m_NeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T(""));
    m_NeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("1000001"));
    EXPECT_STREQ(m_NeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T("1000001"));
    m_NeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("0600001"));
    EXPECT_STREQ(m_NeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T("0600001"));
    m_NeteaseDayLineWebInquiry.SetDownLoadingStockCode(_T("2600001"));
    EXPECT_STREQ(m_NeteaseDayLineWebInquiry.GetDownLoadingStockCode(), _T("2600001"));
  }

  TEST_F(CNeteaseDayLineWebInquiryTest, TestPrepareNextInquiringStr) {
    EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
    CString str;
    gl_pChinaMarket->SetSystemReady(true);
    for (int i = 0; i < 4; i++) {
      if (m_NeteaseDayLineWebInquiry.PrepareNextInquiringStr()) {
        str = m_NeteaseDayLineWebInquiry.GetInquiringString();
        EXPECT_STREQ(str.Left(54), _T("http://quotes.money.163.com/service/chddata.html?code="));
      }
      else EXPECT_EQ(str.GetLength(), 0);
    }
    gl_pChinaMarket->SetSystemReady(false);
  }
}