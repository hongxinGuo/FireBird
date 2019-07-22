//////////////////////////////////////////////////////////////////////////////////////////
//
// ϵͳ���������еĸ��������Բ��õ�ͬ�������Է�ֹ�߳�֮��������ݲ�ͬ�����⣬�����µĲ��Ժ���û�в�������ͬ�����ƣ�
// ֻ�Ǽ򵥵Ĳ������ڲ��߼���
//
//
///////////////////////////////////////////////////////////////////////////////////////////

#include"stdafx.h"
#include"pch.h"

#include"SystemDequeData.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(SystemDequeDataTest, TestGetRTDataDuqueSize) {
    CSystemDequeData dequeData;
    EXPECT_EQ(dequeData.GetRTDataDequeSize(), 0);
    CStockRTDataPtr pRTData = make_shared<CStockRTData>();
    pRTData->SetTime(100100100);
    dequeData.PushRTData(pRTData);
    EXPECT_EQ(dequeData.GetRTDataDequeSize(), 1);
    CStockRTDataPtr p2 = dequeData.PopRTData();
    EXPECT_EQ(dequeData.GetRTDataDequeSize(), 0);
    EXPECT_EQ(p2->GetTime(), 100100100);
  }
}