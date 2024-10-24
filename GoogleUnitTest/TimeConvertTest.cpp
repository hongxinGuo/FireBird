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

	TEST_F(TimeConvertTest, TestTransferToDate) {
		tm tm_{};
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 20; // GMT时间为20时，东八区时间为次日4时
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		tm tm_2 = tm_;
		const long lDate = ConvertToDate(&tm_);
		const time_t tt = _mkgmtime(&tm_2);
		const long lDate2 = ConvertToDate(tt, 0); // UTC时间
		const long lDate3 = ConvertToDate(tt, -8 * 3600); // 东八区时间
		EXPECT_EQ(lDate, 20000105);
		EXPECT_EQ(lDate2, 20000105);
		EXPECT_EQ(lDate3, 20000106) << "东八区时间比UTC时间早8小时，故而是6日了";
	}

	TEST_F(TimeConvertTest, TestTransferToTime) {
		tm tm_{};
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 10;
		tm_.tm_min = 20;
		tm_.tm_sec = 30;
		tm tm_2 = tm_;
		const long lTime = ConvertToTime(&tm_);
		const time_t tt = _mkgmtime(&tm_2);
		const long lTime2 = ConvertToTime(tt, 0); // UTC时间
		const long lTime3 = ConvertToTime(tt, -8 * 3600); // 东八区时间
		EXPECT_EQ(lTime, lTime2);
		EXPECT_EQ(lTime, 102030);
		EXPECT_EQ(lTime2, 102030);
		EXPECT_EQ(lTime3, 182030);
	}

	TEST_F(TimeConvertTest, TestTransferToDateTime) {
		tm tm_{};
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 10;
		tm_.tm_min = 20;
		tm_.tm_sec = 30;
		tm tm_2 = tm_;
		const INT64 lDateTime = ConvertToDateTime(&tm_);
		const time_t tt = _mkgmtime(&tm_2);
		const INT64 lDateTime2 = ConvertToDateTime(tt, 0); // UTC时间
		const INT64 lDateTime3 = ConvertToDateTime(tt, -8 * 3600); // 东八区时间
		EXPECT_EQ(lDateTime, lDateTime2);
		EXPECT_EQ(lDateTime, 20000105102030);
		EXPECT_EQ(lDateTime2, 20000105102030);
		EXPECT_EQ(lDateTime3, 20000105182030);
	}

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

	TEST_F(TimeConvertTest, TestXferYearMonthDayToYYYYMMDD) {
		EXPECT_EQ(XferYearMonthDayToYYYYMMDD(2020, 10, 10), 20201010);
		EXPECT_EQ(XferYearMonthDayToYYYYMMDD(2020, 1, 2), 20200102);
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
		EXPECT_EQ(20200727, GetNextMonday(20200720));
		EXPECT_EQ(20200727, GetNextMonday(20200721));
		EXPECT_EQ(20200727, GetNextMonday(20200722));
		EXPECT_EQ(20200727, GetNextMonday(20200723));
		EXPECT_EQ(20200727, GetNextMonday(20200724));
		EXPECT_EQ(20200727, GetNextMonday(20200725));
		EXPECT_EQ(20200727, GetNextMonday(20200726));
		EXPECT_EQ(20200720, GetNextMonday(20200713));
		EXPECT_EQ(20200720, GetNextMonday(20200714));
		EXPECT_EQ(20200720, GetNextMonday(20200715));
		EXPECT_EQ(20200720, GetNextMonday(20200716));
		EXPECT_EQ(20200720, GetNextMonday(20200717));
		EXPECT_EQ(20200720, GetNextMonday(20200718));
		EXPECT_EQ(20200720, GetNextMonday(20200719));
	}

	TEST_F(TimeConvertTest, TestGetPrevMonday) {
		EXPECT_EQ(20200720, GetPrevMonday(20200721));
		EXPECT_EQ(20200720, GetPrevMonday(20200722));
		EXPECT_EQ(20200720, GetPrevMonday(20200723));
		EXPECT_EQ(20200720, GetPrevMonday(20200724));
		EXPECT_EQ(20200720, GetPrevMonday(20200725));
		EXPECT_EQ(20200720, GetPrevMonday(20200726));
		EXPECT_EQ(20200720, GetPrevMonday(20200727));
		EXPECT_EQ(20200713, GetPrevMonday(20200714));
		EXPECT_EQ(20200713, GetPrevMonday(20200715));
		EXPECT_EQ(20200713, GetPrevMonday(20200716));
		EXPECT_EQ(20200713, GetPrevMonday(20200717));
		EXPECT_EQ(20200713, GetPrevMonday(20200718));
		EXPECT_EQ(20200713, GetPrevMonday(20200719));
		EXPECT_EQ(20200713, GetPrevMonday(20200720));
	}

	TEST_F(TimeConvertTest, TestIsEarlyThen2) {
		EXPECT_FALSE(IsEarlyThen(20200101, 20200201, 32));
		EXPECT_FALSE(IsEarlyThen(20200101, 20200201, 33));
		EXPECT_TRUE(IsEarlyThen(20200101, 20200201, 30));
		EXPECT_FALSE(IsEarlyThen(20200101, 20200201, 31));
	}

	TEST_F(TimeConvertTest, TestTimeSpawn) {
		EXPECT_EQ(TimeSpawn(20200201, 20200101), 31);
		EXPECT_EQ(TimeSpawn(20210101, 20200101), 366);
		EXPECT_EQ(TimeSpawn(20220101, 20210101), 365);
	}

	TEST_F(TimeConvertTest, TestGetCurrentMonday) {
		EXPECT_EQ(20200720, GetCurrentMonday(20200720)) << _T("20200720为星期一");
		EXPECT_EQ(20200720, GetCurrentMonday(20200721));
		EXPECT_EQ(20200720, GetCurrentMonday(20200722));
		EXPECT_EQ(20200720, GetCurrentMonday(20200723));
		EXPECT_EQ(20200720, GetCurrentMonday(20200724));
		EXPECT_EQ(20200720, GetCurrentMonday(20200725));
		EXPECT_EQ(20200720, GetCurrentMonday(20200726));
		EXPECT_EQ(20200713, GetCurrentMonday(20200713));
		EXPECT_EQ(20200713, GetCurrentMonday(20200714));
		EXPECT_EQ(20200713, GetCurrentMonday(20200715));
		EXPECT_EQ(20200713, GetCurrentMonday(20200716));
		EXPECT_EQ(20200713, GetCurrentMonday(20200717));
		EXPECT_EQ(20200713, GetCurrentMonday(20200718));
		EXPECT_EQ(20200713, GetCurrentMonday(20200719));
	}

	TEST_F(TimeConvertTest, TestGetNextSecond) {
		EXPECT_EQ(1, GetNextSecond(0));
		EXPECT_EQ(59, GetNextSecond(58));
		EXPECT_EQ(100, GetNextSecond(59));
		EXPECT_EQ(159, GetNextSecond(158));
		EXPECT_EQ(200, GetNextSecond(159));
		EXPECT_EQ(10100, GetNextSecond(10059));
		EXPECT_EQ(20000, GetNextSecond(15959));
		EXPECT_EQ(20059, GetNextSecond(20058));
		EXPECT_EQ(20200, GetNextSecond(20159));
		EXPECT_EQ(235959, GetNextSecond(235958));
		EXPECT_EQ(240000, GetNextSecond(235959));
	}

	TEST_F(TimeConvertTest, TestGetNextTime) {
		EXPECT_EQ(122304, GetNextTime(112158, 1, 1, 6));
		EXPECT_EQ(121212, GetNextTime(115800, 0, 14, 12));
		EXPECT_EQ(261202, GetNextTime(221200, 4, 0, 2));
	}

	TEST_F(TimeConvertTest, TestGetPrevTime) {
		EXPECT_EQ(102052, GetPrevTime(112158, 1, 1, 6));
		EXPECT_EQ(105548, GetPrevTime(115000, 0, 54, 12));
		EXPECT_EQ(181158, GetPrevTime(221200, 4, 0, 2));
	}

	TEST_F(TimeConvertTest, TestTransferToTTime) {
		EXPECT_EQ(315601200, ConvertToTTime(19800101, gl_pWorldMarket->GetMarketTimeZone(), 150000)) << "美东标准时间的19800101150000，其UTC时间为315601200";
		EXPECT_EQ(315558000, ConvertToTTime(19800101, gl_pChinaMarket->GetMarketTimeZone(), 150000)) << "北京标准时间的19800101150000，其UTC时间为315558000";
	}

	TEST_F(TimeConvertTest, TestTransferToDate2) {
		tm tm_;
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 0;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		const time_t tt = _mkgmtime(&tm_);
		const long lDate = ConvertToDate(tt, 0); // UTC时间
		EXPECT_EQ(lDate, 20000105);
	}

	TEST_F(TimeConvertTest, TestConvertDateToChineseTimeStampString) {
		char buffer[30];
		sprintf_s(buffer, _T("%4d年%2d月%2d日"), 2020, 02, 02);
		const CString str = buffer;
		EXPECT_STREQ(ConvertDateToChineseTimeStampString(20200202), str);
	}

	TEST_F(TimeConvertTest, TestGetUTCTimeStruct) {
		tm tm_;
		constexpr time_t tUTC = 0;

		GetUTCTimeStruct(&tm_, &tUTC);
		EXPECT_EQ(tm_.tm_year, 70);
		EXPECT_EQ(tm_.tm_mon, 0);
		EXPECT_EQ(tm_.tm_yday, 0);
		EXPECT_EQ(tm_.tm_hour, 0);
		EXPECT_EQ(tm_.tm_min, 0);
		EXPECT_EQ(tm_.tm_sec, 0);
	}

	TEST_F(TimeConvertTest, TestGetMarketTimeStruct) {
		tm tm_, tm2_;
		time_t tt;

		time(&tt);
		gmtime_s(&tm2_, &tt);
		GetMarketTimeStruct(&tm_, tt, -8 * 3600);
		EXPECT_TRUE((tm_.tm_hour == (tm2_.tm_hour + 8) || (tm_.tm_hour == tm2_.tm_hour - 16)));
	}

	struct strConvertBufferToTime {
		strConvertBufferToTime(const CString& strFormat, const CString& strBuffer, const INT64 iTime) {
			m_strFormat = strFormat;
			m_strBuffer = strBuffer;
			m_Time = iTime;
		}

	public:
		CString m_strFormat;
		CString m_strBuffer;
		INT64 m_Time;
	};

	strConvertBufferToTime Data101(_T("%4d%2d%2d%2d%2d%2d"), _T("20191030102030"), 20191030102030);
	strConvertBufferToTime Data102(_T("%d-%d-%d %d:%d:%d"), _T("2019-10-30 10:12:30"), 20191030101230);
	strConvertBufferToTime Data105(_T("%4d/%2d/%2d %2d:%2d:%2d"), _T("2019/11/05 12:11:15"), 20191105121115);
	strConvertBufferToTime Data103(_T("%4d/%2d/%2d %2d:%2d:%2d"), _T("0"), -1);
	strConvertBufferToTime Data104(_T("%4d/%4d/%4d %2d:%2d:%2d"), _T("1900/01/01"), -1);
	strConvertBufferToTime Data106(_T("%4d/%4d%4d %2d:%2d:%2d"), _T(" 12:12:12"), -1);

	class ConvertBufferToTimeTest : public::testing::TestWithParam<strConvertBufferToTime*> {
	protected:
		void SetUp() override {
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			const strConvertBufferToTime* pData = GetParam();
			strBuffer = pData->m_strBuffer;
			strFormat = pData->m_strFormat;
			iTime = pData->m_Time;
		}

		void TearDown() override {
			// clearUp
		}

	public:
		CString strFormat;
		CString strBuffer;
		INT64 iTime;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertBufferToTime, ConvertBufferToTimeTest, testing::Values(&Data101, &Data102, &Data103,
		                         &Data104, &Data105, &Data106));

	TEST_P(ConvertBufferToTimeTest, TestConvertBufferToTime) {
		const time_t tt = ConvertBufferToTime(strFormat, strBuffer.GetBuffer(), -8 * 3600);
		tm tm_;
		const INT64 year = iTime / 10000000000;
		const INT64 month = iTime / 100000000 - year * 100;
		const INT64 day = iTime / 1000000 - year * 10000 - month * 100;
		const INT64 hour = iTime / 10000 - year * 1000000 - month * 10000 - day * 100;
		const INT64 minute = iTime / 100 - year * 100000000 - month * 1000000 - day * 10000 - hour * 100;
		const INT64 second = iTime - year * 10000000000 - month * 100000000 - day * 1000000 - hour * 10000 - minute * 100;
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;
		time_t tt2 = _mkgmtime(&tm_);
		if (tt2 > -1) {
			tt2 -= 8 * 3600; // ConvertBufferToTime默认使用东八区标准时间
		}
		if (iTime < 19000101000000) tt2 = iTime;
		EXPECT_EQ(tt, tt2);
	}

	TEST_P(ConvertBufferToTimeTest, TestConvertStringToTime) {
		const time_t tt = ConvertStringToTime(strFormat, strBuffer, -8 * 3600);
		tm tm_;
		const INT64 year = iTime / 10000000000;
		const INT64 month = iTime / 100000000 - year * 100;
		const INT64 day = iTime / 1000000 - year * 10000 - month * 100;
		const INT64 hour = iTime / 10000 - year * 1000000 - month * 10000 - day * 100;
		const INT64 minute = iTime / 100 - year * 100000000 - month * 1000000 - day * 10000 - hour * 100;
		const INT64 second = iTime - year * 10000000000 - month * 100000000 - day * 1000000 - hour * 10000 - minute * 100;
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;
		time_t tt2 = _mkgmtime(&tm_);
		if (tt2 > -1) {
			tt2 -= 8 * 3600; // ConvertStringToTime默认采用东八区标准时间
		}
		if (iTime < 19000101000000) tt2 = iTime;
		EXPECT_EQ(tt, tt2);
	}

	TEST_F(TimeConvertTest, TestXferToTTime) {
		CString strTime = _T("2019-07-05T15:49:15.157000+00:00");
		time_t t = XferToTTime(strTime, _T("%4d-%02d-%02dT%02d:%02d:%02d.%06d+%02d:%02d"));
		tm t1{ 15, 49, 15, 05, 07, 2019 };
		time_t t2 = _mktime64(&t1);
		EXPECT_EQ(t2, t);

		strTime = _T("2019-07-05T15:49:15.157000+08:15");
		t = XferToTTime(strTime, _T("%4d-%02d-%02dT%02d:%02d:%02d.%06d+%02d:%02d"));
		tm t3{ 15, 49, 15, 05, 07, 2019 };
		t2 = _mktime64(&t3);
		t2 += 3600 * 8 + 60 * 15;
		EXPECT_EQ(t2, t);
	}

	TEST_F(TimeConvertTest, TestXferTimeToIndex) {
		EXPECT_EQ(XferChinaMarketTimeToIndex(10), 0);
		EXPECT_EQ(XferChinaMarketTimeToIndex(85000), 0);
		EXPECT_EQ(XferChinaMarketTimeToIndex(92900), 0);
		EXPECT_EQ(XferChinaMarketTimeToIndex(93000), 0);
		EXPECT_EQ(XferChinaMarketTimeToIndex(93100), 1);
		EXPECT_EQ(XferChinaMarketTimeToIndex(112900), 119);
		EXPECT_EQ(XferChinaMarketTimeToIndex(113000), 120);
		EXPECT_EQ(XferChinaMarketTimeToIndex(113100), 120);
		EXPECT_EQ(XferChinaMarketTimeToIndex(125900), 120);
		EXPECT_EQ(XferChinaMarketTimeToIndex(130000), 120);
		EXPECT_EQ(XferChinaMarketTimeToIndex(130100), 121);
		EXPECT_EQ(XferChinaMarketTimeToIndex(145900), 239);
		EXPECT_EQ(XferChinaMarketTimeToIndex(150000), 239);
		EXPECT_EQ(XferChinaMarketTimeToIndex(150100), 239);
		EXPECT_EQ(XferChinaMarketTimeToIndex(190000), 239);
	}

	TEST_F(TimeConvertTest, TestXferTimeToIndex2) {
		tm tm_;
		tm_.tm_hour = 0;
		tm_.tm_min = 0;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 0);
		tm_.tm_hour = 8;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 0);
		tm_.tm_hour = 9;
		tm_.tm_min = 29;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 0);
		tm_.tm_hour = 9;
		tm_.tm_min = 30;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 0);
		tm_.tm_hour = 9;
		tm_.tm_min = 31;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 1);
		tm_.tm_hour = 11;
		tm_.tm_min = 29;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 119);
		tm_.tm_hour = 11;
		tm_.tm_min = 30;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 120);
		tm_.tm_hour = 11;
		tm_.tm_min = 31;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 120);
		tm_.tm_hour = 12;
		tm_.tm_min = 59;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 120);
		tm_.tm_hour = 13;
		tm_.tm_min = 0;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 120);
		tm_.tm_hour = 13;
		tm_.tm_min = 1;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 121);
		tm_.tm_hour = 14;
		tm_.tm_min = 59;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 239);
		tm_.tm_hour = 15;
		tm_.tm_min = 0;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 239);
		tm_.tm_hour = 15;
		tm_.tm_min = 1;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 239);
		tm_.tm_hour = 17;
		tm_.tm_min = 31;
		EXPECT_EQ(XferChinaMarketTimeToIndex(&tm_), 239);
	}
}
