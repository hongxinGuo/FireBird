#include"pch.h"

 

#include"GeneralCheck.h"
import FireBird.Market.TaskQueue;

namespace FireBirdTest {
	class CMarketTaskQueueTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		CMarketTaskQueue marketTaskQueue;
	};

	TEST_F(CMarketTaskQueueTest, TestInitialize) {
		EXPECT_EQ(marketTaskQueue.Size(), 0);
		EXPECT_TRUE(marketTaskQueue.Empty());
	}

	TEST_F(CMarketTaskQueueTest, TestGetSet) {
		marketTaskQueue.AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE__, 101010);
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 10000);

		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 1);
		marketTaskQueue.AddTask(CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, 1);

		EXPECT_EQ(marketTaskQueue.Size(), 4);
		EXPECT_FALSE(marketTaskQueue.Empty());

		auto pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), 1) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), 1);
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__) << "相同时间的任务，排列顺序按入列先后";
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), 101010) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
		EXPECT_EQ(marketTaskQueue.Size(), 0);
	}

	TEST_F(CMarketTaskQueueTest, TestGetTasks) {
		marketTaskQueue.AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE__, 1);
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 3);
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 4);
		marketTaskQueue.AddTask(CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, 2);

		const auto vTask = marketTaskQueue.GetTasks();

		EXPECT_EQ(vTask.at(0)->GetTime(), 1);
		EXPECT_EQ(vTask.at(0)->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
		EXPECT_EQ(vTask.at(1)->GetTime(), 2);
		EXPECT_EQ(vTask.at(1)->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__);
		EXPECT_EQ(vTask.at(2)->GetTime(), 3);
		EXPECT_EQ(vTask.at(2)->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		EXPECT_EQ(vTask.at(3)->GetTime(), 4);
		EXPECT_EQ(vTask.at(3)->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
	}
}
