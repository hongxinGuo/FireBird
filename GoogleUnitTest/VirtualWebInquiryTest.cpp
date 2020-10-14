#include"stdafx.h"
#include"afxinet.h"

#include"pch.h"

#include"globedef.h"

#include"MockVirtualWebInquiry.h"
using namespace testing;

//#include"VirtualWebInquiry.h"

namespace StockAnalysisTest {
  class CVirtualWebInquiryTest : public ::testing::Test {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
    }

    virtual void TearDown(void) override {
      // clearup
      m_VirtualWebInquiry.SetInquiringString(_T(""));
      m_VirtualWebInquiry.SetReadingWebData(false);
      m_VirtualWebInquiry.SetReadingthreadNumber(0);
    }
  public:
    CMockVirtualWebInquiry m_VirtualWebInquiry;
  };
  TEST_F(CVirtualWebInquiryTest, TestInitialize) {
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringString(), _T(""));
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringPrefix(), _T(""));
    EXPECT_STREQ(m_VirtualWebInquiry.GetInquiringStringSuffix(), _T(""));
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
    EXPECT_FALSE(m_VirtualWebInquiry.IsReportStatus());
  }

  /*
  TEST_F(CVirtualWebInquiryTest, TestReadWebData1) {
    m_VirtualWebInquiry.SetReadingWebData(true);
    m_VirtualWebInquiry.SetInquiringString(_T("Dummy"));
    EXPECT_FALSE(m_VirtualWebInquiry.ReadWebData(100, 20)) << "没有正确的网址";
    EXPECT_TRUE(m_VirtualWebInquiry.IsWebError());
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
  }
  */

  TEST_F(CVirtualWebInquiryTest, TestReadWebData2) {
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(8)
      .WillOnce(Return(0)) //第一次返回值为0
      .WillOnce(Return(0))
      .WillOnce(Return(1024)) //第三次返回值为非零
      .WillOnce(Return(0))
      .WillOnce(Return(1024))
      .WillRepeatedly(Return(0)); // 随后的三次皆为零，函数顺利返回
    m_VirtualWebInquiry.SetReadingWebData(true);
    m_VirtualWebInquiry.SetInquiringString(_T("http://hq.sinajs.cn/list=sh600000"));
    //m_VirtualWebInquiry.SetInquiringString(_T("http://quotes.money.163.com/service/chddata.html?code=1600000&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE"));
    EXPECT_TRUE(m_VirtualWebInquiry.ReadWebData(100, 20));
    EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
  }

  TEST_F(CVirtualWebInquiryTest, TestReadDataFromWebOnce) {
    m_VirtualWebInquiry.SetByteReaded(0);
    m_VirtualWebInquiry.ResetCurrentPos();
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(1)
      .WillOnce(Return(0));
    EXPECT_FALSE(m_VirtualWebInquiry.ReadDataFromWebOnce());
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentReadPos() - m_VirtualWebInquiry.GetBufferAddr(), 0);
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(1)
      .WillOnce(Return(1024));
    EXPECT_TRUE(m_VirtualWebInquiry.ReadDataFromWebOnce());
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 1024);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentReadPos() - m_VirtualWebInquiry.GetBufferAddr(), 1024);
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(2)
      .WillOnce(Return(1024))
      .WillOnce(Return(128));
    EXPECT_TRUE(m_VirtualWebInquiry.ReadDataFromWebOnce());
    EXPECT_TRUE(m_VirtualWebInquiry.ReadDataFromWebOnce());
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 1024 + 1024 + 128);
    EXPECT_EQ(m_VirtualWebInquiry.GetCurrentReadPos() - m_VirtualWebInquiry.GetBufferAddr(), 1024 + 1024 + 128);
  }

  TEST_F(CVirtualWebInquiryTest, TestGetWebData) {
    m_VirtualWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_VirtualWebInquiry.GetWebData()) << _T("目前只测试当正在读取网络数据的情况.此基类不允许调用GetWebData()函数");

    m_VirtualWebInquiry.SetReadingWebData(false);
    EXPECT_CALL(m_VirtualWebInquiry, PrepareNextInquiringStr())
      .WillOnce(Return(false));
    EXPECT_CALL(m_VirtualWebInquiry, StartReadingThread())
      .Times(0);
    EXPECT_FALSE(m_VirtualWebInquiry.GetWebData());

    m_VirtualWebInquiry.SetReadingWebData(false);
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
    EXPECT_CALL(m_VirtualWebInquiry, PrepareNextInquiringStr())
      .WillOnce(Return(true));
    EXPECT_CALL(m_VirtualWebInquiry, StartReadingThread())
      .Times(1);
    EXPECT_TRUE(m_VirtualWebInquiry.GetWebData());
    EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << _T("预先设置的此标识，由于Mock类没有重置之，故而还保持着设置状态\n");
  }

  TEST_F(CVirtualWebInquiryTest, TestStartReadingThread) {
    EXPECT_CALL(m_VirtualWebInquiry, StartReadingThread())
      .Times(1);
    m_VirtualWebInquiry.StartReadingThread();
  }

  TEST_F(CVirtualWebInquiryTest, TestTransferWebDataToQueueData) {
    CString str = _T("abcdefghijklmnop");
    m_VirtualWebInquiry.__TESTSetBuffer(str);
    CWebDataPtr p = m_VirtualWebInquiry.TransferWebDataToQueueData();
    EXPECT_EQ(p->GetBufferLength(), str.GetLength());
    char* pChar = p->__TEST_GetBufferAddr();
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

  TEST_F(CVirtualWebInquiryTest, TestIsWebError) {
    EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
    m_VirtualWebInquiry.SetWebError(true);
    EXPECT_TRUE(m_VirtualWebInquiry.IsWebError());
    m_VirtualWebInquiry.SetWebError(false);
    EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
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