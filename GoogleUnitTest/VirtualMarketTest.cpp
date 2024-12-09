#include"pch.h"

 

#include"globedef.h"

#include "ChinaMarket.h"

#include"VirtualMarket.h"

#include"GeneralCheck.h"

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
		virtualMarket.AddTask(3, 10000);
		virtualMarket.AddTask(3, 20000);
		virtualMarket.AddTask(4, 10000);
		virtualMarket.AddTask(1, 10000);
		virtualMarket.AddTask(6, 1);
		virtualMarket.AddTask(5, 1);
		virtualMarket.AddTask(2, 10000);

		auto pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 1) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 1) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 3);
		EXPECT_EQ(pTask->GetTime(), 20000);
		virtualMarket.DiscardCurrentMarketTask();
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
		EXPECT_EQ(pTask->GetTime(), 350) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 34010) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 60000) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardCurrentMarketTask();
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime1) {
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime());
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime2) {
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(0));
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(100));
		EXPECT_TRUE(virtualMarket.IsOrdinaryTradeTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime1) {
		EXPECT_TRUE(virtualMarket.IsWorkingTime());
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime2) {
		EXPECT_TRUE(virtualMarket.IsWorkingTime(0));
		EXPECT_TRUE(virtualMarket.IsWorkingTime(100));
		EXPECT_TRUE(virtualMarket.IsWorkingTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime1) {
		EXPECT_FALSE(virtualMarket.IsDummyTime());
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime2) {
		EXPECT_FALSE(virtualMarket.IsDummyTime(0));
		EXPECT_FALSE(virtualMarket.IsDummyTime(100));
		EXPECT_FALSE(virtualMarket.IsDummyTime(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsTimeToResetSystem) {
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(0));
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(1000));
		EXPECT_FALSE(virtualMarket.IsTimeToResetSystem(virtualMarket.GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestMarketReady) {
		EXPECT_FALSE(virtualMarket.IsSystemReady()) << "市场基类默认为假";
		virtualMarket.SetSystemReady(false);
		EXPECT_FALSE(virtualMarket.IsSystemReady());
		virtualMarket.SetSystemReady(true);
		EXPECT_TRUE(virtualMarket.IsSystemReady());
	}

	TEST_F(CVirtualMarketTest, TestCalculateMarketTime) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		time_t tUTC = GetUTCTime();
		tm tm_, tmLocal;

		virtualMarket.CalculateTime();
		EXPECT_EQ(tUTC, GetUTCTime());
		localtime_s(&tmLocal, &tUTC);
		virtualMarket.GetMarketTimeStruct(&tm_, tUTC);
		long lTimeZone = 0;
		_get_timezone(&lTimeZone);
		virtualMarket.CalculateLastTradeDate();
		EXPECT_EQ(virtualMarket.GetDayOfWeek(), tm_.tm_wday);

		const long day = virtualMarket.ConvertToDate(&tm_);
		EXPECT_EQ(virtualMarket.GetMarketDate(), day);
		EXPECT_EQ(virtualMarket.GetMonthOfYear(), tm_.tm_mon + 1);
		EXPECT_EQ(virtualMarket.GetDateOfMonth(), tm_.tm_mday);
		EXPECT_EQ(virtualMarket.GetYear(), tm_.tm_year + 1900);

		const long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
		EXPECT_EQ(virtualMarket.GetMarketTime(), time);
		char buffer[30];
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		CString str = buffer;

		EXPECT_EQ(str.Compare(virtualMarket.GetStringOfLocalTime()), 0);

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tm_.tm_hour, tm_.tm_min, tm_.tm_sec);
		str = buffer;
		EXPECT_EQ(str.Compare(virtualMarket.GetStringOfMarketTime()), 0);
	}

	TEST_F(CVirtualMarketTest, TestCalculateLastTraddeDay) {
		time_t tMarket;
		auto tUTCTime = GetUTCTime();

		for (int i = 0; i < 7; i++) {
			TestSetUTCTime(tUTCTime + i * 24 * 3600);
			tm tmMarketTime2 = virtualMarket.GetMarketTime(GetUTCTime());

			switch (tmMarketTime2.tm_wday) {
			case 1: // 星期一
				tMarket = GetUTCTime() - 3 * 24 * 3600; // 上周五
				break;
			case 0: //星期日
				tMarket = GetUTCTime() - 3 * 24 * 3600; // 周四
				break;
			case 6: // 星期六
				tMarket = GetUTCTime() - 2 * 24 * 3600; // 周四
				break;
			default: // 其他
				tMarket = GetUTCTime() - 24 * 3600; // 上一日
			}
			const tm tmMarketTime = virtualMarket.GetMarketTime(tMarket);
			auto lMarketLastTradeDate = virtualMarket.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(virtualMarket.CalculateLastTradeDate(), lMarketLastTradeDate) << i;
		}

		// 恢复原状
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CVirtualMarketTest, TestCalculateCurrentTradeDay) {
		time_t tMarket;
		auto tUTCTime = GetUTCTime();
		for (int i = 0; i < 7; i++) {
			TestSetUTCTime(tUTCTime + i * 24 * 3600);
			tm tmMarketTime2 = virtualMarket.GetMarketTime(GetUTCTime());

			switch (tmMarketTime2.tm_wday) {
			case 0: //星期日
				tMarket = GetUTCTime() - 2 * 24 * 3600; // 周五
				break;
			case 6: // 星期六
				tMarket = GetUTCTime() - 1 * 24 * 3600; // 周五
				break;
			default: // 其他
				tMarket = GetUTCTime(); // 本日
			}
			const tm tmMarketTime = virtualMarket.GetMarketTime(tMarket);
			auto lMarketCurrentTradeDate = virtualMarket.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(virtualMarket.CalculateCurrentTradeDate(), lMarketCurrentTradeDate) << i;
		}

		// 恢复原状
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CVirtualMarketTest, TestCalculateNextTraddeDay) {
		time_t tMarket;
		auto tUTCTime = GetUTCTime();

		for (int i = 0; i < 7; i++) {
			TestSetUTCTime(tUTCTime + i * 24 * 3600);
			tm tmMarketTime2 = virtualMarket.GetMarketTime(GetUTCTime());

			switch (tmMarketTime2.tm_wday) {
			case 6: // 星期六
				tMarket = GetUTCTime() + 2 * 24 * 3600; // 下周一
				break;
			case 5: // 周五
				tMarket = GetUTCTime() + 3 * 24 * 3600; // 下周一
				break;
			default: // 其他
				tMarket = GetUTCTime() + 24 * 3600; // 次日
			}
			const tm tmMarketTime = virtualMarket.GetMarketTime(tMarket);
			auto lMarketNextTradeDate = virtualMarket.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(virtualMarket.CalculateNextTradeDate(), lMarketNextTradeDate) << i;
		}

		// 恢复原状
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CVirtualMarketTest, TestTransferToMarketTime) {
		tm tm2_;
		virtualMarket.CalculateTime();
		time_t tt = GetUTCTime();
		gmtime_s(&tm2_, &tt);
		const tm tm_ = virtualMarket.GetMarketTime(GetUTCTime());
		EXPECT_TRUE(tm_.tm_hour == tm2_.tm_hour) << "VirtualMarket默认为GMT标准时间";
	}

	TEST_F(CVirtualMarketTest, TestTransferToUTCTime1) {
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

		tTime = virtualMarket.TransferToUTCTime(&tmMarket); // 使用默认时间150000.
		tm2 = virtualMarket.GetMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 12);
		EXPECT_EQ(tm2.tm_min, 20);
		EXPECT_EQ(tm2.tm_sec, 30);

		tTime = virtualMarket.TransferToUTCTime(lMarketDate); // 使用默认时间150000.
		tm2 = virtualMarket.GetMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 15);
		EXPECT_EQ(tm2.tm_min, 00);
		EXPECT_EQ(tm2.tm_sec, 00);

		tTime = virtualMarket.TransferToUTCTime(lMarketDate, lMarketTime);
		tm3 = virtualMarket.GetMarketTime(tTime);
		EXPECT_EQ(tm3.tm_year, 100);
		EXPECT_EQ(tm3.tm_mon, 2);
		EXPECT_EQ(tm3.tm_mday, 1);
		EXPECT_EQ(tm3.tm_hour, 12);
		EXPECT_EQ(tm3.tm_min, 20);
		EXPECT_EQ(tm3.tm_sec, 30);
	}

	TEST_F(CVirtualMarketTest, TestTransferToMarketDate) {
		tm tmMarket;
		constexpr long lMarketDate = 20000301;
		constexpr long lMarketTime = 002030;

		tmMarket.tm_year = lMarketDate / 10000 - 1900;
		tmMarket.tm_mon = lMarketDate / 100 - (lMarketDate / 10000) * 100 - 1;
		tmMarket.tm_mday = lMarketDate - (lMarketDate / 100) * 100;
		tmMarket.tm_hour = lMarketTime / 10000;
		tmMarket.tm_min = lMarketTime / 100 - (lMarketTime / 10000) * 100;
		tmMarket.tm_sec = lMarketTime - (lMarketTime / 100) * 100;

		const time_t tTime = virtualMarket.TransferToUTCTime(&tmMarket);

		const long lMarketDate2 = virtualMarket.GetMarketDate(tTime);
		EXPECT_EQ(lMarketDate2, lMarketDate);
	}

	TEST_F(CVirtualMarketTest, TestGetNextTradeDate) {
		time_t tUTC = GetUTCTime();
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			tUTC += i * 60 * 60 * 24;
			virtualMarket.GetMarketTimeStruct(&tm2, tUTC);
			tm_ = tm2;
			TestSetUTCTime(tUTC);
			virtualMarket.TEST_SetMarketTM(tm2);

			switch (tm_.tm_wday) {
			case 6: //星期六
				tUTC += 2 * 24 * 3600; //
				break;
			case 5: // 星期五
				tUTC += 3 * 24 * 3600; //
				break;
			default: // 其他
				tUTC += 24 * 3600;
				break;
			}
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC);
			long nextTradeDate = virtualMarket.ConvertToDate(&tm_);
			EXPECT_EQ(virtualMarket.GetNextTradeDate(), nextTradeDate);
		}
	}

	TEST_F(CVirtualMarketTest, TestGetCurrentTradeDate) {
		time_t tUTC = GetUTCTime();
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			tUTC += i * 60 * 60 * 24;
			virtualMarket.GetMarketTimeStruct(&tm2, tUTC);
			tm_ = tm2;
			TestSetUTCTime(tUTC);
			virtualMarket.TEST_SetMarketTM(tm2);

			switch (tm_.tm_wday) {
			case 0: //星期日
				tUTC -= 2 * 24 * 3600; //
				break;
			case 6: // 星期六
				tUTC -= 1 * 24 * 3600; //
				break;
			default: // 其他
				break;
			}
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC);
			long currentTradeDate = virtualMarket.ConvertToDate(&tm_);
			EXPECT_EQ(virtualMarket.GetCurrentTradeDate(), currentTradeDate);
		}
	}

	TEST_F(CVirtualMarketTest, TestGetLastTradeDate) {
		time_t tUTC = GetUTCTime();
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			tUTC += i * 60 * 60 * 24;
			virtualMarket.GetMarketTimeStruct(&tm2, tUTC);
			tm_ = tm2;
			TestSetUTCTime(tUTC);
			virtualMarket.TEST_SetMarketTM(tm2);

			switch (tm_.tm_wday) {
			case 1: // 星期一
				tUTC -= 3 * 24 * 3600; //
				break;
			case 0: //星期日
				tUTC -= 3 * 24 * 3600; //
				break;
			case 6: // 星期六
				tUTC -= 2 * 24 * 3600; //
				break;
			default: // 其他
				tUTC -= 24 * 3600; //
			}
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC);
			long LastTradeDate = virtualMarket.ConvertToDate(&tm_);
			EXPECT_EQ(virtualMarket.GetLastTradeDate(), LastTradeDate);
		}
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingDay) {
		const CTime time1(2019, 11, 25, 0, 0, 0); // 此日为星期一
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time1));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191125));
		const CTime time2(2019, 11, 26, 0, 0, 0); // 此日为星期二
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time2));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191126));
		const CTime time3(2019, 11, 27, 0, 0, 0); // 此日为星期三
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time3));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191127));
		const CTime time4(2019, 11, 28, 0, 0, 0); // 此日为星期四
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time4));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191128));
		const CTime time5(2019, 11, 29, 0, 0, 0); // 此日为星期五
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time5));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191129));
		const CTime time6(2019, 11, 30, 0, 0, 0); // 此日为星期六
		EXPECT_FALSE(virtualMarket.IsWorkingDay(time6));
		EXPECT_FALSE(virtualMarket.IsWorkingDay(20191130));
		const CTime time7(2019, 12, 1, 0, 0, 0); // 此日为星期日
		EXPECT_FALSE(virtualMarket.IsWorkingDay(time7));
		EXPECT_FALSE(virtualMarket.IsWorkingDay(20191201));
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalTime) {
		virtualMarket.CalculateTime();

		time_t tUTC = GetUTCTime();
		tm tmLocal;
		char buffer[30];

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(virtualMarket.GetStringOfLocalTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalDateTime) {
		virtualMarket.CalculateTime();

		const time_t tUTC = GetUTCTime();
		tm tmLocal;
		char buffer[100];

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(virtualMarket.GetStringOfLocalDateTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketTime) {
		virtualMarket.CalculateTime();

		tm tmMarket;
		virtualMarket.GetMarketTimeStruct(&tmMarket, GetUTCTime());
		char buffer[30];

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(virtualMarket.GetStringOfMarketTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDateTime) {
		virtualMarket.CalculateTime();

		tm tmMarket;
		char buffer[100];

		virtualMarket.GetMarketTimeStruct(&tmMarket, GetUTCTime());
		sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmMarket.tm_year + 1900, tmMarket.tm_mon + 1, tmMarket.tm_mday, tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(virtualMarket.GetStringOfMarketDateTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDate) {
		char buffer[30];

		const long lDate = virtualMarket.GetMarketDate();
		const long year = lDate / 10000;
		const long month = lDate / 100 - year * 100;
		const long day = lDate - year * 10000 - month * 100;
		sprintf_s(buffer, _T("%4d年%2d月%2d日"), year, month, day);
		const CString str = buffer;
		EXPECT_STREQ(virtualMarket.GetStringOfMarketDate(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetDateOfWeek) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		time_t tUTC = GetUTCTime();
		tm tm_;
		virtualMarket.GetMarketTimeStruct(&tm_, tUTC);

		virtualMarket.CalculateTime();
		EXPECT_EQ(virtualMarket.GetDayOfWeek(), tm_.tm_wday);
	}

	TEST_F(CVirtualMarketTest, TestUpdateMarketInfo) {
		EXPECT_TRUE(virtualMarket.UpdateMarketInfo());
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
}
