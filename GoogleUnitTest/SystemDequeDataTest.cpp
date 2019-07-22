//////////////////////////////////////////////////////////////////////////////////////////
//
// 系统队列数据中的各个函数皆采用的同步机制以防止线程之间出现数据不同步问题，但此下的测试函数没有测试这种同步机制，
// 只是简单的测试了内部逻辑。
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