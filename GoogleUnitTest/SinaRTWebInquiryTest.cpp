#include"stdafx.h"
#include"pch.h"

#include"ChinaMarket.h"

#include"SinaRTWebInquiry.h"

using namespace std;
#include<atomic>

static CSinaRTWebInquiry m_SinaRTWebData; // ����ʵʱ���ݲɼ�

namespace StockAnalysisTest {
  TEST(SinaWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(m_SinaRTWebData.GetInquiringStringSuffix(), _T(""));
    EXPECT_FALSE(m_SinaRTWebData.IsReportStatus());
  }

  TEST(SinaWebRTDataTest, TestStartReadingThread) {
    EXPECT_FALSE(m_SinaRTWebData.IsReadingWebData());
    EXPECT_EQ(m_SinaRTWebData.GetByteReaded(), 0);
    // �߳��޷����ԣ���ֻ���Գ�ʼ״̬��
  }

  TEST(SinaWebRTDataTest, TestGetInquiringStr) {
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = m_SinaRTWebData.GetNextInquiringStr(900, false);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T("sh600000,"));
  }

  TEST(SinaWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(m_SinaRTWebData.ReportStatus(1));
  }
}