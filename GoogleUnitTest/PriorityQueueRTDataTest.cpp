#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"PriorityQueueRTData.h"

namespace StockAnalysisTest {
  static CPriorityQueueRTData gl_PriorityQueueRTData;

  class CPriorityQueueRTDataTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_PriorityQueueRTData.Reset();
    }
  };

  TEST_F(CPriorityQueueRTDataTest, TestReset) {
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    pRTData->SetTransactionTime(1);
    gl_PriorityQueueRTData.PushRTData(pRTData);
    EXPECT_EQ(gl_PriorityQueueRTData.GetRTDataSize(), 1);
    gl_PriorityQueueRTData.Reset();
    EXPECT_EQ(gl_PriorityQueueRTData.GetRTDataSize(), 0);
  }

  TEST_F(CPriorityQueueRTDataTest, TestPushPopData) {
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    pRTData->SetTransactionTime(1);
    CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
    pRTData2->SetTransactionTime(0);
    gl_PriorityQueueRTData.PushRTData(pRTData);
    gl_PriorityQueueRTData.PushRTData(pRTData2);
    CWebRTDataPtr pRTData3 = gl_PriorityQueueRTData.PopRTData();
    EXPECT_EQ(pRTData3->GetTransactionTime(), 0) << "有优先权的队列，交易时间早的位于前列";
  }
}