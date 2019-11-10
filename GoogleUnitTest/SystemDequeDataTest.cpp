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

#include"QueueRTData.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(SystemDaeuqDataTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    long l = gl_systemMessage.GetInformationDequeSize();
    CQueueRTData systemdeque; // ���ɵڶ���ʵ������һ��Ϊȫ�ֱ�����ϵͳ����ʱ�������ˣ�
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // ϵͳ��������
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST(SystemDequeDataTest, TestGetDuqueRTDataSize) {
    EXPECT_EQ(gl_QueueSinaRTData.GetDequeRTDataSize(), 0);
    CRTDataPtr pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(100100100);
    gl_QueueSinaRTData.PushDequeRTData(pRTData);
    CRTDataPtr pRTData2 = make_shared<CRTData>();
    gl_QueueSinaRTData.PushDequeRTData(pRTData2);
    pRTData2->SetTransactionTime(200200200);
    EXPECT_EQ(gl_QueueSinaRTData.GetDequeRTDataSize(), 2);
    CRTDataPtr p2 = gl_QueueSinaRTData.PopDequeRTData();
    EXPECT_EQ(gl_QueueSinaRTData.GetDequeRTDataSize(), 1);
    EXPECT_EQ(p2->GetTransactionTime(), 100100100);
  }

  // ���������ȼ��Ķ��д洢��ʱʵʱ���ݡ�
  TEST(SystemDequeDataTest, TestGetPriorityRTDataDuqueSize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_EQ(gl_QueueSinaRTData.GetPriorityRTDataSize(), 0);
    CRTDataPtr pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(100100100);
    gl_QueueSinaRTData.PushPriorityRTData(pRTData);
    CRTDataPtr pRTData2 = make_shared<CRTData>();
    pRTData2->SetTransactionTime(200200200);
    pRTData2->SetBuy(1);
    gl_QueueSinaRTData.PushPriorityRTData(pRTData2);
    CRTDataPtr pRTData3 = make_shared<CRTData>();
    pRTData3->SetTransactionTime(200200);
    gl_QueueSinaRTData.PushPriorityRTData(pRTData3);
    CRTDataPtr pRTData4 = make_shared<CRTData>();
    pRTData4->SetTransactionTime(200);
    gl_QueueSinaRTData.PushPriorityRTData(pRTData4);
    CRTDataPtr pRTData5 = make_shared<CRTData>();
    pRTData5->SetTransactionTime(200200200);
    pRTData5->SetBuy(2);
    gl_QueueSinaRTData.PushPriorityRTData(pRTData5);  // �����pRTData2��ʱ����ͬ��Ӧ��λ��pRTData2֮��
    EXPECT_EQ(gl_QueueSinaRTData.GetPriorityRTDataSize(), 5);
    CRTDataPtr p2 = gl_QueueSinaRTData.PopPriorityRTData();
    EXPECT_EQ(gl_QueueSinaRTData.GetPriorityRTDataSize(), 4);
    EXPECT_EQ(p2->GetTransactionTime(), 200);
    p2 = gl_QueueSinaRTData.PopPriorityRTData();
    EXPECT_EQ(gl_QueueSinaRTData.GetPriorityRTDataSize(), 3);
    EXPECT_EQ(p2->GetTransactionTime(), 200200);
    p2 = gl_QueueSinaRTData.PopPriorityRTData();
    EXPECT_EQ(gl_QueueSinaRTData.GetPriorityRTDataSize(), 2);
    EXPECT_EQ(p2->GetTransactionTime(), 100100100);
    p2 = gl_QueueSinaRTData.PopPriorityRTData();
    EXPECT_EQ(gl_QueueSinaRTData.GetPriorityRTDataSize(), 1);
    EXPECT_EQ(p2->GetTransactionTime(), 200200200);
    EXPECT_EQ(p2->GetBuy(), 1);
    p2 = gl_QueueSinaRTData.PopPriorityRTData();
    EXPECT_EQ(gl_QueueSinaRTData.GetPriorityRTDataSize(), 0);
    EXPECT_EQ(p2->GetBuy(), 2); // ��������ͬʱ�������Ӧ��λ�ں���
    EXPECT_EQ(p2->GetTransactionTime(), 200200200);
  }
}