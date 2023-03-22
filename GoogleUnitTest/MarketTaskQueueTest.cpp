#include"pch.h"

#include"GeneralCheck.h"
#include"MarketTaskQueue.h"

namespace FireBirdTest {
	class CMarketTaskQueueTest : public testing::Test {
		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
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
		marketTaskQueue.AddTask(CHINA_MARKET_BUILD_TODAY_DATABASE__, 101010);
		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 10000);

		marketTaskQueue.AddTask(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__, 1);
		marketTaskQueue.AddTask(CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__, 1);

		EXPECT_EQ(marketTaskQueue.Size(), 4);
		EXPECT_FALSE(marketTaskQueue.IsEmpty());

		auto pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardTask();
		EXPECT_EQ(pTask->GetTime(), 1) << "����ʱ��˳������,���������ǰ��";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardTask();
		EXPECT_EQ(pTask->GetTime(), 1);
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_STOCK_SET__) << "��ͬʱ�����������˳�������Ⱥ�";
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "����ʱ��˳������,���������ǰ��";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		pTask = marketTaskQueue.GetTask();
		marketTaskQueue.DiscardTask();
		EXPECT_EQ(pTask->GetTime(), 101010) << "����ʱ��˳������,���������ǰ��";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
		EXPECT_EQ(marketTaskQueue.Size(), 0);
	}
}
