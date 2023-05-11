#include"pch.h"

#include"GeneralCheck.h"
#include"MarketTask.h"

namespace FireBirdTest {
	class CMarketTaskTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

	public:
		CMarketTask marketTask;
	};

	TEST_F(CMarketTaskTest, TestInitialize) {
		EXPECT_EQ(marketTask.GetTime(), 0);
		EXPECT_EQ(marketTask.GetType(), 0);

		const CMarketTask task2(1010, CHINA_MARKET_BUILD_TODAY_DATABASE__);
		EXPECT_EQ(task2.GetTime(), 1010);
		EXPECT_EQ(task2.GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
	}

	TEST_F(CMarketTaskTest, TestGetTime) {
		EXPECT_EQ(marketTask.GetTime(), 0);

		marketTask.SetTime(100101);
		EXPECT_EQ(marketTask.GetTime(), 100101);
	}

	TEST_F(CMarketTaskTest, TestGetType) {
		EXPECT_EQ(marketTask.GetType(), 0);

		marketTask.SetType(CHINA_MARKET_BUILD_TODAY_DATABASE__);
		EXPECT_EQ(marketTask.GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE__);
	}
}
