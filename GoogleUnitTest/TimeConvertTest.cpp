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

	TEST_F(TimeConvertTest, TestXferDateToYearMonthDay) {
		int year, month, day;
		XferDateToYearMonthDay(20200101, year, month, day);
		EXPECT_TRUE((year == 2020) && (month == 01) && (day == 01));
		XferDateToYearMonthDay(20201230, year, month, day);
		EXPECT_TRUE((year == 2020) && (month == 12) && (day == 30));
	}

	TEST_F(TimeConvertTest, TestIsEarlyThen) {
		EXPECT_TRUE(IsEarlyThen(20200101, 20200115, 13));
		EXPECT_FALSE(IsEarlyThen(20200101, 20200115, 14));
		EXPECT_TRUE(IsEarlyThen(20200115, 20200201, 16));
		EXPECT_FALSE(IsEarlyThen(20200115, 20200201, 17));
		EXPECT_TRUE(IsEarlyThen(20191101, 20200115, 74));
		EXPECT_FALSE(IsEarlyThen(20191101, 20200115, 75));
	}

	TEST_F(TimeConvertTest, TestXferToYYYYMMDD2) {
		EXPECT_EQ(XferToYYYYMMDD("2020-10-10"), 20201010);
		EXPECT_EQ(XferToYYYYMMDD("2020-01-02"), 20200102);
		EXPECT_EQ(XferToYYYYMMDD("2020-1-2"), 20200102);
	}

	TEST_F(TimeConvertTest, TestGetNextMonth) {
		EXPECT_EQ(GetNextMonth(20200101), 20200201);
		EXPECT_EQ(GetNextMonth(20201205), 20210101);
		EXPECT_EQ(GetNextMonth(20201111), 20201201);
		EXPECT_EQ(GetNextMonth(20200231), 20200301) << "不检查日期的合法性";
	}

	TEST_F(TimeConvertTest, TestGetNextDate) {
		EXPECT_EQ(GetNextDay(20200101, 1), 20200102);
		EXPECT_EQ(GetNextDay(20200101, 11), 20200112);
		EXPECT_EQ(GetNextDay(20201231, 1), 20210101);
		EXPECT_EQ(GetNextDay(20201221, 14), 20210104);
	}

	TEST_F(TimeConvertTest, TestGetPrevDay) {
		EXPECT_EQ(GetPrevDay(20200102), 20200101);
		EXPECT_EQ(GetPrevDay(20200112, 10), 20200102);
		EXPECT_EQ(GetPrevDay(20200102, 11), 20191222);
		EXPECT_EQ(GetPrevDay(20200202, 11), 20200122);
	}

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

	TEST_F(TimeConvertTest, TestIsEarlyThen2) {
		EXPECT_FALSE(IsEarlyThen(20200101, 20200201, 32));
		EXPECT_FALSE(IsEarlyThen(20200101, 20200201, 33));
		EXPECT_TRUE(IsEarlyThen(20200101, 20200201, 30));
		EXPECT_FALSE(IsEarlyThen(20200101, 20200201, 31));
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

	TEST_F(TimeConvertTest, TestGetNextMonday3) {
		EXPECT_EQ(toYearMonthDay(20200727), GetNextMonday(toYearMonthDay(20200720))) << "20200720为星期一";
		EXPECT_EQ(toYearMonthDay(20200727), GetNextMonday(toYearMonthDay(20200721)));
		EXPECT_EQ(toYearMonthDay(20200727), GetNextMonday(toYearMonthDay(20200722)));
		EXPECT_EQ(toYearMonthDay(20200727), GetNextMonday(toYearMonthDay(20200723)));
		EXPECT_EQ(toYearMonthDay(20200727), GetNextMonday(toYearMonthDay(20200724)));
		EXPECT_EQ(toYearMonthDay(20200727), GetNextMonday(toYearMonthDay(20200725)));
		EXPECT_EQ(toYearMonthDay(20200727), GetNextMonday(toYearMonthDay(20200726)));
		EXPECT_EQ(toYearMonthDay(20200720), GetNextMonday(toYearMonthDay(20200713)));
		EXPECT_EQ(toYearMonthDay(20200720), GetNextMonday(toYearMonthDay(20200714)));
		EXPECT_EQ(toYearMonthDay(20200720), GetNextMonday(toYearMonthDay(20200715)));
		EXPECT_EQ(toYearMonthDay(20200720), GetNextMonday(toYearMonthDay(20200716)));
		EXPECT_EQ(toYearMonthDay(20200720), GetNextMonday(toYearMonthDay(20200717)));
		EXPECT_EQ(toYearMonthDay(20200720), GetNextMonday(toYearMonthDay(20200718)));
		EXPECT_EQ(toYearMonthDay(20200720), GetNextMonday(toYearMonthDay(20200719)));
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
}
