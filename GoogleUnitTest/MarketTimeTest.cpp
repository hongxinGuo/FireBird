#include"pch.h"

#include "ChinaMarket.h"
#include"MarketTime.h"

#include"GeneralCheck.h"
#include "TimeConvert.h"

namespace FireBirdTest {
	class CMarketTimeTest : public testing::Test {
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
		CMarketTime marketTime;
	};

	TEST_F(CMarketTimeTest, TestCalculateMarketTime) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		time_t tUTC;
		tm tm_, tmLocal;

		time(&tUTC);
		marketTime.CalculateTime();
		EXPECT_EQ(tUTC, GetUTCTime());
		localtime_s(&tmLocal, &tUTC);
		GetMarketTimeStruct(&tm_, tUTC, marketTime.GetMarketTimeZone());
		long lTimeZone = 0;
		_get_timezone(&lTimeZone);
		marketTime.CalculateLastTradeDate();
		EXPECT_EQ(marketTime.GetDayOfWeek(), tm_.tm_wday);

		const long day = marketTime.ConvertToDate(&tm_);
		EXPECT_EQ(marketTime.GetMarketDate(), day);
		EXPECT_EQ(marketTime.GetMonthOfYear(), tm_.tm_mon + 1);
		EXPECT_EQ(marketTime.GetDateOfMonth(), tm_.tm_mday);
		EXPECT_EQ(marketTime.GetYear(), tm_.tm_year + 1900);

		const long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
		EXPECT_EQ(marketTime.GetMarketTime(), time);
		char buffer[30];
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		CString str = buffer;

		EXPECT_EQ(str.Compare(marketTime.GetStringOfLocalTime()), 0);

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tm_.tm_hour, tm_.tm_min, tm_.tm_sec);
		str = buffer;
		EXPECT_EQ(str.Compare(marketTime.GetStringOfMarketTime()), 0);
	}

	TEST_F(CMarketTimeTest, TestCalculateLastTraddeDay) {
		time_t tMarket;
		auto tUTCTime = gl_tUTCTime;

		for (int i = 0; i < 7; i++) {
			gl_tUTCTime = tUTCTime + i * 24 * 3600;
			tm tmMarketTime2 = marketTime.GetMarketTime(gl_tUTCTime);

			switch (tmMarketTime2.tm_wday) {
			case 1: // ����һ
				tMarket = gl_tUTCTime - 3 * 24 * 3600; // ������
				break;
			case 0: //������
				tMarket = gl_tUTCTime - 3 * 24 * 3600; // ����
				break;
			case 6: // ������
				tMarket = gl_tUTCTime - 2 * 24 * 3600; // ����
				break;
			default: // ����
				tMarket = gl_tUTCTime - 24 * 3600; // ��һ��
			}
			const tm tmMarketTime = marketTime.GetMarketTime(tMarket);
			auto lMarketLastTradeDate = marketTime.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(marketTime.CalculateLastTradeDate(), lMarketLastTradeDate) << i;
		}

		// �ָ�ԭ״
		gl_tUTCTime = tUTCTime;
	}

	TEST_F(CMarketTimeTest, TestCalculateCurrentTradeDay) {
		time_t tMarket;
		auto tUTCTime = gl_tUTCTime;
		for (int i = 0; i < 7; i++) {
			gl_tUTCTime = tUTCTime + i * 24 * 3600;
			tm tmMarketTime2 = marketTime.GetMarketTime(gl_tUTCTime);

			switch (tmMarketTime2.tm_wday) {
			case 0: //������
				tMarket = gl_tUTCTime - 2 * 24 * 3600; // ����
				break;
			case 6: // ������
				tMarket = gl_tUTCTime - 1 * 24 * 3600; // ����
				break;
			default: // ����
				tMarket = gl_tUTCTime; // ����
			}
			const tm tmMarketTime = marketTime.GetMarketTime(tMarket);
			auto lMarketCurrentTradeDate = marketTime.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(marketTime.CalculateCurrentTradeDate(), lMarketCurrentTradeDate) << i;
		}

		// �ָ�ԭ״
		gl_tUTCTime = tUTCTime;
	}

	TEST_F(CMarketTimeTest, TestCalculateNextTraddeDay) {
		time_t tMarket;
		auto tUTCTime = gl_tUTCTime;

		for (int i = 0; i < 7; i++) {
			gl_tUTCTime = tUTCTime + i * 24 * 3600;
			tm tmMarketTime2 = marketTime.GetMarketTime(gl_tUTCTime);

			switch (tmMarketTime2.tm_wday) {
			case 6: // ������
				tMarket = gl_tUTCTime + 2 * 24 * 3600; // ����һ
				break;
			case 5: // ����
				tMarket = gl_tUTCTime + 3 * 24 * 3600; // ����һ
				break;
			default: // ����
				tMarket = gl_tUTCTime + 24 * 3600; // ����
			}
			const tm tmMarketTime = marketTime.GetMarketTime(tMarket);
			auto lMarketNextTradeDate = marketTime.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(marketTime.CalculateNextTradeDate(), lMarketNextTradeDate) << i;
		}

		// �ָ�ԭ״
		gl_tUTCTime = tUTCTime;
	}

	TEST_F(CMarketTimeTest, TestTransferToMarketTime) {
		tm tm2_;
		time_t tt;

		marketTime.CalculateTime();
		time(&tt);
		gmtime_s(&tm2_, &tt);
		const tm tm_ = marketTime.GetMarketTime(GetUTCTime());
		EXPECT_TRUE((tm_.tm_hour == (tm2_.tm_hour + 8) || (tm_.tm_hour == tm2_.tm_hour - 16))) << "VirtualMarketĬ��Ϊ������";
	}

	TEST_F(CMarketTimeTest, TestTransferToUTCTime1) {
		tm tmMarket, tm2, tm3;
		long lMarketDate = 20000301;
		long lMarketTime = 122030;
		time_t tTime = 0;

		tmMarket.tm_year = lMarketDate / 10000 - 1900;
		tmMarket.tm_mon = lMarketDate / 100 - (lMarketDate / 10000) * 100 - 1;
		tmMarket.tm_mday = lMarketDate - (lMarketDate / 100) * 100;
		tmMarket.tm_hour = lMarketTime / 10000;
		tmMarket.tm_min = lMarketTime / 100 - (lMarketTime / 10000) * 100;
		tmMarket.tm_sec = lMarketTime - (lMarketTime / 100) * 100;

		tTime = marketTime.TransferToUTCTime(&tmMarket); // ʹ��Ĭ��ʱ��150000.
		tm2 = marketTime.GetMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 12);
		EXPECT_EQ(tm2.tm_min, 20);
		EXPECT_EQ(tm2.tm_sec, 30);

		tTime = marketTime.TransferToUTCTime(lMarketDate); // ʹ��Ĭ��ʱ��150000.
		tm2 = marketTime.GetMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 15);
		EXPECT_EQ(tm2.tm_min, 00);
		EXPECT_EQ(tm2.tm_sec, 00);

		tTime = marketTime.TransferToUTCTime(lMarketDate, lMarketTime);
		tm3 = marketTime.GetMarketTime(tTime);
		EXPECT_EQ(tm3.tm_year, 100);
		EXPECT_EQ(tm3.tm_mon, 2);
		EXPECT_EQ(tm3.tm_mday, 1);
		EXPECT_EQ(tm3.tm_hour, 12);
		EXPECT_EQ(tm3.tm_min, 20);
		EXPECT_EQ(tm3.tm_sec, 30);
	}

	TEST_F(CMarketTimeTest, TestTransferToMarketDate) {
		tm tmMarket;
		constexpr long lMarketDate = 20000301;
		constexpr long lMarketTime = 002030;

		tmMarket.tm_year = lMarketDate / 10000 - 1900;
		tmMarket.tm_mon = lMarketDate / 100 - (lMarketDate / 10000) * 100 - 1;
		tmMarket.tm_mday = lMarketDate - (lMarketDate / 100) * 100;
		tmMarket.tm_hour = lMarketTime / 10000;
		tmMarket.tm_min = lMarketTime / 100 - (lMarketTime / 10000) * 100;
		tmMarket.tm_sec = lMarketTime - (lMarketTime / 100) * 100;

		const time_t tTime = marketTime.TransferToUTCTime(&tmMarket);

		const long lMarketDate2 = marketTime.GetMarketDate(tTime);
		EXPECT_EQ(lMarketDate2, lMarketDate);
	}

	TEST_F(CMarketTimeTest, TestGetNextTradeDate) {
		time_t tUTC;
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			time(&tUTC);
			tUTC += i * 60 * 60 * 24;
			GetMarketTimeStruct(&tm2, tUTC, marketTime.GetMarketTimeZone());
			tm_ = tm2;
			marketTime.TEST_SetUTCTime(tUTC);
			marketTime.TEST_SetMarketTM(tm2);

			switch (tm_.tm_wday) {
			case 6: //������
				tUTC += 2 * 24 * 3600; //
				break;
			case 5: // ������
				tUTC += 3 * 24 * 3600; //
				break;
			default: // ����
				tUTC += 24 * 3600;
				break;
			}
			GetMarketTimeStruct(&tm_, tUTC, marketTime.GetMarketTimeZone());
			long nextTradeDate = marketTime.ConvertToDate(&tm_);
			EXPECT_EQ(marketTime.GetNextTradeDate(), nextTradeDate);
		}
	}

	TEST_F(CMarketTimeTest, TestGetCurrentTradeDate) {
		time_t tUTC;
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			time(&tUTC);
			tUTC += i * 60 * 60 * 24;
			GetMarketTimeStruct(&tm2, tUTC, marketTime.GetMarketTimeZone());
			tm_ = tm2;
			marketTime.TEST_SetUTCTime(tUTC);
			marketTime.TEST_SetMarketTM(tm2);

			switch (tm_.tm_wday) {
			case 0: //������
				tUTC -= 2 * 24 * 3600; //
				break;
			case 6: // ������
				tUTC -= 1 * 24 * 3600; //
				break;
			default: // ����
				break;
			}
			GetMarketTimeStruct(&tm_, tUTC, marketTime.GetMarketTimeZone());
			long currentTradeDate = marketTime.ConvertToDate(&tm_);
			EXPECT_EQ(marketTime.GetCurrentTradeDate(), currentTradeDate);
		}
	}

	TEST_F(CMarketTimeTest, TestGetLastTradeDate) {
		time_t tUTC;
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			time(&tUTC);
			tUTC += i * 60 * 60 * 24;
			GetMarketTimeStruct(&tm2, tUTC, marketTime.GetMarketTimeZone());
			tm_ = tm2;
			marketTime.TEST_SetUTCTime(tUTC);
			marketTime.TEST_SetMarketTM(tm2);

			switch (tm_.tm_wday) {
			case 1: // ����һ
				tUTC -= 3 * 24 * 3600; //
				break;
			case 0: //������
				tUTC -= 3 * 24 * 3600; //
				break;
			case 6: // ������
				tUTC -= 2 * 24 * 3600; //
				break;
			default: // ����
				tUTC -= 24 * 3600; //
			}
			GetMarketTimeStruct(&tm_, tUTC, marketTime.GetMarketTimeZone());
			long LastTradeDate = marketTime.ConvertToDate(&tm_);
			EXPECT_EQ(marketTime.GetLastTradeDate(), LastTradeDate);
		}
	}

	TEST_F(CMarketTimeTest, TestIsWorkingDay) {
		const CTime time1(2019, 11, 25, 0, 0, 0); // ����Ϊ����һ
		EXPECT_TRUE(marketTime.IsWorkingDay(time1));
		EXPECT_TRUE(marketTime.IsWorkingDay(20191125));
		const CTime time2(2019, 11, 26, 0, 0, 0); // ����Ϊ���ڶ�
		EXPECT_TRUE(marketTime.IsWorkingDay(time2));
		EXPECT_TRUE(marketTime.IsWorkingDay(20191126));
		const CTime time3(2019, 11, 27, 0, 0, 0); // ����Ϊ������
		EXPECT_TRUE(marketTime.IsWorkingDay(time3));
		EXPECT_TRUE(marketTime.IsWorkingDay(20191127));
		const CTime time4(2019, 11, 28, 0, 0, 0); // ����Ϊ������
		EXPECT_TRUE(marketTime.IsWorkingDay(time4));
		EXPECT_TRUE(marketTime.IsWorkingDay(20191128));
		const CTime time5(2019, 11, 29, 0, 0, 0); // ����Ϊ������
		EXPECT_TRUE(marketTime.IsWorkingDay(time5));
		EXPECT_TRUE(marketTime.IsWorkingDay(20191129));
		const CTime time6(2019, 11, 30, 0, 0, 0); // ����Ϊ������
		EXPECT_FALSE(marketTime.IsWorkingDay(time6));
		EXPECT_FALSE(marketTime.IsWorkingDay(20191130));
		const CTime time7(2019, 12, 1, 0, 0, 0); // ����Ϊ������
		EXPECT_FALSE(marketTime.IsWorkingDay(time7));
		EXPECT_FALSE(marketTime.IsWorkingDay(20191201));
	}

	TEST_F(CMarketTimeTest, TestGetStringOfLocalTime) {
		marketTime.CalculateTime();

		time_t tUTC;
		time(&tUTC);
		tm tmLocal;
		char buffer[30];

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(marketTime.GetStringOfLocalTime(), str);
	}

	TEST_F(CMarketTimeTest, TestGetStringOfLocalDateTime) {
		marketTime.CalculateTime();

		const time_t tUTC = GetUTCTime();
		tm tmLocal;
		char buffer[100];

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%04d��%02d��%02d�� %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(marketTime.GetStringOfLocalDateTime(), str);
	}

	TEST_F(CMarketTimeTest, TestGetStringOfMarketTime) {
		marketTime.CalculateTime();

		tm tmMarket;
		GetMarketTimeStruct(&tmMarket, GetUTCTime(), marketTime.GetMarketTimeZone());
		char buffer[30];

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(marketTime.GetStringOfMarketTime(), str);
	}

	TEST_F(CMarketTimeTest, TestGetStringOfMarketDateTime) {
		marketTime.CalculateTime();

		tm tmMarket;
		char buffer[100];

		GetMarketTimeStruct(&tmMarket, GetUTCTime(), marketTime.GetMarketTimeZone());
		sprintf_s(buffer, _T("%04d��%02d��%02d�� %02d:%02d:%02d "), tmMarket.tm_year + 1900, tmMarket.tm_mon + 1, tmMarket.tm_mday, tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(marketTime.GetStringOfMarketDateTime(), str);
	}

	TEST_F(CMarketTimeTest, TestGetStringOfMarketDate) {
		char buffer[30];

		const long lDate = marketTime.GetMarketDate();
		const long year = lDate / 10000;
		const long month = lDate / 100 - year * 100;
		const long day = lDate - year * 10000 - month * 100;
		sprintf_s(buffer, _T("%4d��%2d��%2d��"), year, month, day);
		const CString str = buffer;
		EXPECT_STREQ(marketTime.GetStringOfMarketDate(), str);
	}

	TEST_F(CMarketTimeTest, TestGetDateOfWeek) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		time_t tUTC;
		tm tm_;
		time(&tUTC);
		GetMarketTimeStruct(&tm_, tUTC, marketTime.GetMarketTimeZone());

		marketTime.CalculateTime();
		EXPECT_EQ(marketTime.GetDayOfWeek(), tm_.tm_wday);
	}

	TEST_F(CMarketTimeTest, TestTransferToTime) {
		tm tm_{};
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 10;
		tm_.tm_min = 20;
		tm_.tm_sec = 30;
		tm tm_2 = tm_;
		const long lTime = marketTime.ConvertToTime(&tm_);
		const time_t tt = _mkgmtime(&tm_2);
		const long lTime2 = ConvertToTime(tt, 0); // UTCʱ��
		const long lTime3 = ConvertToTime(tt, gl_pChinaMarket->GetMarketTimeZone()); // ������ʱ��
		EXPECT_EQ(lTime, lTime2);
		EXPECT_EQ(lTime, 102030);
		EXPECT_EQ(lTime2, 102030);
		EXPECT_EQ(lTime3, 182030);
	}

	TEST_F(CMarketTimeTest, TestTransferToDate) {
		tm tm_{};
		tm_.tm_year = 2000 - 1900;
		tm_.tm_mon = 0;
		tm_.tm_mday = 5;
		tm_.tm_hour = 20; // GMTʱ��Ϊ20ʱ��������ʱ��Ϊ����4ʱ
		tm_.tm_min = 0;
		tm_.tm_sec = 0;
		tm tm_2 = tm_;
		const long lDate = marketTime.ConvertToDate(&tm_);
		const time_t tt = _mkgmtime(&tm_2);
		const long lDate2 = ConvertToDate(tt, 0); // UTCʱ��
		const long lDate3 = ConvertToDate(tt, gl_pChinaMarket->GetMarketTimeZone()); // ������ʱ��
		EXPECT_EQ(lDate, 20000105);
		EXPECT_EQ(lDate2, 20000105);
		EXPECT_EQ(lDate3, 20000106) << "������ʱ���UTCʱ����8Сʱ���ʶ���6����";
	}
}
