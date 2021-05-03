#include"pch.h"

#include"globedef.h"

#include"QueueWebRTData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  static CQueueWebRTData gl_QueueWebRTData;

  class CQueueWebRTDataTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_QueueWebRTData.Reset();
    }
  };

  TEST_F(CQueueWebRTDataTest, TestReset) {
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    pRTData->SetTransactionTime(1);
    gl_QueueWebRTData.PushData(pRTData);
    EXPECT_EQ(gl_QueueWebRTData.GetDataSize(), 1);
    gl_QueueWebRTData.Reset();
    EXPECT_EQ(gl_QueueWebRTData.GetDataSize(), 0);
  }

  TEST_F(CQueueWebRTDataTest, TestPushPopData) {
    CWebRTDataPtr pRTData = make_shared<CWebRTData>();
    pRTData->SetTransactionTime(1);
    CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
    pRTData2->SetTransactionTime(0);
    gl_QueueWebRTData.PushData(pRTData);
    gl_QueueWebRTData.PushData(pRTData2);
    CWebRTDataPtr pRTData3 = gl_QueueWebRTData.PopData();
    EXPECT_EQ(pRTData3->GetTransactionTime(), 1) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";
  }
}