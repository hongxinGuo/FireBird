#include"pch.h"

#include"GeneralCheck.h"

#include"PriorityQueueWebRTData.h"

namespace FireBirdTest {
	static CPriorityQueueWebRTData gl_PriorityQueueRTData;

	class CPriorityQueueRTDataTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			gl_PriorityQueueRTData.Reset();
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CPriorityQueueRTDataTest, TestReset) {
		const auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(1);
		gl_PriorityQueueRTData.PushData(pRTData);
		EXPECT_EQ(gl_PriorityQueueRTData.Size(), 1);
		gl_PriorityQueueRTData.Reset();
		EXPECT_EQ(gl_PriorityQueueRTData.Size(), 0);
	}

	TEST_F(CPriorityQueueRTDataTest, TestPushPopData) {
		const auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(1);
		const auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(0);
		gl_PriorityQueueRTData.PushData(pRTData);
		gl_PriorityQueueRTData.PushData(pRTData2);
		const CWebRTDataPtr pRTData3 = gl_PriorityQueueRTData.PopData();
		EXPECT_EQ(pRTData3->GetTransactionTime(), 0) << "有优先权的队列，交易时间早的位于前列";
	}
}
