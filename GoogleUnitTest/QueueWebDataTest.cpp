#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"QueueWebData.h"

namespace StockAnalysisTest {
  static CQueueWebData gl_QueueWebData;

  class CQueueWebDataTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_QueueWebData.Reset();
    }
  };

  TEST_F(CQueueWebDataTest, TestReset) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    pWebData->SetTime(1);
    gl_QueueWebData.PushData(pWebData);
    EXPECT_EQ(gl_QueueWebData.GetDataSize(), 1);
    gl_QueueWebData.Reset();
    EXPECT_EQ(gl_QueueWebData.GetDataSize(), 0);
  }

  TEST_F(CQueueWebDataTest, TestPushPopData) {
    CWebDataPtr pWebData = make_shared<CWebData>();
    pWebData->SetTime(1);
    CWebDataPtr pWebData2 = make_shared<CWebData>();
    pWebData2->SetTime(0);
    gl_QueueWebData.PushData(pWebData);
    gl_QueueWebData.PushData(pWebData2);
    CWebDataPtr pWebData3 = gl_QueueWebData.PopData();
    EXPECT_EQ(pWebData3->GetTime(), 1) << "无优先权的队列，与交易时间无关，按进队列的先后顺序出队列";
  }
}