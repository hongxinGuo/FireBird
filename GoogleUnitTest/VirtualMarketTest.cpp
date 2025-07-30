#include"pch.h"

#include"GeneralCheck.h"
#include"VirtualMarket.h"

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
		EXPECT_EQ(pTask->GetTime(), 1) << "��ͬʱ��������������";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 1) << "��ͬʱ��������������";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "��ͬʱ��������������";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "��ͬʱ��������������";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "��ͬʱ��������������";
		virtualMarket.DiscardCurrentMarketTask();

		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 10000) << "��ͬʱ��������������";
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
		EXPECT_EQ(pTask->GetTime(), 350) << "���е�ʱ��Դ���240000���ʶ��Լ�ȥ240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 34010) << "���е�ʱ��Դ���240000���ʶ��Լ�ȥ240000";
		virtualMarket.DiscardCurrentMarketTask();
		pTask = virtualMarket.GetMarketTask();
		EXPECT_EQ(pTask->GetTime(), 60000) << "���е�ʱ��Դ���240000���ʶ��Լ�ȥ240000";
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
		EXPECT_FALSE(virtualMarket.IsSystemReady()) << "�г�����Ĭ��Ϊ��";
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
		virtualMarket.GetLastTradeDate();
		EXPECT_EQ(virtualMarket.GetDayOfWeek(), tm_.tm_wday);

		const long day = virtualMarket.ConvertToDate(&tm_);
		EXPECT_EQ(virtualMarket.GetMarketDate(), day);
		EXPECT_EQ(virtualMarket.GetMonthOfYear(), tm_.tm_mon + 1);
		EXPECT_EQ(virtualMarket.GetDateOfMonth(), tm_.tm_mday);
		EXPECT_EQ(virtualMarket.GetYear(), tm_.tm_year + 1900);

		const long time = tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec;
		EXPECT_EQ(virtualMarket.GetMarketTime(), time);
		string s = fmt::format("{:02d}:{:02d}:{:02d}", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);

		EXPECT_EQ(s.compare(virtualMarket.GetStringOfLocalTime()), 0);

		s = fmt::format("{:02d}:{:02d}:{:02d}", tm_.tm_hour, tm_.tm_min, tm_.tm_sec);
		EXPECT_EQ(s.compare(virtualMarket.GetStringOfMarketTime()), 0);
	}

	TEST_F(CVirtualMarketTest, TestGetLastTradeDay) {
		time_t tMarket;
		auto tUTCTime = GetUTCTime();

		for (int i = 0; i < 7; i++) {
			TestSetUTCTime(tUTCTime + i * 24 * 3600);
			tm tmMarketTime2;
			virtualMarket.GetMarketTimeStruct(&tmMarketTime2, GetUTCTime() - virtualMarket.GetMarketOpenTime());

			switch (tmMarketTime2.tm_wday) {
			case 1: // ����һ
				tMarket = GetUTCTime() - 3 * 24 * 3600; // ������
				break;
			case 0: //������
				tMarket = GetUTCTime() - 3 * 24 * 3600; // ����
				break;
			case 6: // ������
				tMarket = GetUTCTime() - 2 * 24 * 3600; // ����
				break;
			default: // ����
				tMarket = GetUTCTime() - 24 * 3600; // ��һ��
			}
			tm tmMarketTime;
			virtualMarket.GetMarketTimeStruct(&tmMarketTime, tMarket - virtualMarket.GetMarketOpenTime());
			auto lMarketLastTradeDate = virtualMarket.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(virtualMarket.GetLastTradeDate(), lMarketLastTradeDate) << i;
		}

		// �ָ�ԭ״
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CVirtualMarketTest, TestGetCurrentTradeDay) {
		time_t tMarket;
		auto tUTCTime = GetUTCTime();
		for (int i = 0; i < 7; i++) {
			TestSetUTCTime(tUTCTime + i * 24 * 3600);
			tm tmMarketTime2;
			virtualMarket.GetMarketTimeStruct(&tmMarketTime2, GetUTCTime() - virtualMarket.GetMarketOpenTime());

			switch (tmMarketTime2.tm_wday) {
			case 0: //������
				tMarket = GetUTCTime() - 2 * 24 * 3600; // ����
				break;
			case 6: // ������
				tMarket = GetUTCTime() - 1 * 24 * 3600; // ����
				break;
			default: // ����
				tMarket = GetUTCTime(); // ����
			}
			tm tmMarketTime;
			virtualMarket.GetMarketTimeStruct(&tmMarketTime, tMarket - virtualMarket.GetMarketOpenTime());
			auto lMarketCurrentTradeDate = virtualMarket.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(virtualMarket.GetCurrentTradeDate(), lMarketCurrentTradeDate) << i;
		}

		// �ָ�ԭ״
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CVirtualMarketTest, TestGetNextTradeDay) {
		time_t tMarket;
		auto tUTCTime = GetUTCTime();

		for (int i = 0; i < 7; i++) {
			TestSetUTCTime(tUTCTime + i * 24 * 3600);
			tm tmMarketTime2;
			virtualMarket.GetMarketTimeStruct(&tmMarketTime2, GetUTCTime() - virtualMarket.GetMarketOpenTime());

			switch (tmMarketTime2.tm_wday) {
			case 6: // ������
				tMarket = GetUTCTime() + 2 * 24 * 3600; // ����һ
				break;
			case 5: // ����
				tMarket = GetUTCTime() + 3 * 24 * 3600; // ����һ
				break;
			default: // ����
				tMarket = GetUTCTime() + 24 * 3600; // ����
			}
			tm tmMarketTime;
			virtualMarket.GetMarketTimeStruct(&tmMarketTime, tMarket - virtualMarket.GetMarketOpenTime());
			auto lMarketNextTradeDate = virtualMarket.ConvertToDate(&tmMarketTime);
			EXPECT_EQ(virtualMarket.GetNextTradeDate(), lMarketNextTradeDate) << i;
		}

		// �ָ�ԭ״
		TestSetUTCTime(tUTCTime);
	}

	TEST_F(CVirtualMarketTest, TestTransferToMarketTime) {
		tm tm2_;
		virtualMarket.CalculateTime();
		time_t tt = GetUTCTime();
		gmtime_s(&tm2_, &tt);
		tm tm_;
		virtualMarket.GetMarketTimeStruct(&tm_, GetUTCTime());
		EXPECT_TRUE(tm_.tm_hour == tm2_.tm_hour) << "VirtualMarketĬ��ΪGMT��׼ʱ��";
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

		tTime = virtualMarket.TransferToUTCTime(&tmMarket); // ʹ��Ĭ��ʱ��150000.
		virtualMarket.GetMarketTimeStruct(&tm2, tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 12);
		EXPECT_EQ(tm2.tm_min, 20);
		EXPECT_EQ(tm2.tm_sec, 30);

		tTime = virtualMarket.TransferToUTCTime(lMarketDate); // ʹ��Ĭ��ʱ��150000.
		virtualMarket.GetMarketTimeStruct(&tm2, tTime);
		EXPECT_EQ(tm2.tm_year, 100);
		EXPECT_EQ(tm2.tm_mon, 2);
		EXPECT_EQ(tm2.tm_mday, 1);
		EXPECT_EQ(tm2.tm_hour, 15);
		EXPECT_EQ(tm2.tm_min, 00);
		EXPECT_EQ(tm2.tm_sec, 00);

		tTime = virtualMarket.TransferToUTCTime(lMarketDate, lMarketTime);
		virtualMarket.GetMarketTimeStruct(&tm3, tTime);
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
		tm tm_;

		for (int i = 0; i < 7; i++) {
			tUTC += i * 60 * 60 * 24;
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC - virtualMarket.GetMarketOpenTime());
			TestSetUTCTime(tUTC);

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
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC - virtualMarket.GetMarketOpenTime());
			long nextTradeDate = virtualMarket.ConvertToDate(&tm_);
			EXPECT_EQ(virtualMarket.GetNextTradeDate(), nextTradeDate) << i;
		}
	}

	TEST_F(CVirtualMarketTest, TestGetCurrentTradeDate) {
		time_t tUTC = GetUTCTime();
		tm tm_;

		for (int i = 0; i < 7; i++) {
			tUTC += i * 60 * 60 * 24;
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC - virtualMarket.GetMarketOpenTime());
			TestSetUTCTime(tUTC);

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
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC - virtualMarket.GetMarketOpenTime());
			long currentTradeDate = virtualMarket.ConvertToDate(&tm_);
			EXPECT_EQ(virtualMarket.GetCurrentTradeDate(), currentTradeDate) << i;
		}
	}

	TEST_F(CVirtualMarketTest, TestGetLastTradeDate) {
		time_t tUTC = GetUTCTime();
		tm tm_;

		for (int i = 0; i < 7; i++) {
			tUTC += i * 60 * 60 * 24;
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC - virtualMarket.GetMarketOpenTime());
			TestSetUTCTime(tUTC);

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
			virtualMarket.GetMarketTimeStruct(&tm_, tUTC - virtualMarket.GetMarketOpenTime());
			long LastTradeDate = virtualMarket.ConvertToDate(&tm_);
			EXPECT_EQ(virtualMarket.GetLastTradeDate(), LastTradeDate) << i;
		}
	}

	TEST_F(CVirtualMarketTest, TestIsWorkingDay) {
		const CTime time1(2019, 11, 25, 0, 0, 0); // ����Ϊ����һ
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time1));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191125));
		const CTime time2(2019, 11, 26, 0, 0, 0); // ����Ϊ���ڶ�
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time2));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191126));
		const CTime time3(2019, 11, 27, 0, 0, 0); // ����Ϊ������
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time3));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191127));
		const CTime time4(2019, 11, 28, 0, 0, 0); // ����Ϊ������
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time4));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191128));
		const CTime time5(2019, 11, 29, 0, 0, 0); // ����Ϊ������
		EXPECT_TRUE(virtualMarket.IsWorkingDay(time5));
		EXPECT_TRUE(virtualMarket.IsWorkingDay(20191129));
		const CTime time6(2019, 11, 30, 0, 0, 0); // ����Ϊ������
		EXPECT_FALSE(virtualMarket.IsWorkingDay(time6));
		EXPECT_FALSE(virtualMarket.IsWorkingDay(20191130));
		const CTime time7(2019, 12, 1, 0, 0, 0); // ����Ϊ������
		EXPECT_FALSE(virtualMarket.IsWorkingDay(time7));
		EXPECT_FALSE(virtualMarket.IsWorkingDay(20191201));
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalTime) {
		virtualMarket.CalculateTime();

		time_t tUTC = GetUTCTime();
		tm tmLocal;

		localtime_s(&tmLocal, &tUTC);
		string s = fmt::format("{:02d}:{:02d}:{:02d}", tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		EXPECT_EQ(virtualMarket.GetStringOfLocalTime(), s);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfLocalDateTime) {
		virtualMarket.CalculateTime();

		const time_t tUTC = GetUTCTime();
		tm tmLocal;

		localtime_s(&tmLocal, &tUTC);
		string s = fmt::format("{:04d}��{:02d}��{:02d}�� {:02d}:{:02d}:{:02d}", tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);
		EXPECT_EQ(virtualMarket.GetStringOfLocalDateTime(), s);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketTime) {
		virtualMarket.CalculateTime();

		tm tmMarket;
		virtualMarket.GetMarketTimeStruct(&tmMarket, GetUTCTime());

		string s = fmt::format("{:02d}:{:02d}:{:02d}", tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		EXPECT_EQ(virtualMarket.GetStringOfMarketTime(), s);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDateTime) {
		virtualMarket.CalculateTime();

		tm tmMarket;

		virtualMarket.GetMarketTimeStruct(&tmMarket, GetUTCTime());
		string s = fmt::format("{:04d}��{:02d}��{:02d}�� {:02d}:{:02d}:{:02d}", tmMarket.tm_year + 1900, tmMarket.tm_mon + 1, tmMarket.tm_mday, tmMarket.tm_hour, tmMarket.tm_min, tmMarket.tm_sec);
		EXPECT_EQ(virtualMarket.GetStringOfMarketDateTime(), s);
	}

	TEST_F(CVirtualMarketTest, TestGetStringOfMarketDate) {
		const long lDate = virtualMarket.GetMarketDate();
		const long year = lDate / 10000;
		const long month = lDate / 100 - year * 100;
		const long day = lDate - year * 10000 - month * 100;
		string s = fmt::format("{:04Ld}��{:02Ld}��{:02Ld}��", year, month, day);
		EXPECT_EQ(virtualMarket.GetStringOfMarketDate(), s);
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
		strConvertBufferToTime(const string& strFormat, const string& strBuffer, const INT64 iTime) {
			m_strFormat = strFormat;
			m_strBuffer = strBuffer;
			m_Time = iTime;
		}

	public:
		string m_strFormat;
		string m_strBuffer;
		INT64 m_Time;
	};
}
