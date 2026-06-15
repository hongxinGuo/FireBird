#include"pch.h"

#include"GeneralCheck.h"
#include"VirtualMarket.h"

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

	TEST_F(CVirtualMarketTest, TestAddTask) {
		virtualMarket.DiscardAllMarketTask();

		virtualMarket.AddTask(3, 10000);
		virtualMarket.AddTask(3, 20000);
		virtualMarket.AddTask(4, 10000);
		virtualMarket.AddTask(1, 10000);
		virtualMarket.AddTask(6, 1);
		virtualMarket.AddTask(5, 1);

		auto pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(1)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(1)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10000)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10000)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(10000)) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 3);
		EXPECT_EQ(pTask->GetTime(), toLocalTime(20000));
		virtualMarket.DiscardCurrentMarketTask();

		EXPECT_TRUE(virtualMarket.IsMarketTaskEmpty()) << virtualMarket.GetMarketTask()->GetTime();
	}

	TEST_F(CVirtualMarketTest, TestRectifyTaskTime) {
		auto pTask = make_shared<CMarketTask>();
		pTask->SetTime(240350);
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(300000);
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(274010);
		virtualMarket.AddTask(pTask);

		virtualMarket.AdjustTaskTime();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(350)) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(34010)) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), toLocalTime(60000)) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime1) {
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime());
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime2) {
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(toLocalTime(0)));
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(toLocalTime(100)));
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime1) {
		EXPECT_TRUE(virtualMarket.IsWorkingTime());
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime2) {
		EXPECT_TRUE(virtualMarket.IsWorkingTime(toLocalTime(0)));
		EXPECT_TRUE(virtualMarket.IsWorkingTime(toLocalTime(100)));
		EXPECT_TRUE(virtualMarket.IsWorkingTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime1) {
		EXPECT_FALSE(virtualMarket.IsDummyTime());
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime2) {
		EXPECT_FALSE(virtualMarket.IsDummyTime(toLocalTime(0)));
		EXPECT_FALSE(virtualMarket.IsDummyTime(toLocalTime(100)));
		EXPECT_FALSE(virtualMarket.IsDummyTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsTimeToResetSystem) {
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(toLocalTime(0)));
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(toLocalTime(1000)));
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

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDate) {
		const chrono::local_days lDate = virtualMarket.GetMarketDate();
		string s = "1970-01-01";
		EXPECT_EQ(virtualMarket.GetStringOfMarketDate(), s);
	}
}
