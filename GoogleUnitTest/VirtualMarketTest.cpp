#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"

#include"VirtualMarket.h"
#include"TimeConvert.h"

#include"MarketTask.h"

using namespace std;
using std::literals::chrono_literals::operator ""s;
using std::literals::chrono_literals::operator ""min;
using std::literals::chrono_literals::operator ""h;

namespace FireBirdTest {
	class CVirtualMarketTest : public testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualMarket virtualMarket;
	};

	TEST_F(CVirtualMarketTest, TestIsResetTime) {
		EXPECT_FALSE(virtualMarket.IsResetTime()) << "永远为假";
	}

	TEST_F(CVirtualMarketTest, TestAddTask) {
		virtualMarket.DiscardAllMarketTask();

		virtualMarket.AddTask(3, local_seconds(1h + 0min + 0s));
		virtualMarket.AddTask(3, local_seconds(2h + 0min + 0s));
		virtualMarket.AddTask(4, local_seconds(1h + 0min + 0s));
		virtualMarket.AddTask(1, local_seconds(1h + 0min + 0s));
		virtualMarket.AddTask(6, local_seconds(1s));
		virtualMarket.AddTask(5, local_seconds(1s));

		auto pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1s)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1s)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1h)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1h)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(1h)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 3);
		EXPECT_EQ(pTask->GetTime(), local_seconds(2h));
		virtualMarket.DiscardCurrentMarketTask();

		EXPECT_TRUE(virtualMarket.IsMarketTaskEmpty()) << virtualMarket.GetMarketTask()->GetTime();
	}

	TEST_F(CVirtualMarketTest, TestRectifyTaskTime) {
		auto pTask = make_shared<CMarketTask>();
		pTask->SetTime(local_seconds(24h + 03min + 50s));
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(local_seconds(30h));
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(local_seconds(27h + 40min + 10s));
		virtualMarket.AddTask(pTask);

		virtualMarket.AdjustTaskTime();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(3min + 50s)) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(3h + 40min + 10s)) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), local_seconds(6h)) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime1) {
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime());
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime2) {
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(local_seconds(0s)));
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(local_seconds(1min)));
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime1) {
		EXPECT_TRUE(virtualMarket.IsWorkingTime());
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime2) {
		EXPECT_TRUE(virtualMarket.IsWorkingTime(local_seconds(0s)));
		EXPECT_TRUE(virtualMarket.IsWorkingTime(local_seconds(1min)));
		EXPECT_TRUE(virtualMarket.IsWorkingTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime1) {
		EXPECT_FALSE(virtualMarket.IsDummyTime());
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime2) {
		EXPECT_FALSE(virtualMarket.IsDummyTime(local_seconds(0s)));
		EXPECT_FALSE(virtualMarket.IsDummyTime(local_seconds(1min)));
		EXPECT_FALSE(virtualMarket.IsDummyTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsTimeToResetSystem) {
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(local_seconds(0s)));
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(local_seconds(10min)));
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestMarketReady) {
		EXPECT_FALSE(virtualMarket.IsSystemReady()) << "市场基类默认为假";
		virtualMarket.SetSystemReady(false);
		EXPECT_FALSE(virtualMarket.IsSystemReady());
		virtualMarket.SetSystemReady(true);
		EXPECT_TRUE(virtualMarket.IsSystemReady());
	}

	TEST_F(CVirtualMarketTest, TestGetNextTradeDate) {
		chrono::days day;
		chrono::local_seconds lt = virtualMarket.ToLocalTime(gl_tpNow);

		virtualMarket.CalculateTime();

		if (virtualMarket.GetWeekDay() == chrono::Saturday) {
			day = chrono::days(2);
		}
		else if (virtualMarket.GetWeekDay() == chrono::Friday) {
			day = chrono::days(3);
		}
		else {
			day = chrono::days(1);
		}

		lt += day;
		EXPECT_EQ(virtualMarket.GetNextTradeDate(), chrono::floor<chrono::days>(lt));
	}

	TEST_F(CVirtualMarketTest, TestGetCurrentTradeDate) {
		chrono::days day;
		chrono::local_seconds lt = virtualMarket.ToLocalTime(gl_tpNow);

		virtualMarket.CalculateTime();

		if (virtualMarket.GetWeekDay() == chrono::Saturday) {
			day = chrono::days(1);
		}
		else if (virtualMarket.GetWeekDay() == chrono::Sunday) {
			day = chrono::days(2);
		}
		else {
			day = chrono::days(0);
		}

		lt -= day;
		chrono::year_month_day ymd = chrono::year_month_day{ chrono::floor<chrono::days>(lt) };
		long currentTradeDate = static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());

		EXPECT_EQ(virtualMarket.GetCurrentTradeDate(), toLocalDays(currentTradeDate));
	}

	TEST_F(CVirtualMarketTest, TestGetLastTradeDate) {
		chrono::days day;
		chrono::local_seconds lt = virtualMarket.ToLocalTime(gl_tpNow);

		virtualMarket.CalculateTime();
		if (virtualMarket.GetWeekDay() == chrono::Monday) {
			day = chrono::days(3); // 周五
		}
		else if (virtualMarket.GetWeekDay() == chrono::Sunday) {
			day = chrono::days(3); // 周四
		}
		else if (virtualMarket.GetWeekDay() == chrono::Saturday) {
			day = chrono::days(2); // 周四
		}
		else {
			day = chrono::days(1); // 上一日
		}
		lt -= day;
		chrono::year_month_day ymd = chrono::year_month_day{ chrono::floor<chrono::days>(lt) };
		long LastTradeDate = static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());

		EXPECT_EQ(virtualMarket.GetLastTradeDate(), toLocalDays(LastTradeDate));
	}
}
