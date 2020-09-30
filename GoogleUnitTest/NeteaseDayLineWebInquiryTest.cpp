#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"MockNeteaseDayLineWebInquiry.h"

using namespace std;
using namespace testing;

namespace StockAnalysisTest {
  class CNeteaseDLWebInquiryTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->ResetNeteaseDLDataInquiringIndex();
      m_NeteaseDLWebInquiry.ResetDownLoadingStockCode();
      EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
      gl_pChinaStockMarket->SetResetMarket(true);
      gl_pChinaStockMarket->ResetNeteaseDLDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      gl_pChinaStockMarket->SetSystemReady(false);
      gl_pChinaStockMarket->SetCurrentStockChanged(false);
      m_NeteaseDLWebInquiry.ResetDownLoadingStockCode();
      for (int i = 0; i < gl_pChinaStockMarket->GetTotalStock(); i++) {
        CChinaStockPtr pStock = gl_pChinaStockMarket->GetStock(i);
        if (!pStock->IsDLNeedUpdate()) pStock->SetDLNeedUpdate(true);
      }
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
    }
    CMockNeteaseDLWebInquiry m_NeteaseDLWebInquiry; // 网易日线历史数据
  };

  TEST_F(CNeteaseDLWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseDLWebInquiry.GetInquiringStringPrefix(), _T("http://quotes.money.163.com/service/chddata.html?code="));
    EXPECT_STREQ(m_NeteaseDLWebInquiry.GetInquiringStringSuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
    EXPECT_STREQ(m_NeteaseDLWebInquiry.GetConnection(), _T("NeteaseDL"));
  }

  TEST_F(CNeteaseDLWebInquiryTest, TestGetWebData) {
    EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
    m_NeteaseDLWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_NeteaseDLWebInquiry.GetWebData());
    m_NeteaseDLWebInquiry.SetReadingWebData(false);
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(m_NeteaseDLWebInquiry, StartReadingThread)
      .Times(1);
    m_NeteaseDLWebInquiry.GetWebData();
    EXPECT_TRUE(m_NeteaseDLWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
  }

  TEST_F(CNeteaseDLWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseDLWebInquiry.ReportStatus(1));
  }

  TEST_F(CNeteaseDLWebInquiryTest, TestSetDownLoadingStockCode) {
    EXPECT_STREQ(m_NeteaseDLWebInquiry.GetDownLoadingStockCode(), _T(""));
    m_NeteaseDLWebInquiry.SetDownLoadingStockCode(_T("1000001"));
    EXPECT_STREQ(m_NeteaseDLWebInquiry.GetDownLoadingStockCode(), _T("sz000001"));
    m_NeteaseDLWebInquiry.SetDownLoadingStockCode(_T("0600001"));
    EXPECT_STREQ(m_NeteaseDLWebInquiry.GetDownLoadingStockCode(), _T("sh600001"));
    m_NeteaseDLWebInquiry.SetDownLoadingStockCode(_T("2600001"));
    EXPECT_STREQ(m_NeteaseDLWebInquiry.GetDownLoadingStockCode(), _T("2600001"));
  }

  TEST_F(CNeteaseDLWebInquiryTest, TestPrepareNextInquiringStr) {
    EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedUpdateNumber(), 12000);
    CString str;
    gl_pChinaStockMarket->SetSystemReady(true);
    for (int i = 0; i < 4; i++) {
      if (m_NeteaseDLWebInquiry.PrepareNextInquiringStr()) {
        str = m_NeteaseDLWebInquiry.GetInquiringString();
        EXPECT_STREQ(str.Left(54), _T("http://quotes.money.163.com/service/chddata.html?code="));
      }
      else EXPECT_EQ(str.GetLength(), 0);
    }
    gl_pChinaStockMarket->SetSystemReady(false);
  }
}