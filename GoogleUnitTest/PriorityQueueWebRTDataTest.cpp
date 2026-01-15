#include"pch.h"

#include"GeneralCheck.h"

#include"PriorityQueueWebRTData.h"

namespace {
	CPriorityQueueWebRTData s_PriorityQueueRTData;
}

namespace FireBirdTest {
	class CPriorityQueueRTDataTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			s_PriorityQueueRTData.Reset();
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CPriorityQueueRTDataTest, TestReset) {
		const auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(1);
		s_PriorityQueueRTData.PushData(pRTData);
		EXPECT_EQ(s_PriorityQueueRTData.Size(), 1);
		s_PriorityQueueRTData.Reset();
		EXPECT_EQ(s_PriorityQueueRTData.Size(), 0);
	}

	TEST_F(CPriorityQueueRTDataTest, TestPushPopData) {
		const auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(1);
		const auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(0);
		s_PriorityQueueRTData.PushData(pRTData);
		s_PriorityQueueRTData.PushData(pRTData2);
		const CWebRTDataPtr pRTData3 = s_PriorityQueueRTData.PopData();
		EXPECT_EQ(pRTData3->GetTimePoint().time_since_epoch().count(), 0) << "有优先权的队列，交易时间早的位于前列";
	}
}
