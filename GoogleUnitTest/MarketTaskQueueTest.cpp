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
		marketTaskQueue.StoreTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(10000);
		pTask->SetType(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		marketTaskQueue.StoreTask(pTask);

		pTask = make_shared<CMarketTask>();
		pTask->SetTime(1);
		pTask->SetType(CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		marketTaskQueue.StoreTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(1);
		pTask->SetType(CHINA_MARKET_CHOICE_10_RS_STRONG_1_STOCK_SET__);
		marketTaskQueue.StoreTask(pTask);

		EXPECT_EQ(marketTaskQueue.Size(), 4);
		EXPECT_FALSE(marketTaskQueue.IsEmpty());

		pTask = marketTaskQueue.GetTask();
		EXPECT_EQ(pTask->GetTime(), 1) << "����ʱ��˳������,���������ǰ��";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		pTask = marketTaskQueue.GetTask();
		EXPECT_EQ(pTask->GetTime(), 1);
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_CHOICE_10_RS_STRONG_1_STOCK_SET__) << "��ͬʱ�����������˳�������Ⱥ�";
		pTask = marketTaskQueue.GetTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "����ʱ��˳������,���������ǰ��";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_DISTRIBUTE_AND_CALCULATE_RT_DATA__);
		pTask = marketTaskQueue.GetTask();
		EXPECT_EQ(pTask->GetTime(), 101010) << "����ʱ��˳������,���������ǰ��";
		EXPECT_EQ(pTask->GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
		EXPECT_EQ(marketTaskQueue.Size(), 0);
	}
}