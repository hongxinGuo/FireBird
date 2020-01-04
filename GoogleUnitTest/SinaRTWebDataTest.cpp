#include"stdafx.h"
#include"pch.h"

#include"Market.h"

#include"SinaWebRTData.h"

using namespace std;
#include<atomic>

namespace StockAnalysisTest {
  TEST(SinaWebRTDataTest, TestInitialize) {
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringStringPrefix(), _T("http://hq.sinajs.cn/list="));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringStringSuffix(), _T(""));
  }

  TEST(SinaWebRTDataTest, TestStartReadingThread) {
    EXPECT_FALSE(gl_SinaWebRTData.IsReadingWebData());
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 0);
    // �߳��޷����ԣ���ֻ���Գ�ʼ״̬��
  }

  TEST(SinaWebRTDataTest, TestGetInquiringStr) {
    gl_ChinaStockMarket.ResetSinaRTDataInquiringIndex();
    CString str = gl_SinaWebRTData.GetNextInquiringStr(900, false);
    CString str2 = str.Left(9);
    EXPECT_STREQ(str2, _T("sh600000,"));
  }

  TEST(SinaWebRTDataTest, TestReportStatus) {
    EXPECT_TRUE(gl_SinaWebRTData.ReportStatus(1));
  }
}

// ���ڻ���CWebDataΪ���࣬�޷�ֱ������ʵ�����ʶ�����ķ����⺯���ڴ˲���
namespace StockAnalysisTest {
  TEST(CWebDataTest, TestIsReadingWebData) {
    EXPECT_FALSE(gl_SinaWebRTData.IsReadingWebData());
    gl_SinaWebRTData.SetReadingWebData(true);
    EXPECT_TRUE(gl_SinaWebRTData.IsReadingWebData());
    gl_SinaWebRTData.SetReadingWebData(false);
    EXPECT_FALSE(gl_SinaWebRTData.IsReadingWebData());
  }

  TEST(CWebDataTest, TestGetByteReaded) {
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 0);
    gl_SinaWebRTData.SetByteReaded(10000);
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 10000);
    gl_SinaWebRTData.AddByteReaded(10000);
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 20000);
  }

  TEST(CWebDataTest, TestGetInquiringStr) {
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T(""));
    gl_SinaWebRTData.SetInquiringString(_T("abcdefg"));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T("abcdefg"));
    gl_SinaWebRTData.AppendInquiringString(_T("hijk"));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T("abcdefghijk"));
    gl_SinaWebRTData.CreateTotalInquiringString(_T("dcba"));
    EXPECT_STREQ(gl_SinaWebRTData.GetInquiringString(), _T("http://hq.sinajs.cn/list=dcba"));
  }

  TEST(CWebDataTest, TestTransferWebDataToBuffer) {
    gl_SinaWebRTData.__TESTSetBuffer(_T("abcdefg"));

    vector<char> buffer;
    buffer.resize(8);
    gl_SinaWebRTData.TransferWebDataToBuffer(buffer);
    EXPECT_EQ(buffer.at(0), 'a');
    EXPECT_EQ(buffer.at(1), 'b');
    EXPECT_EQ(buffer.at(2), 'c');
    EXPECT_EQ(buffer.at(3), 'd');
    EXPECT_EQ(buffer.at(4), 'e');
    EXPECT_EQ(buffer.at(5), 'f');
    EXPECT_EQ(buffer.at(6), 'g');
    EXPECT_EQ(buffer.at(7), 0x000);
    EXPECT_EQ(gl_SinaWebRTData.GetByteReaded(), 7);
  }

  TEST(CWebDataTest, TestIncreaseCurentPos) {
    EXPECT_EQ(gl_SinaWebRTData.GetCurrentPos(), 0);
    gl_SinaWebRTData.IncreaseCurrentPos();
    EXPECT_EQ(gl_SinaWebRTData.GetCurrentPos(), 1);
    gl_SinaWebRTData.IncreaseCurrentPos(100);
    EXPECT_EQ(gl_SinaWebRTData.GetCurrentPos(), 101);
  }
}