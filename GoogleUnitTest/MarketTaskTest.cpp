#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"
#include"MarketTask.h"

using std::literals::chrono_literals::operator ""s;
using std::literals::chrono_literals::operator ""min;
using std::literals::chrono_literals::operator ""h;

namespace FireBirdTest {
	class CMarketTaskTest : public testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		CMarketTask marketTask;
	};

	TEST_F(CMarketTaskTest, TestInitialize) {
		EXPECT_EQ(marketTask.GetTime(), local_seconds( 0s ));
		EXPECT_EQ(marketTask.GetType(), 0);

		const CMarketTask task2(CHINA_MARKET_BUILD_TODAY_DATABASE_, local_seconds(10min + 10s));
		EXPECT_EQ(task2.GetTime(), local_seconds(10min + 10s));
		EXPECT_EQ(task2.GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE_);
	}

	TEST_F(CMarketTaskTest, TestGetTime) {
		EXPECT_EQ(marketTask.GetTime(), local_seconds( 0s ));

		marketTask.SetTime(local_seconds(10h + 01min + 01s));
		EXPECT_EQ(marketTask.GetTime(), local_seconds(10h + 01min + 01s));
	}

	TEST_F(CMarketTaskTest, TestGetType) {
		EXPECT_EQ(marketTask.GetType(), 0);

		marketTask.SetType(CHINA_MARKET_BUILD_TODAY_DATABASE_);
		EXPECT_EQ(marketTask.GetType(), CHINA_MARKET_BUILD_TODAY_DATABASE_);
	}
}
