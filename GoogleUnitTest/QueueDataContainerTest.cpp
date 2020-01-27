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
#include"ChinaMarket.h"

#include"RTDataContainer.h"

namespace StockAnalysisTest {
  TEST(QueueDataContainerTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    long l = gl_systemMessage.GetInformationDequeSize();
    for (int i = 0; i < l; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // �����Ϣ����
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  // ���������ȼ��Ķ��д洢��ʱʵʱ���ݡ�
  TEST(QueueDataContainerTest, TestGetRTDataDuqueSize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 0);
    CRTDataPtr pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(100100100);
    gl_RTDataContainer.PushSinaRTData(pRTData);
    CRTDataPtr pRTData2 = make_shared<CRTData>();
    pRTData2->SetTransactionTime(200200200);
    pRTData2->SetBuy(1);
    gl_RTDataContainer.PushSinaRTData(pRTData2);
    CRTDataPtr pRTData3 = make_shared<CRTData>();
    pRTData3->SetTransactionTime(200200);
    gl_RTDataContainer.PushSinaRTData(pRTData3);
    CRTDataPtr pRTData4 = make_shared<CRTData>();
    pRTData4->SetTransactionTime(200);
    gl_RTDataContainer.PushSinaRTData(pRTData4);
    CRTDataPtr pRTData5 = make_shared<CRTData>();
    pRTData5->SetTransactionTime(200200200);
    pRTData5->SetBuy(2);
    gl_RTDataContainer.PushSinaRTData(pRTData5);  // �����pRTData2��ʱ����ͬ��Ӧ��λ��pRTData2֮��
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 5);
    CRTDataPtr p2 = gl_RTDataContainer.PopSinaRTData();
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 4);
    EXPECT_EQ(p2->GetTransactionTime(), 200);
    p2 = gl_RTDataContainer.PopSinaRTData();
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 3);
    EXPECT_EQ(p2->GetTransactionTime(), 200200);
    p2 = gl_RTDataContainer.PopSinaRTData();
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 2);
    EXPECT_EQ(p2->GetTransactionTime(), 100100100);
    p2 = gl_RTDataContainer.PopSinaRTData();
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 1);
    EXPECT_EQ(p2->GetTransactionTime(), 200200200);
    EXPECT_EQ(p2->GetBuy(), 1);
    p2 = gl_RTDataContainer.PopSinaRTData();
    EXPECT_EQ(gl_RTDataContainer.GetSinaRTDataSize(), 0);
    EXPECT_EQ(p2->GetBuy(), 2); // ��������ͬʱ�������Ӧ��λ�ں���
    EXPECT_EQ(p2->GetTransactionTime(), 200200200);
  }
}