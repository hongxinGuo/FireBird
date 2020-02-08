#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"QueueRTData.h"

namespace StockAnalysisTest {
  static CQueueRTData gl_QueueRTData;

  class CQueueRTDataTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_QueueRTData.Reset();
    }
  };

  TEST_F(CQueueRTDataTest, TestReset) {
    CRTDataPtr pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(1);
    gl_QueueRTData.PushRTData(pRTData);
    EXPECT_EQ(gl_QueueRTData.GetRTDataSize(), 1);
    gl_QueueRTData.Reset();
    EXPECT_EQ(gl_QueueRTData.GetRTDataSize(), 0);
  }

  TEST_F(CQueueRTDataTest, TestPushPopData) {
    CRTDataPtr pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(1);
    CRTDataPtr pRTData2 = make_shared<CRTData>();
    pRTData2->SetTransactionTime(0);
    gl_QueueRTData.PushRTData(pRTData);
    gl_QueueRTData.PushRTData(pRTData2);
    CRTDataPtr pRTData3 = gl_QueueRTData.PopRTData();
    EXPECT_EQ(pRTData3->GetTransactionTime(), 1) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";
  }
}