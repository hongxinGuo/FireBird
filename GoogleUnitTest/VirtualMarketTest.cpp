#include"pch.h"

#include"TimeConvert.h"

#include"VirtualMarket.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class CVirtualMarketTest : public testing::Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
			virtualMarket.SetResetMarketPermission(true);
			virtualMarket.SetReadyToRun(true);
			virtualMarket.SetResetMarket(true);
		}

		void TearDown() override {
			// clearUp
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
		virtualMarket.DiscardMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 1) << "相同时间的任务，随机排列";
		virtualMarket.DiscardMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "相同时间的任务，随机排列";
		virtualMarket.DiscardMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 3);
		EXPECT_EQ(pTask->GetTime(), 20000);
		virtualMarket.DiscardMarketTask();
	}

	TEST_F(CVirtualMarketTest, TestRectifyTaskTime1) {
		auto pTask = make_shared<CMarketTask>();
		pTask->SetTime(20);
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(30);
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(240510);
		virtualMarket.AddTask(pTask);

		virtualMarket.RectifyTaskTime();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 20);
	}

	TEST_F(CVirtualMarketTest, TestRectifyTaskTime2) {
		auto pTask = make_shared<CMarketTask>();
		pTask->SetTime(240350);
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(300000);
		virtualMarket.AddTask(pTask);
		pTask = make_shared<CMarketTask>();
		pTask->SetTime(274010);
		virtualMarket.AddTask(pTask);

		virtualMarket.RectifyTaskTime();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 350) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 34010) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 60000) << "所有的时间皆大于240000，故而皆减去240000";
		virtualMarket.DiscardMarketTask();
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

	TEST_F(CVirtualMarketTest, TestIsReadyToRun) {
		EXPECT_TRUE(virtualMarket.IsReadyToRun());
		virtualMarket.SetReadyToRun(false);
		EXPECT_FALSE(virtualMarket.IsReadyToRun());
		virtualMarket.SetReadyToRun(true);
		EXPECT_TRUE(virtualMarket.IsReadyToRun());
	}

	TEST_F(CVirtualMarketTest, TestIsPermitResetMarket) {
		EXPECT_TRUE(virtualMarket.HaveResetMarketPermission()) << "PermitResetMarket should be true\n";
		virtualMarket.SetResetMarketPermission(false);
		EXPECT_FALSE(virtualMarket.HaveResetMarketPermission());
		virtualMarket.SetResetMarketPermission(true);
		EXPECT_TRUE(virtualMarket.HaveResetMarketPermission());
	}

	TEST_F(CVirtualMarketTest, TestMarketReady) {
		EXPECT_FALSE(virtualMarket.IsSystemReady()) << "市场基类默认为假";
		virtualMarket.SetSystemReady(false);
		EXPECT_FALSE(virtualMarket.IsSystemReady());
		virtualMarket.SetSystemReady(true);
		EXPECT_TRUE(virtualMarket.IsSystemReady());
	}

	TEST_F(CVirtualMarketTest, TestCalculateMarketTime) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		time_t tUTC;
		tm tm_, tmLocal;

		time(&tUTC);
		virtualMarket.CalculateTime();
		EXPECT_EQ(tUTC, GetUTCTime());
		localtime_s(&tmLocal, &tUTC);
		GetMarketTimeStruct(&tm_, tUTC, virtualMarket.GetMarketTimeZone());
		long lTimeZone = 0;
		_get_timezone(&lTimeZone);
		virtualMarket.CalculateLastTradeDate();
		EXPECT_EQ(virtualMarket.GetDayOfWeek(), tm_.tm_wday);

		const long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
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
		GetMarketTimeStruct(&tm_, tUTC, virtualMarket.GetMarketTimeZone());
		const long LastTradeDate = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
		EXPECT_EQ(virtualMarket.GetLastTradeDate(), LastTradeDate);
	}

	TEST_F(CVirtualMarketTest, TestTransferToMarketTime) {
		tm tm2_;
		time_t tt;

		virtualMarket.CalculateTime();
		time(&tt);
		gmtime_s(&tm2_, &tt);
		const tm tm_ = virtualMarket.TransferToMarketTime();
		EXPECT_TRUE((tm_.tm_hour == (tm2_.tm_hour + 8) || (tm_.tm_hour == tm2_.tm_hour - 16))) << "VirtualMarket默认为东八区";
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
		tm2 = virtualMarket.TransferToMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 12);
		EXPECT_EQ(tm2.tm_min, 20);
		EXPECT_EQ(tm2.tm_sec, 30);

		tTime = virtualMarket.TransferToUTCTime(lMarketDate); // 使用默认时间150000.
		tm2 = virtualMarket.TransferToMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 15);
		EXPECT_EQ(tm2.tm_min, 00);
		EXPECT_EQ(tm2.tm_sec, 00);

		tTime = virtualMarket.TransferToUTCTime(lMarketDate, lMarketTime);
		tm3 = virtualMarket.TransferToMarketTime(tTime);
		EXPECT_EQ(tm3.tm_year, 100);
		EXPECT_EQ(tm3.tm_mon, 2);
		EXPECT_EQ(tm3.tm_mday, 1);
		EXPECT_EQ(tm3.tm_hour, 12);
		EXPECT_EQ(tm3.tm_min, 20);
		EXPECT_EQ(tm3.tm_sec, 30);
	}

	TEST_F(CVirtualMarketTest, TestTransferToMarketDate) {
		tm tmMarket;
		const long lMarketDate = 20000301;
		const long lMarketTime = 002030;

		tmMarket.tm_year = lMarketDate / 10000 - 1900;
		tmMarket.tm_mon = lMarketDate / 100 - (lMarketDate / 10000) * 100 - 1;
		tmMarket.tm_mday = lMarketDate - (lMarketDate / 100) * 100;
		tmMarket.tm_hour = lMarketTime / 10000;
		tmMarket.tm_min = lMarketTime / 100 - (lMarketTime / 10000) * 100;
		tmMarket.tm_sec = lMarketTime - (lMarketTime / 100) * 100;

		const time_t tTime = virtualMarket.TransferToUTCTime(&tmMarket);

		const long lMarketDate2 = virtualMarket.TransferToMarketDate(tTime);
		EXPECT_EQ(lMarketDate2, lMarketDate);
	}

	TEST_F(CVirtualMarketTest, TestGetLastTradeDate) {
		time_t tUTC;
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			time(&tUTC);
			tUTC += i * 60 * 60 * 24;
			GetMarketTimeStruct(&tm2, tUTC, virtualMarket.GetMarketTimeZone());
			tm_ = tm2;
			virtualMarket.TEST_SetUTCTime(tUTC);
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
			GetMarketTimeStruct(&tm_, tUTC, virtualMarket.GetMarketTimeZone());
			long LastTradeDate = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
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

		time_t tUTC;
		time(&tUTC);
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
		GetMarketTimeStruct(&tmMarket, GetUTCTime(), virtualMarket.GetMarketTimeZone());
		char buffer[30];

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		CString str = buffer;
		EXPECT_STREQ(virtualMarket.GetStringOfMarketTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDateTime) {
		virtualMarket.CalculateTime();

		tm tmMarket;
		char buffer[100];

		GetMarketTimeStruct(&tmMarket, GetUTCTime(), virtualMarket.GetMarketTimeZone());
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
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		time_t tUTC;
		tm tm_;
		time(&tUTC);
		GetMarketTimeStruct(&tm_, tUTC, virtualMarket.GetMarketTimeZone());

		virtualMarket.CalculateTime();
		EXPECT_EQ(virtualMarket.GetDayOfWeek(), tm_.tm_wday);
	}

	TEST_F(CVirtualMarketTest, TestPreparingExitMarket) {
		EXPECT_TRUE(virtualMarket.PreparingExitMarket()) << "简单返回";
	}

	TEST_F(CVirtualMarketTest, TestTaskResetMarketFlagAtMidnight) {
		// 这个其实是测试的CVirtualMarket类中的函数。
		EXPECT_TRUE(virtualMarket.HaveResetMarketPermission());
		virtualMarket.SetResetMarketPermission(false);
		virtualMarket.ResetMarketFlagAtMidnight(0);
		EXPECT_TRUE(virtualMarket.HaveResetMarketPermission());
		virtualMarket.SetResetMarketPermission(false);
		virtualMarket.ResetMarketFlagAtMidnight(101);
		EXPECT_FALSE(virtualMarket.HaveResetMarketPermission());
		virtualMarket.ResetMarketFlagAtMidnight(100);
		EXPECT_TRUE(virtualMarket.HaveResetMarketPermission());

		EXPECT_THAT(gl_systemMessage.InformationSize(), 2);
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CVirtualMarketTest, TestUpdateMarketInfo) {
		EXPECT_TRUE(virtualMarket.UpdateMarketInfo());
	}
}
