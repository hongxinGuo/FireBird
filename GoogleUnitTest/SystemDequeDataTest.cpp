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
    ASSERT_FALSE(gl_fNormalMode);
    long l = gl_systemMessage.GetInformationDequeSize();
    CSystemQueueData systemdeque; // ���ɵڶ���ʵ������һ��Ϊȫ�ֱ�����ϵͳ����ʱ�������ˣ�
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // ϵͳ��������
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST(SystemDequeDataTest, TestGetRTDataDuqueSize) {
    EXPECT_EQ(gl_systemDequeData.GetRTDataDequeSize(), 0);
    CStockRTDataPtr pRTData = make_shared<CStockRTData>();
    pRTData->SetTransactionTime(100100100);
    gl_systemDequeData.PushRTData(pRTData);
    CStockRTDataPtr pRTData2 = make_shared<CStockRTData>();
    gl_systemDequeData.PushRTData(pRTData2);
    pRTData2->SetTransactionTime(200200200);
    EXPECT_EQ(gl_systemDequeData.GetRTDataDequeSize(), 2);
    CStockRTDataPtr p2 = gl_systemDequeData.PopRTData();
    EXPECT_EQ(gl_systemDequeData.GetRTDataDequeSize(), 1);
    EXPECT_EQ(p2->GetTransactionTime(), 100100100);
  }

  // ���������ȼ��Ķ��д洢��ʱʵʱ���ݡ�
  TEST(SystemDequeDataTest, TestGetPriorityRTDataDuqueSize) {
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataDequeSize(), 0);
    CStockRTDataPtr pRTData = make_shared<CStockRTData>();
    pRTData->SetTransactionTime(100100100);
    gl_systemDequeData.PushPriorityRTData(pRTData);
    CStockRTDataPtr pRTData2 = make_shared<CStockRTData>();
    pRTData2->SetTransactionTime(200200200);
    gl_systemDequeData.PushPriorityRTData(pRTData2);
    CStockRTDataPtr pRTData3 = make_shared<CStockRTData>();
    pRTData3->SetTransactionTime(200200);
    gl_systemDequeData.PushPriorityRTData(pRTData3);
    CStockRTDataPtr pRTData4 = make_shared<CStockRTData>();
    pRTData4->SetTransactionTime(200);
    gl_systemDequeData.PushPriorityRTData(pRTData4);
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataDequeSize(), 4);
    CStockRTDataPtr p2 = gl_systemDequeData.PopPriorityRTData();
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataDequeSize(), 3);
    EXPECT_EQ(p2->GetTransactionTime(), 200);
    p2 = gl_systemDequeData.PopPriorityRTData();
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataDequeSize(), 2);
    EXPECT_EQ(p2->GetTransactionTime(), 200200);
  }
}