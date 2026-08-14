#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"

#include"MarketTaskQueue.h"
#include"TimeConvert.h"

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
		marketTaskQueue.AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE_, toLocalTime(101010));
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, toLocalTime(10000));

		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, toLocalTime(1));
		marketTaskQueue.AddTask(CHINA_MARKET_CHECK_SYSTEM, toLocalTime(1));

		EXPECT_EQ(marketTaskQueue.Size(), 4);
		EXPECT_FALSE(marketTaskQueue.Empty());

		auto pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(1)) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(1));
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM) << "相同时间的任务，排列顺序按入列先后";
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10000)) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(101010)) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE_);
		EXPECT_EQ(marketTaskQueue.Size(), 0);
	}

	TEST_F(CMarketTaskQueueTest, TestGetTasks) {
		marketTaskQueue.AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE_, toLocalTime(1));
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, toLocalTime(3));
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, toLocalTime(2));

		const auto vTask = marketTaskQueue.GetTasks();

		EXPECT_EQ(vTask.at(0)->GetTime(), toLocalTime(1));
		EXPECT_EQ(vTask.at(0)->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE_);
		EXPECT_EQ(vTask.at(1)->GetTime(), toLocalTime(2));
		EXPECT_EQ(vTask.at(1)->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		EXPECT_EQ(vTask.at(2)->GetTime(), toLocalTime(3));
		EXPECT_EQ(vTask.at(2)->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
	}
}
