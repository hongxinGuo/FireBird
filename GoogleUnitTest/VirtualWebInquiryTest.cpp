#include"stdafx.h"
#include"afxinet.h"

#include"pch.h"

#include"globedef.h"

#include"VirtualWebInquiry.h"
CVirtualWebInquiry gl_VirtualWebInquiry;

namespace StockAnalysisTest {
  class CVirtualWebInquiryTest : public ::testing::Test {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_VirtualWebInquiry.SetInquiringString(_T(""));
    }
  public:
  };
  TEST_F(CVirtualWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringString(), _T(""));
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringStringPrefix(), _T(""));
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_EQ(gl_VirtualWebInquiry.GetByteReaded(), 0);
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPos(), 0);
    EXPECT_EQ(gl_VirtualWebInquiry.GetBufferAddr(), gl_VirtualWebInquiry.GetCurrentPosPtr());
    EXPECT_FALSE(gl_VirtualWebInquiry.IsReadingWebData());
    EXPECT_FALSE(gl_VirtualWebInquiry.IsReportStatus());
  }

  TEST_F(CVirtualWebInquiryTest, TestTransferWebDataToQueueData) {
    CString str = _T("abcdefghijklmnop");
    gl_VirtualWebInquiry.__TESTSetBuffer(str);
    CWebDataReceivedPtr p = gl_VirtualWebInquiry.TransferWebDataToQueueData();
    EXPECT_EQ(p->GetBufferLength(), str.GetLength());
    char* pChar = p->GetBufferAddr();
    CString strCompare = pChar;
    EXPECT_STREQ(strCompare, str);

    str = _T("");
    gl_VirtualWebInquiry.__TESTSetBuffer(str); // 清除掉之前的设置。
  }

  TEST_F(CVirtualWebInquiryTest, TestReportStatus) {
    EXPECT_TRUE(gl_VirtualWebInquiry.ReportStatus(0));
  }

  TEST_F(CVirtualWebInquiryTest, TestCreateTotalInquiringString) {
    CString str = _T("abcdef");
    gl_VirtualWebInquiry.CreateTotalInquiringString(str);
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringString(), str);
  }

  TEST_F(CVirtualWebInquiryTest, TestInquireNextWebData) {
    //EXPECT_NO_FATAL_FAILURE(gl_VirtualWebInquiry.InquireNextWebData());
  }

  TEST_F(CVirtualWebInquiryTest, TestIsReadingWebData) {
    EXPECT_FALSE(gl_VirtualWebInquiry.IsReadingWebData());
    gl_VirtualWebInquiry.SetReadingWebData(true);
    EXPECT_TRUE(gl_VirtualWebInquiry.IsReadingWebData());
    gl_VirtualWebInquiry.SetReadingWebData(false);
    EXPECT_FALSE(gl_VirtualWebInquiry.IsReadingWebData());
  }

  TEST_F(CVirtualWebInquiryTest, TestGetByteReaded) {
    EXPECT_EQ(gl_VirtualWebInquiry.GetByteReaded(), 0);
    gl_VirtualWebInquiry.SetByteReaded(10000);
    EXPECT_EQ(gl_VirtualWebInquiry.GetByteReaded(), 10000);
    gl_VirtualWebInquiry.AddByteReaded(10000);
    EXPECT_EQ(gl_VirtualWebInquiry.GetByteReaded(), 20000);
  }

  TEST_F(CVirtualWebInquiryTest, TestGetInquiringStr) {
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringString(), _T(""));
    gl_VirtualWebInquiry.SetInquiringString(_T("abcdefg"));
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringString(), _T("abcdefg"));
    gl_VirtualWebInquiry.AppendInquiringString(_T("hijk"));
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringString(), _T("abcdefghijk"));
    gl_VirtualWebInquiry.CreateTotalInquiringString(_T("dcba"));
    EXPECT_STREQ(gl_VirtualWebInquiry.GetInquiringString(), _T("dcba"));
  }

  TEST_F(CVirtualWebInquiryTest, TestIncreaseCurentPos) {
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPos(), 0);
    gl_VirtualWebInquiry.IncreaseCurrentPos();
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPosPtr(), gl_VirtualWebInquiry.GetBufferAddr() + 1);
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPos(), 1);
    gl_VirtualWebInquiry.IncreaseCurrentPos(100);
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPos(), 101);
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPosPtr(), gl_VirtualWebInquiry.GetBufferAddr() + 101);
    gl_VirtualWebInquiry.ResetCurrentPos();
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPos(), 0);
    EXPECT_EQ(gl_VirtualWebInquiry.GetCurrentPosPtr(), gl_VirtualWebInquiry.GetBufferAddr());
  }
}