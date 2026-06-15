#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"TimeConvert.h"

namespace FireBirdTest {
	class TimeConvertTest : public ::testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(TimeConvertTest, TestGetNextMonday) {
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200720)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200721)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200722)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200723)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200724)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200725)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200726)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200713)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200714)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200715)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200716)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200717)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200718)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200719)));
	}
	TEST_F(TimeConvertTest, TestGetPrevMonday) {
		EXPECT_EQ(toLocalDays(20200713), GetPrevMonday(toLocalDays(20200720))) << "20200720为星期一";
		EXPECT_EQ(toLocalDays(20200713), GetPrevMonday(toLocalDays(20200721)));
		EXPECT_EQ(toLocalDays(20200713), GetPrevMonday(toLocalDays(20200722)));
		EXPECT_EQ(toLocalDays(20200713), GetPrevMonday(toLocalDays(20200723)));
		EXPECT_EQ(toLocalDays(20200713), GetPrevMonday(toLocalDays(20200724)));
		EXPECT_EQ(toLocalDays(20200713), GetPrevMonday(toLocalDays(20200725)));
		EXPECT_EQ(toLocalDays(20200713), GetPrevMonday(toLocalDays(20200726)));
		EXPECT_EQ(toLocalDays(20200706), GetPrevMonday(toLocalDays(20200716)));
		EXPECT_EQ(toLocalDays(20200706), GetPrevMonday(toLocalDays(20200717)));
		EXPECT_EQ(toLocalDays(20200706), GetPrevMonday(toLocalDays(20200718)));
		EXPECT_EQ(toLocalDays(20200706), GetPrevMonday(toLocalDays(20200719)));
		EXPECT_EQ(toLocalDays(20200720), GetPrevMonday(toLocalDays(20200727)));
	}

	TEST_F(TimeConvertTest, TestGetNextMonday2) {
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200720))) << "20200720为星期一";
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200721)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200722)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200723)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200724)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200725)));
		EXPECT_EQ(toLocalDays(20200727), GetNextMonday(toLocalDays(20200726)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200713)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200714)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200715)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200716)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200717)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200718)));
		EXPECT_EQ(toLocalDays(20200720), GetNextMonday(toLocalDays(20200719)));
	}

	TEST_F(TimeConvertTest, TestGetCurrentMonday) {
		EXPECT_EQ(toLocalDays(20200720), GetCurrentMonday(toLocalDays(20200720))) << "20200720为星期一";
		EXPECT_EQ(toLocalDays(20200720), GetCurrentMonday(toLocalDays(20200721)));
		EXPECT_EQ(toLocalDays(20200720), GetCurrentMonday(toLocalDays(20200722)));
		EXPECT_EQ(toLocalDays(20200720), GetCurrentMonday(toLocalDays(20200723)));
		EXPECT_EQ(toLocalDays(20200720), GetCurrentMonday(toLocalDays(20200724)));
		EXPECT_EQ(toLocalDays(20200720), GetCurrentMonday(toLocalDays(20200725)));
		EXPECT_EQ(toLocalDays(20200720), GetCurrentMonday(toLocalDays(20200726)));
		EXPECT_EQ(toLocalDays(20200713), GetCurrentMonday(toLocalDays(20200713)));
		EXPECT_EQ(toLocalDays(20200713), GetCurrentMonday(toLocalDays(20200714)));
		EXPECT_EQ(toLocalDays(20200713), GetCurrentMonday(toLocalDays(20200715)));
		EXPECT_EQ(toLocalDays(20200713), GetCurrentMonday(toLocalDays(20200716)));
		EXPECT_EQ(toLocalDays(20200713), GetCurrentMonday(toLocalDays(20200717)));
		EXPECT_EQ(toLocalDays(20200713), GetCurrentMonday(toLocalDays(20200718)));
		EXPECT_EQ(toLocalDays(20200713), GetCurrentMonday(toLocalDays(20200719)));
	}
	TEST_F(TimeConvertTest, TestGetNextSecond) {
		EXPECT_EQ(toLocalTime(1), GetNextSecond(toLocalTime(0)));
		EXPECT_EQ(toLocalTime(59), GetNextSecond(toLocalTime(58)));
		EXPECT_EQ(toLocalTime(100), GetNextSecond(toLocalTime(59)));
		EXPECT_EQ(toLocalTime(159), GetNextSecond(toLocalTime(158)));
		EXPECT_EQ(toLocalTime(200), GetNextSecond(toLocalTime(159)));
		EXPECT_EQ(toLocalTime(10100), GetNextSecond(toLocalTime(10059)));
		EXPECT_EQ(toLocalTime(20000), GetNextSecond(toLocalTime(15959)));
		EXPECT_EQ(toLocalTime(20059), GetNextSecond(toLocalTime(20058)));
		EXPECT_EQ(toLocalTime(20200), GetNextSecond(toLocalTime(20159)));
		EXPECT_EQ(toLocalTime(235959), GetNextSecond(toLocalTime(235958)));
		EXPECT_EQ(toLocalTime(240000), GetNextSecond(toLocalTime(235959)));
	}

	TEST_F(TimeConvertTest, TestGetNextTime) {
		EXPECT_EQ(toLocalTime(122304), GetNextTime(toLocalTime(112158), 1h, 1min, 6s));
		EXPECT_EQ(toLocalTime(121212), GetNextTime(toLocalTime(115800), 0h, 14min, 12s));
		EXPECT_EQ(toLocalTime(261202), GetNextTime(toLocalTime(221200), 4h, 0min, 2s));
	}

	TEST_F(TimeConvertTest, TestGetPrevTime) {
		EXPECT_EQ(toLocalTime(102052), GetPrevTime(toLocalTime(112158), 1h, 1min, 6s));
		EXPECT_EQ(toLocalTime(105548), GetPrevTime(toLocalTime(115000), 0h, 54min, 12s));
		EXPECT_EQ(toLocalTime(181158), GetPrevTime(toLocalTime(221200), 4h, 0min, 2s));
	}

	TEST(FormatToMKTest, TestFormatToMK) {
		string str = FormatToMK(123);
		EXPECT_EQ(str, " 123");
		str = FormatToMK(12 * 1024);
		EXPECT_EQ(str, "  12K");
		str = FormatToMK(12 * 1024 * 1024);
		EXPECT_EQ(str, "12M");
		str = FormatToMK(static_cast<int64_t>(12) * 1024 * 1024);
		EXPECT_EQ(str, "12M");
		str = FormatToMK(static_cast<int64_t>(1234567) * 1024 * 1024);
		EXPECT_EQ(str, "1234567M");
	}

	TEST_F(TimeConvertTest, TestToLocalDays) {
		chrono::year_month_day ymd{ 2020y / 7 / 20 };
		chrono::local_days days{ ymd };
		EXPECT_EQ(toLocalDays(20200720), chrono::local_days{ ymd });
		EXPECT_EQ(toLocalDays(20200720), chrono::local_days{ ymd });
		EXPECT_EQ(chrono::local_days(chrono::days(0)), chrono::local_days{ chrono::days(0) });
	}

	TEST_F(TimeConvertTest, TestToLocalDays2) {
		string s = "2020-01-02";
		istringstream ss(s);
		chrono::local_days ld;
		ss >> chrono::parse("%F", ld);
		chrono::year_month_day ymd(ld);
		EXPECT_EQ(ymd.year(), 2020y);
		EXPECT_EQ(ymd.month(), chrono::month(1));
		EXPECT_EQ(ymd.day(), chrono::day(2));
	}

	TEST_F(TimeConvertTest, TestToLocalDays3) {
		string s = "2020-01-02 12:22:33";
		istringstream ss(s);
		chrono::local_days ld;
		ss >> chrono::parse("%F", ld);
		chrono::year_month_day ymd(ld);
		EXPECT_EQ(ymd.year(), 2020y);
		EXPECT_EQ(ymd.month(), chrono::month(1));
		EXPECT_EQ(ymd.day(), chrono::day(2));

		chrono::local_seconds ls;
		istringstream ss2(s);
		ss2 >> chrono::parse("%F %T", ls);
		chrono::year_month_day ymd2{ chrono::local_days(floor<chrono::days>(ls)) };
		EXPECT_EQ(ymd2.year(), 2020y);
		EXPECT_EQ(ymd2.month(), chrono::month(1));
		EXPECT_EQ(ymd2.day(), chrono::day(2));
		chrono::hh_mm_ss<chrono::seconds> hms = toTodayClock(ls);
		EXPECT_EQ(hms.hours(), 12h);
		EXPECT_EQ(hms.minutes(), 22min);
		EXPECT_EQ(hms.seconds(), 33s);
	}
}
