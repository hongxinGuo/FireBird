#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"
#include "MarketTask.h"

#include"MarketTaskQueue.h"
#include"TimeConvert.h"

using std::literals::chrono_literals::operator ""s;
using std::literals::chrono_literals::operator ""min;
using std::literals::chrono_literals::operator ""h;

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
		marketTaskQueue.AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE_, local_seconds(10h + 10min + 10s));
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, local_seconds(1h + 0min + 0s));

		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, local_seconds(1s));
		marketTaskQueue.AddTask(CHINA_MARKET_CHECK_SYSTEM, local_seconds(1s));

		EXPECT_EQ(marketTaskQueue.Size(), 4);
		EXPECT_FALSE(marketTaskQueue.Empty());

		auto pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1s)) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1s));
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHECK_SYSTEM) << "相同时间的任务，排列顺序按入列先后";
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1h)) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardCurrentTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(10h + 10min + 10s)) << "任务按时间顺序排列,较早的排在前面";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE_);
		EXPECT_EQ(marketTaskQueue.Size(), 0);
	}

	TEST_F(CMarketTaskQueueTest, TestGetTasks) {
		marketTaskQueue.AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE_, local_seconds(1s));
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, local_seconds(3s));
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_, local_seconds(2s));

		const auto vTask = marketTaskQueue.GetTasks();

		EXPECT_EQ(vTask.at(0)->GetTime(), local_seconds(1s));
		EXPECT_EQ(vTask.at(0)->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE_);
		EXPECT_EQ(vTask.at(1)->GetTime(), local_seconds(2s));
		EXPECT_EQ(vTask.at(1)->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
		EXPECT_EQ(vTask.at(2)->GetTime(), local_seconds(3s));
		EXPECT_EQ(vTask.at(2)->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA_);
	}
}
