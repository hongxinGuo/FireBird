#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"MockNeteaseRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace Testing;

static CMockNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�

namespace StockAnalysisTest {
  class CNeteaseRTWebInquiryTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();

      EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStockMarket->SetSystemReady(false);
      gl_pChinaStockMarket->ResetNeteaseRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
  };

  TEST_F(CNeteaseRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringPrefix(), _T("http://api.money.126.net/data/feed/"));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetConnection(), _T("NeteaseRT"));
    EXPECT_EQ(m_NeteaseRTWebInquiry.GetInquiringNumber(), 600) << _T("����Ĭ��ֵ");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetWebData) {
    m_NeteaseRTWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_NeteaseRTWebInquiry.GetWebData());
    m_NeteaseRTWebInquiry.SetReadingWebData(false);
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(m_NeteaseRTWebInquiry, StartReadingThread)
      .Times(1);
    m_NeteaseRTWebInquiry.GetWebData();
    EXPECT_TRUE(m_NeteaseRTWebInquiry.IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseRTWebInquiry.ReportStatus(1));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetNextInquiryMiddleStr) {
    gl_pChinaStockMarket->SetSystemReady(true);
    CString str = m_NeteaseRTWebInquiry.GetNextInquiringMiddleStr(1, false); // �õ���һ����Ʊ�ַ���
    EXPECT_STREQ(str, _T("0600000"));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestPrepareNextInquiringStr) {
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_TRUE(m_NeteaseRTWebInquiry.PrepareNextInquiringStr());
    CString str = m_NeteaseRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
  }
}