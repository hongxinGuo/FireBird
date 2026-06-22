#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"TimeConvert.h"

namespace FireBirdTest {
	class CTimeConvertTest : public ::testing::Test {
	protected:
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

	using namespace std::chrono;

	TEST(IsEarlyThen, EarlyPlusSpanLessThanLater_ReturnsTrue) {
		local_days early{ year{ 2026 } / month{ 6 } / day{ 15 } };
		local_days later{ year{ 2026 } / month{ 6 } / day{ 17 } };
		EXPECT_TRUE(IsEarlyThen(early, later, 1)); // early + 1 day = 2026-06-16 < 2026-06-17
	}

	TEST(IsEarlyThen, EarlyPlusSpanEqualsLater_ReturnsFalse) {
		local_days early{ year{ 2026 } / month{ 6 } / day{ 15 } };
		local_days later{ year{ 2026 } / month{ 6 } / day{ 16 } };
		EXPECT_FALSE(IsEarlyThen(early, later, 1)); // early + 1 day = 2026-06-16 == later -> not <
	}

	TEST(IsEarlyThen, SameDaySpanZero_ReturnsFalse) {
		local_days d{ year{ 2026 } / month{ 6 } / day{ 15 } };
		EXPECT_FALSE(IsEarlyThen(d, d, 0)); // equal dates, span 0 -> not <
	}

	TEST_F(CTimeConvertTest, XferToLocalDays_RoundTrip) {
		auto ld = XferToLocalDays("2020-02-29");
		EXPECT_EQ(toFormattedDate(ld), 20200229);

		auto ld2 = XferToLocalDays("1999-12-31");
		EXPECT_EQ(toFormattedDate(ld2), 19991231);
	}

	TEST(GetNextMonth, YearMonthDay_MiddleOfMonth_ReturnsFirstOfNextMonth) {
		year_month_day ymd{ year{ 2026 } / month{ 6 } / day{ 15 } };
		auto next = GetNextMonth(ymd);
		EXPECT_EQ(toFormattedDate(next), 20260701L);
	}

	TEST(GetNextMonth, YearMonthDay_FirstDay_ReturnsFirstOfFollowingMonth) {
		year_month_day ymd{ year{ 2026 } / month{ 7 } / day{ 1 } };
		auto next = GetNextMonth(ymd);
		EXPECT_EQ(toFormattedDate(next), 20260801L);
	}

	TEST(GetNextMonth, LocalDays_EndOfYear_RollsToNextYearJanuary) {
		local_days ld{ year{ 2026 } / month{ 12 } / day{ 31 } };
		auto nextLd = GetNextMonth(ld);
		EXPECT_EQ(toFormattedDate(nextLd), 20270101L);
	}

	TEST(GetNextDay, LocalDays_DefaultSpan_IncrementsByOne) {
		local_days d{ year{ 2026 } / month{ 6 } / day{ 15 } };
		auto out = GetNextDay(d);
		EXPECT_EQ(toFormattedDate(out), 20260616L);
	}

	TEST(GetNextDay, YearMonthDay_SpanThree_CrossesMonthBoundary) {
		year_month_day ymd{ year{ 2026 } / month{ 6 } / day{ 29 } };
		auto out = GetNextDay(ymd, 3);
		EXPECT_EQ(toFormattedDate(out), 20260702L);
	}

	TEST(GetNextDay, ZeroSpan_ReturnsSameDay) {
		local_days d{ year{ 2026 } / month{ 12 } / day{ 31 } };
		auto out = GetNextDay(d, 0);
		EXPECT_EQ(toFormattedDate(out), 20261231L);
	}

	TEST(GetNextDay, LeapYear_Feb28_ToFeb29) {
		year_month_day ymd{ year{ 2020 } / month{ 2 } / day{ 28 } };
		auto out = GetNextDay(ymd);
		EXPECT_EQ(toFormattedDate(out), 20200229L);
	}

	TEST(GetNextDay, EndOfYear_RollsToNextYear) {
		local_days d{ year{ 2023 } / month{ 12 } / day{ 31 } };
		auto out = GetNextDay(d);
		EXPECT_EQ(toFormattedDate(out), 20240101L);
	}

	TEST(GetPrevDay, LocalDays_DefaultSpan_DecrementsByOne) {
		local_days d{ year{ 2026 } / month{ 6 } / day{ 15 } };
		auto out = GetPrevDay(d);
		EXPECT_EQ(toFormattedDate(out), 20260614L);
	}

	TEST(GetPrevDay, YearMonthDay_LeapYear_PreviousDayIsLeapDay) {
		year_month_day ymd{ year{ 2020 } / month{ 3 } / day{ 1 } };
		auto out = GetPrevDay(ymd); // default span 1
		EXPECT_EQ(toFormattedDate(out), 20200229L);
	}

	TEST(GetPrevDay, LocalDays_ZeroSpan_ReturnsSameDay) {
		local_days d{ year{ 2026 } / month{ 6 } / day{ 1 } };
		auto out = GetPrevDay(d, 0);
		EXPECT_EQ(toFormattedDate(out), 20260601L);
	}

	TEST(GetPrevDay, LocalDays_EndOfYear_RollsToPrevYear) {
		local_days d{ year{ 2024 } / month{ 1 } / day{ 1 } };
		auto out = GetPrevDay(d);
		EXPECT_EQ(toFormattedDate(out), 20231231L);
	}

	TEST_F(CTimeConvertTest, GetNextSecond_Overloads) {
		// hh_mm_ss overload: 23:59:58 -> next is 23:59:59
		auto hms = chrono::hh_mm_ss{ chrono::seconds{ 23 * 3600 + 59 * 60 + 58 } };
		auto next = GetNextSecond(hms);
		EXPECT_EQ(toFormattedTime(next), 235959u);

		// hh_mm_ss overload: 23:59:59 -> next is 24:00:00 represented as 240000
		auto hms2 = chrono::hh_mm_ss{ chrono::seconds{ 23 * 3600 + 59 * 60 + 59 } };
		auto next2 = GetNextSecond(hms2);
		EXPECT_EQ(toFormattedTime(next2), 0u) << "自动减去了一天";

		// local_seconds overload
		auto ls = toLocalTime(235958u);
		EXPECT_EQ(toFormattedTime(GetNextSecond(ls)), 235959u);
		EXPECT_EQ(toFormattedTime(GetNextSecond(toLocalTime(235959u))), 0u) << "自动减去了一天";
	}

	TEST_F(CTimeConvertTest, GetNextTime_And_GetPrevTime_HHMMSSOverload) {
		// Prepare hh_mm_ss from an unsigned time using helpers
		auto h = toTodayClock(toLocalTime(112158)); // 11:21:58
		auto next = GetNextTime(h, chrono::hours{ 1 }, chrono::minutes{ 1 }, chrono::seconds{ 6 });
		EXPECT_EQ(toFormattedTime(next), 122304u); // 12:23:04

		auto h2 = toTodayClock(toLocalTime(115800)); // 11:58:00
		auto next2 = GetNextTime(h2, chrono::hours{ 0 }, chrono::minutes{ 14 }, chrono::seconds{ 12 });
		EXPECT_EQ(toFormattedTime(next2), 121212u); // 12:12:12

		// Prev time via hh_mm_ss
		auto hp = toTodayClock(toLocalTime(112158));
		auto prev = GetPrevTime(hp, chrono::hours{ 1 }, chrono::minutes{ 1 }, chrono::seconds{ 6 });
		EXPECT_EQ(toFormattedTime(prev), 102052u); // 10:20:52
	}

	TEST_F(CTimeConvertTest, GetNextTime_And_GetPrevTime_LocalSecondsOverload) {
		// local_seconds overload (already covered in other tests, but ensure hh vs local_seconds parity)
		auto t = toLocalTime(112158u);
		auto next = GetNextTime(t, chrono::hours{ 1 }, chrono::minutes{ 1 }, chrono::seconds{ 6 });
		EXPECT_EQ(toFormattedTime(next), 122304u);

		auto prev = GetPrevTime(t, chrono::hours{ 1 }, chrono::minutes{ 1 }, chrono::seconds{ 6 });
		EXPECT_EQ(toFormattedTime(prev), 102052u);
	}

	TEST_F(CTimeConvertTest, NextMonth_FromMiddleOfMonth) {
		auto input = toLocalDays(20230115); // 2023-01-15
		auto out = GetNextMonth(input);
		EXPECT_EQ(toFormattedDate(out), 20230201);
	}

	TEST_F(CTimeConvertTest, NextMonth_FromEndOfMonth) {
		auto input = toLocalDays(20230131); // 2023-01-31
		auto out = GetNextMonth(input);
		EXPECT_EQ(toFormattedDate(out), 20230201);
	}

	TEST_F(CTimeConvertTest, NextMonth_LeapYearFeb) {
		auto input = toLocalDays(20200229); // 2020-02-29 (leap day)
		auto out = GetNextMonth(input);
		EXPECT_EQ(toFormattedDate(out), 20200301);
	}

	TEST_F(CTimeConvertTest, NextMonth_CrossYearDecember) {
		auto input = toLocalDays(20231210); // 2023-12-10
		auto out = GetNextMonth(input);
		EXPECT_EQ(toFormattedDate(out), 20240101);
	}

	TEST_F(CTimeConvertTest, TestGetNextMonday) {
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
	TEST_F(CTimeConvertTest, TestGetPrevMonday) {
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

	TEST_F(CTimeConvertTest, TestGetNextMonday2) {
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

	TEST_F(CTimeConvertTest, TestGetCurrentMonday) {
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
	TEST_F(CTimeConvertTest, TestGetNextSecond) {
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

	TEST_F(CTimeConvertTest, TestGetNextTime) {
		EXPECT_EQ(toLocalTime(122304), GetNextTime(toLocalTime(112158), 1h, 1min, 6s));
		EXPECT_EQ(toLocalTime(121212), GetNextTime(toLocalTime(115800), 0h, 14min, 12s));
		EXPECT_EQ(toLocalTime(261202), GetNextTime(toLocalTime(221200), 4h, 0min, 2s));
	}

	TEST_F(CTimeConvertTest, TestGetPrevTime) {
		EXPECT_EQ(toLocalTime(102052), GetPrevTime(toLocalTime(112158), 1h, 1min, 6s));
		EXPECT_EQ(toLocalTime(105548), GetPrevTime(toLocalTime(115000), 0h, 54min, 12s));
		EXPECT_EQ(toLocalTime(181158), GetPrevTime(toLocalTime(221200), 4h, 0min, 2s));
	}

	TEST_F(CTimeConvertTest, TestToLocalDays) {
		chrono::year_month_day ymd{ 2020y / 7 / 20 };
		chrono::local_days days{ ymd };
		EXPECT_EQ(toLocalDays(20200720), chrono::local_days{ ymd });
		EXPECT_EQ(toLocalDays(20200720), chrono::local_days{ ymd });
		EXPECT_EQ(chrono::local_days(chrono::days(0)), chrono::local_days{ chrono::days(0) });
	}

	TEST_F(CTimeConvertTest, TestToLocalDays2) {
		string s = "2020-01-02";
		istringstream ss(s);
		chrono::local_days ld;
		ss >> chrono::parse("%F", ld);
		chrono::year_month_day ymd(ld);
		EXPECT_EQ(ymd.year(), 2020y);
		EXPECT_EQ(ymd.month(), chrono::month(1));
		EXPECT_EQ(ymd.day(), chrono::day(2));
	}

	TEST_F(CTimeConvertTest, TestToLocalDays3) {
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

	TEST(TimeConvert_ToLocalTime, FromHhMmSs) {
		using namespace std::chrono;

		hh_mm_ss<seconds> t{ hours{ 13 } + minutes{ 5 } + seconds{ 7 } };
		local_seconds ls = toLocalTime(t);

		// Expect reconstructed unsigned time to match input (13:05:07 -> 130507)
		EXPECT_EQ(toFormattedTime(ls), 130507u);
	}

	TEST(TimeConvert_ToLocalTime, FromUnsigned) {
		using namespace std::chrono;

		unsigned input = 235959u;
		local_seconds ls = toLocalTime(input);

		// Expect round-trip to unsigned time to match
		EXPECT_EQ(toFormattedTime(ls), input);

		// Also verify a couple of components
		auto clk = toTodayClock(ls);
		EXPECT_EQ(clk.hours().count(), 23);
		EXPECT_EQ(clk.minutes().count(), 59);
		EXPECT_EQ(clk.seconds().count(), 59);
	}

	TEST(TimeConvertTest, ToYearMonthDay_FromLocalDays) {
		using namespace std::chrono;

		local_days ld = local_days{ year{ 2026 } / month{ 6 } / day{ 18 } };
		year_month_day ymd = toYearMonthDay(ld);

		EXPECT_EQ(static_cast<int>(ymd.year()), 2026);
		EXPECT_EQ(static_cast<unsigned>(ymd.month()), 6u);
		EXPECT_EQ(static_cast<unsigned>(ymd.day()), 18u);
	}

	TEST(TimeConvert_ToYearMonthDay_Unsigned, BasicDate) {
		using namespace std::chrono;

		unsigned input = 20260618u;
		year_month_day ymd = toYearMonthDay(input);

		EXPECT_EQ(static_cast<int>(ymd.year()), 2026);
		EXPECT_EQ(static_cast<unsigned>(ymd.month()), 6u);
		EXPECT_EQ(static_cast<unsigned>(ymd.day()), 18u);
	}

	TEST(TimeConvert_ToYearMonthDay_Unsigned, LeapDay) {
		using namespace std::chrono;

		unsigned input = 20200229u;
		year_month_day ymd = toYearMonthDay(input);

		EXPECT_EQ(static_cast<int>(ymd.year()), 2020);
		EXPECT_EQ(static_cast<unsigned>(ymd.month()), 2u);
		EXPECT_EQ(static_cast<unsigned>(ymd.day()), 29u);
	}

	TEST(TimeConvert_ToTodayClock, RoundTripFromHhMmSs) {
		using namespace std::chrono;

		hh_mm_ss<seconds> t{ hours{ 9 } + minutes{ 30 } + seconds{ 15 } };
		local_seconds ls = toLocalTime(t);

		auto clk = toTodayClock(ls);
		EXPECT_EQ(clk.hours().count(), 9);
		EXPECT_EQ(clk.minutes().count(), 30);
		EXPECT_EQ(clk.seconds().count(), 15);
	}

	TEST(TimeConvert_ToTodayClock, FromLocalDaysPlusTime) {
		using namespace std::chrono;

		local_days ld = local_days{ year{ 2026 } / month{ 6 } / day{ 18 } };
		local_seconds ls = local_seconds{ ld + hours{ 17 } + minutes{ 45 } + seconds{ 59 } };

		auto clk = toTodayClock(ls);
		EXPECT_EQ(clk.hours().count(), 17);
		EXPECT_EQ(clk.minutes().count(), 45);
		EXPECT_EQ(clk.seconds().count(), 59);
	}

	TEST(TimeConvert_ToLocalDateTime, CombinesDateAndTime) {
		using namespace std::chrono;

		local_days ld = local_days{ year{ 2026 } / month{ 6 } / day{ 18 } };
		hh_mm_ss<seconds> t{ hours{ 8 } + minutes{ 2 } + seconds{ 3 } };
		local_seconds dt = toLocalDateTime(ld, toLocalTime(t));

		// Date part
		local_days date_only = chrono::floor<chrono::days>(dt);
		EXPECT_EQ(toFormattedDate(date_only), 20260618L);

		// Time part
		EXPECT_EQ(toFormattedTime(dt), 80203u);
	}

	TEST(TimeConvert_ToLocalDateTime, RoundTripWithDifferentTime) {
		using namespace std::chrono;

		local_days ld = local_days{ year{ 2024 } / month{ 2 } / day{ 29 } }; // leap day
		hh_mm_ss<seconds> t{ hours{ 23 } + minutes{ 59 } + seconds{ 59 } };
		local_seconds dt = toLocalDateTime(ld, toLocalTime(t));

		local_days date_only = chrono::floor<chrono::days>(dt);
		EXPECT_EQ(toFormattedDate(date_only), 20240229L);
		EXPECT_EQ(toFormattedTime(dt), 235959u);
	}

	TEST(TimeConvert_ToFormattedDate, FromLocalDays) {
		using namespace std::chrono;

		local_days ld = local_days{ year{ 2026 } / month{ 6 } / day{ 18 } };
		long formatted = toFormattedDate(ld);

		EXPECT_EQ(formatted, 20260618L);
	}

	TEST(TimeConvert_ToFormattedDate, FromYearMonthDayOverload) {
		using namespace std::chrono;

		year_month_day ymd{ year{ 2020 } / month{ 2 } / day{ 29 } }; // leap day
		long formatted = toFormattedDate(ymd);

		EXPECT_EQ(formatted, 20200229L);
	}

	TEST(TimeConvert_ToFormattedDate, ConsistentOverloads) {
		using namespace std::chrono;

		year_month_day ymd{ year{ 1999 } / month{ 12 } / day{ 31 } };
		local_days ld{ ymd };

		EXPECT_EQ(toFormattedDate(ymd), toFormattedDate(ld));
	}

	TEST(TimeConvert_ToFormattedTime, FromHhMmSs) {
		using namespace std::chrono;

		hh_mm_ss<seconds> t{ hours{ 9 } + minutes{ 5 } + seconds{ 7 } };
		EXPECT_EQ(toFormattedTime(t), 90507u);
	}

	TEST(TimeConvert_ToFormattedTime, FromLocalSeconds) {
		using namespace std::chrono;

		hh_mm_ss<seconds> t{ hours{ 23 } + minutes{ 59 } + seconds{ 59 } };
		local_seconds ls = toLocalTime(t);

		EXPECT_EQ(toFormattedTime(ls), 235959u);

		// also verify midnight
		hh_mm_ss<seconds> midnight{ hours{ 0 } + minutes{ 0 } + seconds{ 0 } };
		EXPECT_EQ(toFormattedTime(midnight), 0u);
		EXPECT_EQ(toFormattedTime(toLocalTime(midnight)), 0u);
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

	TEST_F(CTimeConvertTest, FormatToMK_Boundaries) {
		// below 1K -> raw number, width 4 formatting with no suffix
		EXPECT_EQ(FormatToMK(1023), "1023");
		// exactly 1024 -> falls into else branch (not > 1024)
		EXPECT_EQ(FormatToMK(1024), "1024");
		// just above 1K -> K branch, 1025/1024 == 1 -> width 4 -> three spaces then 1, plus K
		EXPECT_EQ(FormatToMK(1025), "   1K");
		// 12 * 1024 -> "  12K" as in existing tests
		EXPECT_EQ(FormatToMK(12 * 1024), "  12K");
		// exactly 1M (1024*1024) -> not > 1M, so treated as K
		EXPECT_EQ(FormatToMK(1024 * 1024), "1024K");
		// just above 1M -> M branch
		EXPECT_EQ(FormatToMK(static_cast<int64_t>(1024) * 1024 + 1), "1M");
	}

	TEST_F(CTimeConvertTest, TestToFormattedDateString) {
		EXPECT_EQ(toFormattedDateString(12345678), "12345678");
		EXPECT_EQ(toFormattedDateString(20200101), "20200101");
		EXPECT_EQ(toFormattedDateString(19990131), "19990131");
	}
}
