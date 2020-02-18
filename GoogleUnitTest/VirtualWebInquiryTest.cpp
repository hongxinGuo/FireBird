#include"stdafx.h"
#include"afxinet.h"

#include"pch.h"

#include"globedef.h"

#include"VirtualWebInquiry.h"

namespace StockAnalysisTest {
  class CVirtualWebInquiryTest : public ::testing::Test {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
    }

    virtual void TearDown(void) override {
      // clearup
      m_VirtualWebInquiry.SetInquiringString(_T(""));
      m_VirtualWebInquiry.SetReadingWebData(false);
    }
  public:
    CVirtualWebInquiry m_VirtualWebInquiry;
  };
  TEST_F(CVirtualWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T(""));
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringPrefix(), _T(""));
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPos(), 0);
    EXPECT_EQ(m_VirtualWebInquiry.GetBufferAddr(), m_VirtualWebInquiry.GetCurrentPosPtr());
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
    EXPECT_FALSE(m_VirtualWebInquiry.IsReportStatus());
  }

  TEST_F(CVirtualWebInquiryTest, TestGetWebData) {
    m_VirtualWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_VirtualWebInquiry.GetWebData()) << _T("目前只测试当正在读取网络数据的情况.此基类不允许调用GetWebData()函数");
  }

  TEST_F(CVirtualWebInquiryTest, TestTransferWebDataToQueueData) {
    CString str = _T("abcdefghijklmnop");
    m_VirtualWebInquiry.__TESTSetBuffer(str);
    CWebDataPtr p = m_VirtualWebInquiry.TransferWebDataToQueueData();
    EXPECT_EQ(p->GetBufferLength(), str.GetLength());
    char* pChar = p->GetBufferAddr();
    CString strCompare = pChar;
    EXPECT_STREQ(strCompare, str);

    str = _T("");
    m_VirtualWebInquiry.__TESTSetBuffer(str); // 清除掉之前的设置。
  }

  TEST_F(CVirtualWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(m_VirtualWebInquiry.ReportStatus(0));
  }

  TEST_F(CVirtualWebInquiryTest, TestCreateTotalInquiringString) {
    CString str = _T("abcdef");
    m_VirtualWebInquiry.CreateTotalInquiringString(str);
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), str);
  }

  TEST_F(CVirtualWebInquiryTest, TestInquireNextWebData) {
    //EXPECT_NO_FATAL_FAILURE(m_VirtualWebInquiry.InquireNextWebData());
  }

  TEST_F(CVirtualWebInquiryTest, TestIsReadingWebData) {
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
    m_VirtualWebInquiry.SetReadingWebData(true);
    EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData());
    m_VirtualWebInquiry.SetReadingWebData(false);
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
  }

  TEST_F(CVirtualWebInquiryTest, TestGetByteReaded) {
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
    m_VirtualWebInquiry.SetByteReaded(10000);
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 10000);
    m_VirtualWebInquiry.AddByteReaded(10000);
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 20000);
  }

  TEST_F(CVirtualWebInquiryTest, TestGetInquiringString) {
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T(""));
    m_VirtualWebInquiry.SetInquiringString(_T("abcdefg"));
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T("abcdefg"));
    m_VirtualWebInquiry.AppendInquiringString(_T("hijk"));
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T("abcdefghijk"));
    m_VirtualWebInquiry.CreateTotalInquiringString(_T("dcba"));
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T("dcba"));
  }

  TEST_F(CVirtualWebInquiryTest, TestIncreaseCurentPos) {
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPos(), 0);
    m_VirtualWebInquiry.IncreaseCurrentPos();
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPosPtr(), m_VirtualWebInquiry.GetBufferAddr() + 1);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPos(), 1);
    m_VirtualWebInquiry.IncreaseCurrentPos(100);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPos(), 101);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPosPtr(), m_VirtualWebInquiry.GetBufferAddr() + 101);
    m_VirtualWebInquiry.ResetCurrentPos();
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPos(), 0);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentPosPtr(), m_VirtualWebInquiry.GetBufferAddr());
  }

  TEST_F(CVirtualWebInquiryTest, TestGetInquiringNumber) {
    EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 500) << _T("默认值为500");
    m_VirtualWebInquiry.SetInquiringNumber(800);
    EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 800);
  }

  TEST_F(CVirtualWebInquiryTest, TestGetReadingThreadNumber) {
    EXPECT_EQ(m_VirtualWebInquiry.GetReadingThreadNumber(), 0) << _T("初始时读取网络线程数为零");
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebThreadRunning());
    m_VirtualWebInquiry.SetReadingthreadNumber(100);
    EXPECT_EQ(m_VirtualWebInquiry.GetReadingThreadNumber(), 100);
    EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebThreadRunning());
    m_VirtualWebInquiry.SetReadingthreadNumber(0);
  }
}