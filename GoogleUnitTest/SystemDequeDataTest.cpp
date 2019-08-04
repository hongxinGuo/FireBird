//////////////////////////////////////////////////////////////////////////////////////////
//
// ϵͳ���������еĸ��������Բ��õ�ͬ�������Է�ֹ�߳�֮��������ݲ�ͬ�����⣬�����µĲ��Ժ���û�в�������ͬ�����ƣ�
// ֻ�Ǽ򵥵Ĳ������ڲ��߼���
//
//
///////////////////////////////////////////////////////////////////////////////////////////

#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"SystemDequeData.h"

using namespace testing;

namespace StockAnalysisTest {
  
  TEST(SystemDaeuqDataTest, TestInitialize) {
    ASSERT_TRUE(gl_fTestMode);
    long l = gl_systemMessage.GetInformationDequeSize();
    CSystemDequeData systemdeque; // ���ɵڶ���ʵ������һ��Ϊȫ�ֱ�����ϵͳ����ʱ�������ˣ�
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // ϵͳ��������
    for (int i = 0; i < l+1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }
  
  TEST(SystemDequeDataTest, TestGetRTDataDuqueSize) {
    EXPECT_EQ(gl_systemDequeData.GetRTDataDequeSize(), 0);
    CStockRTDataPtr pRTData = make_shared<CStockRTData>();
    pRTData->SetTime(100100100);
    gl_systemDequeData.PushRTData(pRTData);
    EXPECT_EQ(gl_systemDequeData.GetRTDataDequeSize(), 1);
    CStockRTDataPtr p2 = gl_systemDequeData.PopRTData();
    EXPECT_EQ(gl_systemDequeData.GetRTDataDequeSize(), 0);
    EXPECT_EQ(p2->GetTime(), 100100100);
  }
}