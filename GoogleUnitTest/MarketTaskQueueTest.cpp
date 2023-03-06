#include"pch.h"

#include"GeneralCheck.h"
#include"MarketTaskQueue.h"

namespace FireBirdTest {
	class CMarketTaskQueueTest : public testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			GeneralCheck();
		}

	public:
		CMarketTaskQueue marketTaskQueue;
	};

	TEST_F(CMarketTaskQueueTest, TestInitialize) {
		EXPECT_EQ(marketTaskQueue.Size(), 0);
		EXPECT_TRUE(marketTaskQueue.IsEmpty());
	}

	TEST_F(CMarketTaskQueueTest, TestGetSet) {
		CMarketTaskPtr pTask = make_shared<CMarketTask>();
		pTask->SetTime(101010);
		pTask->SetType(CHINA_MARKET_BUILD_TODAY_DATABASE__);
		marketTaskQueue.SetTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(10000);
		pTask->SetType(CHINA_MARKET_PROCESS_TODAY_RT_DATA__);
		marketTaskQueue.SetTask(pTask);

		EXPECT_EQ(marketTaskQueue.Size(), 2);
		EXPECT_FALSE(marketTaskQueue.IsEmpty());

		pTask = marketTaskQueue.GetTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_PROCESS_TODAY_RT_DATA__);
		pTask = marketTaskQueue.GetTask();
		EXPECT_EQ(pTask->GetTime(), 101010) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
		EXPECT_EQ(marketTaskQueue.Size(), 0);
	}
}
