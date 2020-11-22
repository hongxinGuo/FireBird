#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"MockNeteaseRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace testing;

namespace StockAnalysisTest {
  class CNeteaseRTWebInquiryTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pChinaStakeMarket->CalculateTime();
      gl_pChinaStakeMarket->ResetNeteaseRTDataInquiringIndex();

      EXPECT_TRUE(gl_pChinaStakeMarket->IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStakeMarket->SetSystemReady(false);
      gl_pChinaStakeMarket->ResetNeteaseRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
    CMockNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
  };

  TEST_F(CNeteaseRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringPrefix(), _T("http://api.money.126.net/data/feed/"));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_STREQ(m_NeteaseRTWebInquiry.GetConnection(), _T("NeteaseRT"));
    EXPECT_EQ(m_NeteaseRTWebInquiry.GetInquiringNumber(), 700) << _T("����Ĭ��ֵ");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetWebData) {
    m_NeteaseRTWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_NeteaseRTWebInquiry.GetWebData());
    m_NeteaseRTWebInquiry.SetReadingWebData(false);
    gl_pChinaStakeMarket->SetSystemReady(true);
    EXPECT_CALL(m_NeteaseRTWebInquiry, StartReadingThread)
      .Times(1);
    m_NeteaseRTWebInquiry.GetWebData();
    EXPECT_TRUE(m_NeteaseRTWebInquiry.IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_NeteaseRTWebInquiry.ReportStatus(1));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestGetNextInquiryMiddleStr) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    CString str = m_NeteaseRTWebInquiry.GetNextInquiringMiddleStr(1, false); // �õ���һ����Ʊ�ַ���
    EXPECT_STREQ(str, _T("0600000"));
  }

  TEST_F(CNeteaseRTWebInquiryTest, TestPrepareNextInquiringStr) {
    gl_pChinaStakeMarket->SetSystemReady(true);
    EXPECT_TRUE(m_NeteaseRTWebInquiry.PrepareNextInquiringStr());
    CString str = m_NeteaseRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
  }
}