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

  TEST(SystemDequeDataTest, TestGetDuqueRTDataSize) {
    EXPECT_EQ(gl_systemDequeData.GetDequeRTDataSize(), 0);
    CStockRTDataPtr pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(100100100);
    gl_systemDequeData.PushDequeRTData(pRTData);
    CStockRTDataPtr pRTData2 = make_shared<CRTData>();
    gl_systemDequeData.PushDequeRTData(pRTData2);
    pRTData2->SetTransactionTime(200200200);
    EXPECT_EQ(gl_systemDequeData.GetDequeRTDataSize(), 2);
    CStockRTDataPtr p2 = gl_systemDequeData.PopDequeRTData();
    EXPECT_EQ(gl_systemDequeData.GetDequeRTDataSize(), 1);
    EXPECT_EQ(p2->GetTransactionTime(), 100100100);
  }

  // ���������ȼ��Ķ��д洢��ʱʵʱ���ݡ�
  TEST(SystemDequeDataTest, TestGetPriorityRTDataDuqueSize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataSize(), 0);
    CStockRTDataPtr pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(100100100);
    gl_systemDequeData.PushPriorityRTData(pRTData);
    CStockRTDataPtr pRTData2 = make_shared<CRTData>();
    pRTData2->SetTransactionTime(200200200);
    pRTData2->SetBuy(1);
    gl_systemDequeData.PushPriorityRTData(pRTData2);
    CStockRTDataPtr pRTData3 = make_shared<CRTData>();
    pRTData3->SetTransactionTime(200200);
    gl_systemDequeData.PushPriorityRTData(pRTData3);
    CStockRTDataPtr pRTData4 = make_shared<CRTData>();
    pRTData4->SetTransactionTime(200);
    gl_systemDequeData.PushPriorityRTData(pRTData4);
    CStockRTDataPtr pRTData5 = make_shared<CRTData>();
    pRTData5->SetTransactionTime(200200200);
    pRTData5->SetBuy(2);
    gl_systemDequeData.PushPriorityRTData(pRTData5);  // �����pRTData2��ʱ����ͬ��Ӧ��λ��pRTData2֮��
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataSize(), 5);
    CStockRTDataPtr p2 = gl_systemDequeData.PopPriorityRTData();
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataSize(), 4);
    EXPECT_EQ(p2->GetTransactionTime(), 200);
    p2 = gl_systemDequeData.PopPriorityRTData();
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataSize(), 3);
    EXPECT_EQ(p2->GetTransactionTime(), 200200);
    p2 = gl_systemDequeData.PopPriorityRTData();
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataSize(), 2);
    EXPECT_EQ(p2->GetTransactionTime(), 100100100);
    p2 = gl_systemDequeData.PopPriorityRTData();
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataSize(), 1);
    EXPECT_EQ(p2->GetTransactionTime(), 200200200);
    EXPECT_EQ(p2->GetBuy(), 1);
    p2 = gl_systemDequeData.PopPriorityRTData();
    EXPECT_EQ(gl_systemDequeData.GetPriorityRTDataSize(), 0);
    EXPECT_EQ(p2->GetBuy(), 2); // ��������ͬʱ�������Ӧ��λ�ں���
    EXPECT_EQ(p2->GetTransactionTime(), 200200200);
  }
}