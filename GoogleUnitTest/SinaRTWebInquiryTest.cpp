#include"pch.h"

#include"globedef.h"

#include"MockSinaRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace testing;

namespace StockAnalysisTest {
  class CSinaRTWebInquiryTest : public ::testing::Test {
  protected:
    static void SetUpTestSuite(void) {
      gl_pChinaStockMarket->SetSinaStockRTDataInquiringIndex(0);
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStockMarket->SetSinaStockRTDataInquiringIndex(0);
    }
  public:
    CMockSinaRTWebInquiry m_SinaRTWebInquiry; // ����ʵʱ���ݲɼ�
  };

  TEST_F(CSinaRTWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(m_SinaRTWebInquiry.IsReportStatus());
    EXPECT_EQ(m_SinaRTWebInquiry.GetInquiringNumber(), 850) << _T("����Ĭ��ֵ");
  }

  TEST_F(CSinaRTWebInquiryTest, TestStartReadingThread) {
    EXPECT_FALSE(m_SinaRTWebInquiry.IsReadingWebData());
    EXPECT_EQ(m_SinaRTWebInquiry.GetByteReaded(), 0);
  }

  TEST_F(CSinaRTWebInquiryTest, TestGetWebData) {
    m_SinaRTWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_SinaRTWebInquiry.GetWebData());
    m_SinaRTWebInquiry.SetReadingWebData(false);
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(m_SinaRTWebInquiry, StartReadingThread)
      .Times(1);
    m_SinaRTWebInquiry.GetWebData();
    EXPECT_TRUE(m_SinaRTWebInquiry.IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");

    gl_pChinaStockMarket->SetSinaStockRTDataInquiringIndexFromTotalStockSet(0);
  }

  TEST_F(CSinaRTWebInquiryTest, TestPrepareInquiringStr) {
    EXPECT_EQ(gl_pChinaStockMarket->GetSinaStockRTDataInquiringIndex(), 0);
    EXPECT_EQ(gl_pChinaStockMarket->GetSinaStockRTDataInquiringIndexFromTotalStockSet(), 0);
    gl_pChinaStockMarket->SetCheckActiveStock(true);
    EXPECT_TRUE(m_SinaRTWebInquiry.PrepareNextInquiringStr());
    CString str = m_SinaRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(33), _T("http://hq.sinajs.cn/list=sh000000")) << "ȫ�ֹ�Ʊ����ʼ����Ϊsh000000";
    gl_pChinaStockMarket->SetSinaStockRTDataInquiringIndexFromTotalStockSet(0); // �ָ�ԭ״

    gl_pChinaStockMarket->SetCheckActiveStock(false);
    EXPECT_TRUE(m_SinaRTWebInquiry.PrepareNextInquiringStr());
    str = m_SinaRTWebInquiry.GetInquiringString();
    EXPECT_STREQ(str.Left(33), _T("http://hq.sinajs.cn/list=sh000001")) << "Ŀǰ��Ծ��Ʊ����ʼ����Ϊsh000001";

    gl_pChinaStockMarket->SetCheckActiveStock(true);
  }

  TEST_F(CSinaRTWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_SinaRTWebInquiry.ReportStatus(1));
  }
}