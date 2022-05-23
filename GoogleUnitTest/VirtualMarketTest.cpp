#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"
#include"Accessory.h"
#include"VirtualMarket.h"

#include"GeneralCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	CVirtualMarketPtr gl_pVirtualMarket;

	class CVirtualMarketTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			gl_pVirtualMarket = make_shared<CVirtualMarket>();
			//gl_pVirtualMarket->ResetMarket(); // 不要这么做
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
			gl_pVirtualMarket->SetPermitResetMarket(true);
			gl_pVirtualMarket->SetReadyToRun(true);
			gl_pVirtualMarket->SetResetMarket(true);
		}

		virtual void TearDown(void) override {
			// clearup
			GeneralCheck();
		}
	};

	TEST_F(CVirtualMarketTest, TestIsReadyToRun) {
		EXPECT_TRUE(gl_pVirtualMarket->IsReadyToRun());
		gl_pVirtualMarket->SetReadyToRun(false);
		EXPECT_FALSE(gl_pVirtualMarket->IsReadyToRun());
		gl_pVirtualMarket->SetReadyToRun(true);
		EXPECT_TRUE(gl_pVirtualMarket->IsReadyToRun());
	}

	TEST_F(CVirtualMarketTest, TestIsPermitResetMarket) {
		EXPECT_TRUE(gl_pVirtualMarket->IsPermitResetMarket()) << "PermitResetMarket should be true\n";
		gl_pVirtualMarket->SetPermitResetMarket(false);
		EXPECT_FALSE(gl_pVirtualMarket->IsPermitResetMarket());
		gl_pVirtualMarket->SetPermitResetMarket(true);
		EXPECT_TRUE(gl_pVirtualMarket->IsPermitResetMarket());
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
		ASSERT_FALSE(gl_fNormalMode);
		time_t tUTC;
		tm tm_, tmLocal;

		time(&tUTC);
		gl_pVirtualMarket->CalculateTime();
		EXPECT_EQ(tUTC, gl_pVirtualMarket->GetUTCTime());
		localtime_s(&tmLocal, &tUTC);
		GetMarketTimeStruct(&tm_, tUTC, gl_pVirtualMarket->GetMarketTimeZone());
		long lTimeZone = 0;
		_get_timezone(&lTimeZone);
		gl_pVirtualMarket->CalculateLastTradeDate();
		EXPECT_EQ(gl_pVirtualMarket->GetDayOfWeek(), tm_.tm_wday);

		long day = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
		EXPECT_EQ(gl_pVirtualMarket->GetMarketDate(), day);
		EXPECT_EQ(gl_pVirtualMarket->GetMonthOfYear(), tm_.tm_mon + 1);
		EXPECT_EQ(gl_pVirtualMarket->GetDateOfMonth(), tm_.tm_mday);
		EXPECT_EQ(gl_pVirtualMarket->GetYear(), tm_.tm_year + 1900);

		long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
		EXPECT_EQ(gl_pVirtualMarket->GetMarketTime(), time);
		char buffer[30];
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		CString str;
		str = buffer;

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
		long LastTradeDate = (tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday;
		EXPECT_EQ(gl_pVirtualMarket->GetLastTradeDate(), LastTradeDate);
	}

	TEST_F(CVirtualMarketTest, TestTransferToMarketTime) {
		tm tm_, tm2_;
		time_t tt;

		gl_pVirtualMarket->CalculateTime();
		time(&tt);
		gmtime_s(&tm2_, &tt);
		tm_ = gl_pVirtualMarket->TransferToMarketTime();
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
		long lMarketDate = 20000301, lMarketDate2 = 0;
		long lMarketTime = 002030;
		time_t tTime = 0;

		tmMarket.tm_year = lMarketDate / 10000 - 1900;
		tmMarket.tm_mon = lMarketDate / 100 - (lMarketDate / 10000) * 100 - 1;
		tmMarket.tm_mday = lMarketDate - (lMarketDate / 100) * 100;
		tmMarket.tm_hour = lMarketTime / 10000;
		tmMarket.tm_min = lMarketTime / 100 - (lMarketTime / 10000) * 100;
		tmMarket.tm_sec = lMarketTime - (lMarketTime / 100) * 100;

		tTime = gl_pVirtualMarket->TransferToUTCTime(&tmMarket);

		lMarketDate2 = gl_pVirtualMarket->TransferToMarketDate(tTime);
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
			gl_pVirtualMarket->__TEST_SetUTCTime(tUTC);
			gl_pVirtualMarket->__TEST_SetMarketTM(tm2);

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
		CTime time1(2019, 11, 25, 0, 0, 0); // 此日为星期一
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time1));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191125));
		CTime time2(2019, 11, 26, 0, 0, 0); // 此日为星期二
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time2));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191126));
		CTime time3(2019, 11, 27, 0, 0, 0); // 此日为星期三
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time3));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191127));
		CTime time4(2019, 11, 28, 0, 0, 0); // 此日为星期四
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time4));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191128));
		CTime time5(2019, 11, 29, 0, 0, 0); // 此日为星期五
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(time5));
		EXPECT_TRUE(gl_pVirtualMarket->IsWorkingDay(20191129));
		CTime time6(2019, 11, 30, 0, 0, 0); // 此日为星期六
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(time6));
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(20191130));
		CTime time7(2019, 12, 1, 0, 0, 0); // 此日为星期日
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(time7));
		EXPECT_FALSE(gl_pVirtualMarket->IsWorkingDay(20191201));
	}

	TEST_F(CVirtualMarketTest, TestIsEarlyThen) {
		EXPECT_TRUE(gl_pVirtualMarket->IsEarlyThen(20200101, 20200115, 13));
		EXPECT_FALSE(gl_pVirtualMarket->IsEarlyThen(20200101, 20200115, 14));
		EXPECT_TRUE(gl_pVirtualMarket->IsEarlyThen(20200115, 20200201, 16));
		EXPECT_FALSE(gl_pVirtualMarket->IsEarlyThen(20200115, 20200201, 17));
		EXPECT_TRUE(gl_pVirtualMarket->IsEarlyThen(20191101, 20200115, 74));
		EXPECT_FALSE(gl_pVirtualMarket->IsEarlyThen(20191101, 20200115, 75));
	}

	TEST_F(CVirtualMarketTest, TestGetNextDate) {
		EXPECT_EQ(gl_pVirtualMarket->GetNextDay(20200101, 1), 20200102);
		EXPECT_EQ(gl_pVirtualMarket->GetNextDay(20200101, 11), 20200112);
		EXPECT_EQ(gl_pVirtualMarket->GetNextDay(20201231, 1), 20210101);
		EXPECT_EQ(gl_pVirtualMarket->GetNextDay(20201221, 14), 20210104);
	}

	TEST_F(CVirtualMarketTest, TestGetPrevDay) {
		EXPECT_EQ(gl_pVirtualMarket->GetPrevDay(20200102), 20200101);
		EXPECT_EQ(gl_pVirtualMarket->GetPrevDay(20200112, 10), 20200102);
		EXPECT_EQ(gl_pVirtualMarket->GetPrevDay(20200102, 11), 20191222);
		EXPECT_EQ(gl_pVirtualMarket->GetPrevDay(20200202, 11), 20200122);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalTime) {
		gl_pVirtualMarket->CalculateTime();

		time_t tUTC;
		time(&tUTC);
		tm tmLocal;
		char buffer[30];
		CString str;

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfLocalTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalDateTime) {
		gl_pVirtualMarket->CalculateTime();

		time_t tUTC = gl_pVirtualMarket->GetUTCTime();
		tm tmLocal;
		char buffer[100];
		CString str;

		localtime_s(&tmLocal, &tUTC);
		sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfLocalDateTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketTime) {
		gl_pVirtualMarket->CalculateTime();

		tm tmMarket;
		GetMarketTimeStruct(&tmMarket, gl_pVirtualMarket->GetUTCTime(), gl_pVirtualMarket->GetMarketTimeZone());
		char buffer[30];
		CString str;

		sprintf_s(buffer, _T("%02d:%02d:%02d "), tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfMarketTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDateTime) {
		gl_pVirtualMarket->CalculateTime();

		tm tmMarket;
		char buffer[100];
		CString str;

		GetMarketTimeStruct(&tmMarket, gl_pVirtualMarket->GetUTCTime(), gl_pVirtualMarket->GetMarketTimeZone());
		sprintf_s(buffer, _T("%04d年%02d月%02d日 %02d:%02d:%02d "), tmMarket.tm_year + 1900, tmMarket.tm_mon + 1, tmMarket.tm_mday, tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfMarketDateTime(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDate) {
		char buffer[30];
		CString str;

		long lDate = gl_pVirtualMarket->GetMarketDate();
		long year = lDate / 10000;
		long month = lDate / 100 - year * 100;
		long day = lDate - year * 10000 - month * 100;
		sprintf_s(buffer, _T("%4d年%2d月%2d日"), year, month, day);
		str = buffer;
		EXPECT_STREQ(gl_pVirtualMarket->GetStringOfMarketDate(), str);
	}

	TEST_F(CVirtualMarketTest, TestGetDateOfWeek) {
		ASSERT_FALSE(gl_fNormalMode);
		time_t tUTC;
		tm tm_;
		time(&tUTC);
		GetMarketTimeStruct(&tm_, tUTC, gl_pVirtualMarket->GetMarketTimeZone());

		gl_pVirtualMarket->CalculateTime();
		EXPECT_EQ(gl_pVirtualMarket->GetDayOfWeek(), tm_.tm_wday);
	}

	TEST_F(CVirtualMarketTest, TestTaskResetMarketFlagAtMidnight) { // 这个其实是测试的CVirtualMarket类中的函数。
		EXPECT_TRUE(gl_pVirtualMarket->IsPermitResetMarket());
		gl_pVirtualMarket->SetPermitResetMarket(false);
		gl_pVirtualMarket->TaskResetMarketFlagAtMidnight(0);
		EXPECT_TRUE(gl_pVirtualMarket->IsPermitResetMarket());
		gl_pVirtualMarket->SetPermitResetMarket(false);
		gl_pVirtualMarket->TaskResetMarketFlagAtMidnight(1501);
		EXPECT_FALSE(gl_pVirtualMarket->IsPermitResetMarket());
		gl_pVirtualMarket->TaskResetMarketFlagAtMidnight(1500);
		EXPECT_TRUE(gl_pVirtualMarket->IsPermitResetMarket());

		EXPECT_THAT(gl_systemMessage.InformationSize(), 2);
		gl_systemMessage.PopInformationMessage();
		gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CVirtualMarketTest, TestSchedulingTask) {
		EXPECT_TRUE(gl_pVirtualMarket->SchedulingTask()) << "第一次调用时，内部时间初值为0，故而返回真";
		EXPECT_FALSE(gl_pVirtualMarket->SchedulingTask()) << "第二次调用时，内部时间已经设置为当前时间了，间隔不超过一秒，故而返回假";
	}

	TEST_F(CVirtualMarketTest, TestSchedulingTaskPerSecond) {
		EXPECT_TRUE(gl_pVirtualMarket->SchedulingTaskPerSecond(90000));
	}

	TEST_F(CVirtualMarketTest, TestSchedulingTaskPerMinute) {
		EXPECT_TRUE(gl_pVirtualMarket->SchedulingTaskPerMinute(60, 10000));
		EXPECT_FALSE(gl_pVirtualMarket->SchedulingTaskPerMinute(59, 12000));
		EXPECT_TRUE(gl_pVirtualMarket->SchedulingTaskPerMinute(1, 12010));
	}

	TEST_F(CVirtualMarketTest, TestUpdateMarketInfo) {
		EXPECT_TRUE(gl_pVirtualMarket->UpdateMarketInfo());
	}
}