#include"pch.h"

#include"TimeConvert.h"

#include"VirtualMarket.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	CVirtualMarketPtr gl_pVirtualMarket;

	class CVirtualMarketTest : public testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			gl_pVirtualMarket = make_shared<CVirtualMarket>();
			//gl_pVirtualMarket->ResetMarket(); // 不要这么做
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
			gl_pVirtualMarket->SetResetMarketPermission(true);
			gl_pVirtualMarket->SetReadyToRun(true);
			gl_pVirtualMarket->SetResetMarket(true);
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CVirtualMarketTest, TestAddTask) {
		gl_pVirtualMarket->AddTask(3, 10000);
		gl_pVirtualMarket->AddTask(3, 20000);
		gl_pVirtualMarket->AddTask(4, 10000);
		gl_pVirtualMarket->AddTask(1, 10000);
		gl_pVirtualMarket->AddTask(6, 1);
		gl_pVirtualMarket->AddTask(5, 1);
		gl_pVirtualMarket->AddTask(2, 10000);

		auto pTask = gl_pVirtualMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 6) << "相同时间的任务，随机排列";
		EXPECT_EQ(pTask->GetTime(), 1);
		gl_pVirtualMarket->DiscardMarketTask();

		pTask = gl_pVirtualMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 5) << "相同时间的任务，随机排列";
		EXPECT_EQ(pTask->GetTime(), 1);
		gl_pVirtualMarket->DiscardMarketTask();

		pTask = gl_pVirtualMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 4) << "相同时间的任务，随机排列";
		EXPECT_EQ(pTask->GetTime(), 10000);
		gl_pVirtualMarket->DiscardMarketTask();

		pTask = gl_pVirtualMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 2) << "相同时间的任务，随机排列";
		EXPECT_EQ(pTask->GetTime(), 10000);
		gl_pVirtualMarket->DiscardMarketTask();

		pTask = gl_pVirtualMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 1) << "相同时间的任务，随机排列";
		EXPECT_EQ(pTask->GetTime(), 10000);
		gl_pVirtualMarket->DiscardMarketTask();

		pTask = gl_pVirtualMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 3) << "相同时间的任务，随机排列";
		EXPECT_EQ(pTask->GetTime(), 10000);
		gl_pVirtualMarket->DiscardMarketTask();

		pTask = gl_pVirtualMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), 3);
		EXPECT_EQ(pTask->GetTime(), 20000);
		gl_pVirtualMarket->DiscardMarketTask();
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime1) {
		EXPECT_TRUE(gl_pVirtualMarket->IsOrdinaryTradeTime());
	}

	TEST_F(CVirtualMarketTest, TestIsOrdinaryTradeTime2) {
		EXPECT_TRUE(gl_pVirtualMarket->IsOrdinaryTradeTime(0));
		EXPECT_TRUE(gl_pVirtualMarket->IsOrdinaryTradeTime(100));
		EXPECT_TRUE(gl_pVirtualMarket->IsOrdinaryTradeTime(gl_pVirtualMarket->GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime1) {
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingTime());
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingTime2) {
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingTime(0));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingTime(100));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingTime(gl_pVirtualMarket->GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime1) {
		EXPECT_FALSE(gl_pVirtualMarket->IsDummyTime());
	}

	TEST_F(CVirtualMarketTest, TestIsDummyTime2) {
		EXPECT_FALSE(gl_pVirtualMarket->IsDummyTime(0));
		EXPECT_FALSE(gl_pVirtualMarket->IsDummyTime(100));
		EXPECT_FALSE(gl_pVirtualMarket->IsDummyTime(gl_pVirtualMarket->GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsTimeToResetSystem) {
		EXPECT_FALSE(gl_pVirtualMarket->IsTimeToResetSystem(0));
		EXPECT_FALSE(gl_pVirtualMarket->IsTimeToResetSystem(1000));
		EXPECT_FALSE(gl_pVirtualMarket->IsTimeToResetSystem(gl_pVirtualMarket->GetMarketTime()));
	}

	TEST_F(CVirtualMarketTest, TestIsReadyToRun) {
		EXPECT_TRUE(gl_pVirtualMarket->IsReadyToRun());
		gl_pVirtualMarket->SetReadyToRun(false);
		EXPECT_FALSE(gl_pVirtualMarket->IsReadyToRun());
		gl_pVirtualMarket->SetReadyToRun(true);
		EXPECT_TRUE(gl_pVirtualMarket->IsReadyToRun());
	}

	TEST_F(CVirtualMarketTest, TestIsPermitResetMarket) {
		EXPECT_TRUE(gl_pVirtualMarket->HaveResetMarketPermission()) << "PermitResetMarket should be true\n";
		gl_pVirtualMarket->SetResetMarketPermission(false);
		EXPECT_FALSE(gl_pVirtualMarket->HaveResetMarketPermission());
		gl_pVirtualMarket->SetResetMarketPermission(true);
		EXPECT_TRUE(gl_pVirtualMarket->HaveResetMarketPermission());
	}

	TEST_F(CVirtualMarketTest, TestMarketReady) {
		EXPECT_TRUE(gl_pVirtualMarket->IsSystemReady()) << "市场基类默认为真";
		gl_pVirtualMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pVirtualMarket->IsSystemReady());
		gl_pVirtualMarket->SetSystemReady(false);
		EXPECT_FALSE(gl_pVirtualMarket->IsSystemReady());

		gl_pVirtualMarket->SetSystemReady(true); // 恢复原态
	}

	TEST_F(CVirtualMarketTest, TestCalculateMarketTime) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		time_t tUTC;
		tm tm_, tmLocal;

		time(&tUTC);
		gl_pVirtualMarket->CalculateTime();
		EXPECT_EQ(tUTC, GetUTCTime());
		localtime_s(&tmLocal, &tUTC);
		GetMarketTimeStruct(&tm_, tUTC, gl_pVirtualMarket->GetMarketTimeZone());
		long lTimeZone = 0;
		_get_timezone(&lTimeZone);
		gl_pVirtualMarket->CalculateLastTradeDate();
		EXPECT_EQ(gl_pVirtualMarket->GetDayOfWeek(), tm_.tm_wday);

		const long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
		EXPECT_EQ(gl_pVirtualMarket->GetMarketDate(), day);
		EXPECT_EQ(gl_pVirtualMarket->GetMonthOfYear(), tm_.tm_mon + 1);
		EXPECT_EQ(gl_pVirtualMarket->GetDateOfMonth(), tm_.tm_mday);
		EXPECT_EQ(gl_pVirtualMarket->GetYear(), tm_.tm_year + 1900);

		const long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
		EXPECT_EQ(gl_pVirtualMarket->GetMarketTime(), time);
		char buffer[30];
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		CString str = buffer;

		EXPECT_EQ(str.Compare(gl_pVirtualMarket->GetStringOfLocalTime()), 0);

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tm_.tm_hour, tm_.tm_min, tm_.tm_sec);
		str = buffer;
		EXPECT_EQ(str.Compare(gl_pVirtualMarket->GetStringOfMarketTime()), 0);

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
		GetMarketTimeStruct(&tm_, tUTC, gl_pVirtualMarket->GetMarketTimeZone());
		const long LastTradeDate = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
		EXPECT_EQ(gl_pVirtualMarket->GetLastTradeDate(), LastTradeDate);
	}

	TEST_F(CVirtualMarketTest, TestTransferToMarketTime) {
		tm tm2_;
		time_t tt;

		gl_pVirtualMarket->CalculateTime();
		time(&tt);
		gmtime_s(&tm2_, &tt);
		const tm tm_ = gl_pVirtualMarket->TransferToMarketTime();
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

		tTime = gl_pVirtualMarket->TransferToUTCTime(&tmMarket); // 使用默认时间150000.
		tm2 = gl_pVirtualMarket->TransferToMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 12);
		EXPECT_EQ(tm2.tm_min, 20);
		EXPECT_EQ(tm2.tm_sec, 30);

		tTime = gl_pVirtualMarket->TransferToUTCTime(lMarketDate); // 使用默认时间150000.
		tm2 = gl_pVirtualMarket->TransferToMarketTime(tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 15);
		EXPECT_EQ(tm2.tm_min, 00);
		EXPECT_EQ(tm2.tm_sec, 00);

		tTime = gl_pVirtualMarket->TransferToUTCTime(lMarketDate, lMarketTime);
		tm3 = gl_pVirtualMarket->TransferToMarketTime(tTime);
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

		const time_t tTime = gl_pVirtualMarket->TransferToUTCTime(&tmMarket);

		const long lMarketDate2 = gl_pVirtualMarket->TransferToMarketDate(tTime);
		EXPECT_EQ(lMarketDate2, lMarketDate);
	}

	TEST_F(CVirtualMarketTest, TestGetLastTradeDate) {
		time_t tUTC;
		tm tm_, tm2;

		for (int i = 0; i < 7; i++) {
			time(&tUTC);
			tUTC += i * 60 * 60 * 24;
			GetMarketTimeStruct(&tm2, tUTC, gl_pVirtualMarket->GetMarketTimeZone());
			tm_ = tm2;
			gl_pVirtualMarket->TEST_SetUTCTime(tUTC);
			gl_pVirtualMarket->TEST_SetMarketTM(tm2);

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
			GetMarketTimeStruct(&tm_, tUTC, gl_pVirtualMarket->GetMarketTimeZone());
			long LastTradeDate = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
			EXPECT_EQ(gl_pVirtualMarket->GetLastTradeDate(), LastTradeDate);
		}
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingDay) {
		const CTime time1(2019, 11, 25, 0, 0, 0); // 此日为星期一
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time1));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191125));
		const CTime time2(2019, 11, 26, 0, 0, 0); // 此日为星期二
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time2));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191126));
		const CTime time3(2019, 11, 27, 0, 0, 0); // 此日为星期三
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time3));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191127));
		const CTime time4(2019, 11, 28, 0, 0, 0); // 此日为星期四
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time4));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191128));
		const CTime time5(2019, 11, 29, 0, 0, 0); // 此日为星期五
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time5));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191129));
		const CTime time6(2019, 11, 30, 0, 0, 0); // 此日为星期六
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(time6));
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(20191130));
		const CTime time7(2019, 12, 1, 0, 0, 0); // 此日为星期日
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(time7));
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(20191201));
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalTime) {
		gl_pVirtualMarket->CalculateTime();

		time_t tUTC;
		time(&tUTC);
		tm tmLocal;
		char buffer[30];

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfLocalTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalDateTime) {
		gl_pVirtualMarket->CalculateTime();

		const time_t tUTC = GetUTCTime();
		tm tmLocal;
		char buffer[100];

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfLocalDateTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketTime) {
		gl_pVirtualMarket->CalculateTime();

		tm tmMarket;
		GetMarketTimeStruct(&tmMarket, GetUTCTime(), gl_pVirtualMarket->GetMarketTimeZone());
		char buffer[30];

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		CString str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfMarketTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDateTime) {
		gl_pVirtualMarket->CalculateTime();

		tm tmMarket;
		char buffer[100];

		GetMarketTimeStruct(&tmMarket, GetUTCTime(), gl_pVirtualMarket->GetMarketTimeZone());
		sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmMarket.tm_year + 1900, tmMarket.tm_mon + 1, tmMarket.tm_mday, tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		const CString str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfMarketDateTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDate) {
		char buffer[30];

		const long lDate = gl_pVirtualMarket->GetMarketDate();
		const long year = lDate / 10000;
		const long month = lDate / 100 - year * 100;
		const long day = lDate - year * 10000 - month * 100;
		sprintf_s(buffer, _T("%4d年%2d月%2d日"), year, month, day);
		const CString str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfMarketDate(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetDateOfWeek) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		time_t tUTC;
		tm tm_;
		time(&tUTC);
		GetMarketTimeStruct(&tm_, tUTC, gl_pVirtualMarket->GetMarketTimeZone());

		gl_pVirtualMarket->CalculateTime();
		EXPECT_EQ(gl_pVirtualMarket->GetDayOfWeek(), tm_.tm_wday);
	}

	TEST_F(CVirtualMarketTest, TestPreparingExitMarket) {
		EXPECT_TRUE(gl_pVirtualMarket->PreparingExitMarket()) << "简单返回";
	}

	TEST_F(CVirtualMarketTest, TestTaskResetMarketFlagAtMidnight) {
		// 这个其实是测试的CVirtualMarket类中的函数。
		EXPECT_TRUE(gl_pVirtualMarket->HaveResetMarketPermission());
		gl_pVirtualMarket->SetResetMarketPermission(false);
		gl_pVirtualMarket->ResetMarketFlagAtMidnight(0);
		EXPECT_TRUE(gl_pVirtualMarket->HaveResetMarketPermission());
		gl_pVirtualMarket->SetResetMarketPermission(false);
		gl_pVirtualMarket->ResetMarketFlagAtMidnight(101);
		EXPECT_FALSE(gl_pVirtualMarket->HaveResetMarketPermission());
		gl_pVirtualMarket->ResetMarketFlagAtMidnight(100);
		EXPECT_TRUE(gl_pVirtualMarket->HaveResetMarketPermission());

		EXPECT_THAT(gl_systemMessage.InformationSize(), 2);
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CVirtualMarketTest, TestUpdateMarketInfo) {
		EXPECT_TRUE(gl_pVirtualMarket->UpdateMarketInfo());
	}
}
