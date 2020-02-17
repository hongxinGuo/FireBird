#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"

#include"SinaRTWebInquiry.h"

using namespace std;
#include<atomic>

static CSinaRTWebInquiry m_SinaRTWebData; // ����ʵʱ���ݲɼ�

namespace StockAnalysisTest {
  class CSinaWebRTDataTest : public ::testing::Test {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    }
  public:
  };

  TEST_F(CSinaWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(m_SinaRTWebData.IsReportStatus());
    EXPECT_EQ(m_SinaRTWebData.GetInquiringNumber(), 850) << _T("����Ĭ��ֵ");
  }

  TEST_F(CSinaWebRTDataTest, TestStartReadingThread) {
    EXPECT_FALSE(m_SinaRTWebData.IsReadingWebData());
    EXPECT_EQ(m_SinaRTWebData.GetByteReaded(), 0);
    // �߳��޷����ԣ���ֻ���Գ�ʼ״̬��
  }

  TEST_F(CSinaWebRTDataTest, TestGetNextInquiringMIddleStr) {
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = m_SinaRTWebData.GetNextInquiringMiddleStr(900, false);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T("sh600000,"));
  }

  TEST_F(CSinaWebRTDataTest, TestPrepareInquiringStr) {
    gl_ChinaStockMarket.SetSystemReady(false);
    EXPECT_TRUE(m_SinaRTWebData.PrepareNextInquiringStr());
    CString str = m_SinaRTWebData.GetInquiringString();
    EXPECT_STREQ(str.Left(33), _T("http://hq.sinajs.cn/list=sh600000"));
    EXPECT_STREQ(str.Right(8), _T("sh600849")) << _T("ÿ������850����Ʊ");
  }

  TEST_F(CSinaWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(m_SinaRTWebData.ReportStatus(1));
  }
}