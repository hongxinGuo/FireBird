#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

//#include"ConvertToString.h"
#include"TimeConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class TimeConvertTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};

	TEST_F(TimeConvertTest, TestTransferToDate) {
		tm tm_, tm_2;
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 20; // GMTʱ��Ϊ20ʱ��������ʱ��Ϊ����4ʱ
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		tm_2 = tm_;
		long lDate = TransferToDate(&tm_);
		time_t tt = _mkgmtime(&tm_2);
		long lDate2 = TransferToDate(tt, 0); // UTCʱ��
		long lDate3 = TransferToDate(tt); // Ĭ�϶�����ʱ��
		EXPECT_EQ(lDate, 20000105);
		EXPECT_EQ(lDate2, 20000105);
		EXPECT_EQ(lDate3, 20000106) << "������ʱ���UTCʱ����8Сʱ���ʶ���6����";
	}

	TEST_F(TimeConvertTest, TestTransferToTime) {
		tm tm_, tm_2;
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 10;
		tm_.tm_min = 20;
		tm_.tm_sec = 30;
		tm_2 = tm_;
		long lTime = TransferToTime(&tm_);
		time_t tt = _mkgmtime(&tm_2);
		long lTime2 = TransferToTime(tt, 0); // UTCʱ��
		long lTime3 = TransferToTime(tt); // Ĭ�϶�����ʱ��
		EXPECT_EQ(lTime, lTime2);
		EXPECT_EQ(lTime, 102030);
		EXPECT_EQ(lTime2, 102030);
		EXPECT_EQ(lTime3, 182030);
	}

	TEST_F(TimeConvertTest, TestTransferToDateTime) {
		tm tm_, tm_2;
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 10;
		tm_.tm_min = 20;
		tm_.tm_sec = 30;
		tm_2 = tm_;
		INT64 lDateTime = TransferToDateTime(&tm_);
		time_t tt = _mkgmtime(&tm_2);
		INT64 lDateTime2 = TransferToDateTime(tt, 0); // UTCʱ��
		INT64 lDateTime3 = TransferToDateTime(tt); // Ĭ�϶�����ʱ��
		EXPECT_EQ(lDateTime, lDateTime2);
		EXPECT_EQ(lDateTime, 20000105102030);
		EXPECT_EQ(lDateTime2, 20000105102030);
		EXPECT_EQ(lDateTime3, 20000105182030);
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

	TEST_F(TimeConvertTest, TestGetCurrentMonday) {
		EXPECT_EQ(20200720, GetCurrentMonday(20200720)) << _T("20200720Ϊ����һ");
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

	TEST_F(TimeConvertTest, TestTransferToTTime) {
		EXPECT_EQ(315601200, TransferToTTime(19800101, gl_pWorldMarket->GetMarketTimeZone(), 150000)) << "������׼ʱ���19800101150000����UTCʱ��Ϊ315601200";
		EXPECT_EQ(315558000, TransferToTTime(19800101, gl_pChinaMarket->GetMarketTimeZone(), 150000)) << "������׼ʱ���19800101150000����UTCʱ��Ϊ315558000";
	}

	TEST_F(TimeConvertTest, TestTransferToDate2) {
		tm tm_;
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 0;
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		time_t tt = _mkgmtime(&tm_);
		long lDate = TransferToDate(tt, 0); // UTCʱ��
		EXPECT_EQ(lDate, 20000105);
	}

	TEST_F(TimeConvertTest, TestGetStringOfDate) {
		char buffer[30];
		sprintf_s(buffer, _T("%4d��%2d��%2d��"), 2020, 02, 02);
		CString str;
		str = buffer;
		EXPECT_STREQ(ConvertDateToString(20200202), str);
	}

	TEST_F(TimeConvertTest, TestGetUTCTimeStruct) {
		tm tm_;
		time_t tUTC = 0;

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
		strConvertBufferToTime(CString strFormat, CString strBuffer, INT64 iTime) {
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

	class ConvertBufferToTimeTest : public::testing::TestWithParam<strConvertBufferToTime*>
	{
	protected:
		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			strConvertBufferToTime* pData = GetParam();
			strBuffer = pData->m_strBuffer;
			strFormat = pData->m_strFormat;
			iTime = pData->m_Time;
		}

		virtual void TearDown(void) override {
			// clearup
		}

	public:
		CString strFormat;
		CString strBuffer;
		INT64 iTime;
	};

	INSTANTIATE_TEST_SUITE_P(TestConvertBufferToTime, ConvertBufferToTimeTest, testing::Values(&Data101, &Data102, &Data103,
		&Data104, &Data105, &Data106));

	TEST_P(ConvertBufferToTimeTest, TestConvertBufferToTime) {
		time_t tt = ConvertBufferToTime(strFormat, strBuffer.GetBuffer());
		time_t tt2;
		tm tm_;
		INT64 year = iTime / 10000000000;
		INT64 month = iTime / 100000000 - year * 100;
		INT64 day = iTime / 1000000 - year * 10000 - month * 100;
		INT64 hour = iTime / 10000 - year * 1000000 - month * 10000 - day * 100;
		INT64 minute = iTime / 100 - year * 100000000 - month * 1000000 - day * 10000 - hour * 100;
		INT64 second = iTime - year * 10000000000 - month * 100000000 - day * 1000000 - hour * 10000 - minute * 100;
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;
		tt2 = _mkgmtime(&tm_);
		if (tt2 > -1) {
			tt2 -= 8 * 3600; // ConvertBufferToTimeĬ��ʹ�ö�������׼ʱ��
		}
		if (iTime < 19000101000000) tt2 = iTime;
		EXPECT_EQ(tt, tt2);
	}

	TEST_P(ConvertBufferToTimeTest, TestConvertStringToTime) {
		time_t tt = ConvertStringToTime(strFormat, strBuffer);
		time_t tt2;
		tm tm_;
		INT64 year = iTime / 10000000000;
		INT64 month = iTime / 100000000 - year * 100;
		INT64 day = iTime / 1000000 - year * 10000 - month * 100;
		INT64 hour = iTime / 10000 - year * 1000000 - month * 10000 - day * 100;
		INT64 minute = iTime / 100 - year * 100000000 - month * 1000000 - day * 10000 - hour * 100;
		INT64 second = iTime - year * 10000000000 - month * 100000000 - day * 1000000 - hour * 10000 - minute * 100;
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		tm_.tm_hour = hour;
		tm_.tm_min = minute;
		tm_.tm_sec = second;
		tm_.tm_isdst = 0;
		tt2 = _mkgmtime(&tm_);
		if (tt2 > -1) {
			tt2 -= 8 * 3600; // ConvertStringToTimeĬ�ϲ��ö�������׼ʱ��
		}
		if (iTime < 19000101000000) tt2 = iTime;
		EXPECT_EQ(tt, tt2);
	}
}