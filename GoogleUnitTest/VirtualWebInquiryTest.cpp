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
    EXPECT_FALSE(m_VirtualWebInquiry.ReadWebData()) << "û����ȷ����ַ";
    EXPECT_TRUE(m_VirtualWebInquiry.IsWebError());
    EXPECT_FALSE(m_VirtualWebInquiry.IsReadingWebData());
  }
  */

  TEST_F(CVirtualWebInquiryTest, TestReadWebData3) {
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(8)
      .WillOnce(Return(0)) //��һ�η���ֵΪ0
      .WillOnce(Return(0))
      .WillOnce(Return(1024)) //�����η���ֵΪ����
      .WillOnce(Return(0))
      .WillOnce(Return(1024))
      .WillRepeatedly(Return(0)); // �������ν�Ϊ�㣬����˳������
    m_VirtualWebInquiry.SetReadingWebData(true);
    m_VirtualWebInquiry.SetInquiringString(_T("http://hq.sinajs.cn/list=sh600000"));
    //m_VirtualWebInquiry.SetInquiringString(_T("http://quotes.money.163.com/service/chddata.html?code=1600000&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE"));
    EXPECT_TRUE(m_VirtualWebInquiry.ReadWebData3(100, 30, 20));
    EXPECT_FALSE(m_VirtualWebInquiry.IsWebError());
    EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << "ֱ�������߳��˳�ʱ�����ô˱�ʶ";
  }

  TEST_F(CVirtualWebInquiryTest, TestReadDataFromWebOnce) {
    long lCurrentByteReaded = 0;
    m_VirtualWebInquiry.SetByteReaded(0);
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(1)
      .WillOnce(Return(0));
    EXPECT_FALSE(m_VirtualWebInquiry.ReadDataFromWebOnce(lCurrentByteReaded));
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 0);
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(1)
      .WillOnce(Return(1024));
    EXPECT_TRUE(m_VirtualWebInquiry.ReadDataFromWebOnce(lCurrentByteReaded));
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 1024);
    EXPECT_CALL(m_VirtualWebInquiry, ReadWebFile())
      .Times(2)
      .WillOnce(Return(1024))
      .WillOnce(Return(128));
    EXPECT_TRUE(m_VirtualWebInquiry.ReadDataFromWebOnce(lCurrentByteReaded));
    EXPECT_TRUE(m_VirtualWebInquiry.ReadDataFromWebOnce(lCurrentByteReaded));
    EXPECT_EQ(m_VirtualWebInquiry.GetByteReaded(), 1024 + 1024 + 128);
  }

  TEST_F(CVirtualWebInquiryTest, TestGetWebData) {
    m_VirtualWebInquiry.SetReadingWebData(true);
    EXPECT_FALSE(m_VirtualWebInquiry.GetWebData()) << _T("Ŀǰֻ���Ե����ڶ�ȡ�������ݵ����.�˻��಻�������GetWebData()����");

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
    EXPECT_TRUE(m_VirtualWebInquiry.IsReadingWebData()) << _T("Ԥ�����õĴ˱�ʶ������Mock��û������֮���ʶ�������������״̬\n");
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
    char buffer[30];
    int i = 0;
    while ((i < 30) && (p->GetData(i) != 0x000)) {
      buffer[i] = p->GetData(i);
      i++;
    }
    buffer[i] = 0x000;
    CString strCompare = buffer;
    EXPECT_STREQ(strCompare, str);

    str = _T("");
    m_VirtualWebInquiry.__TESTSetBuffer(str); // �����֮ǰ�����á�
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
    EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 500) << _T("Ĭ��ֵΪ500");
    m_VirtualWebInquiry.SetInquiringNumber(800);
    EXPECT_EQ(m_VirtualWebInquiry.GetInquiringNumber(), 800);
  }
}