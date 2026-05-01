#include"pch.h"

#include "CharSetTransfer.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"DayLine.h"
#include"SetForexDayLine.h"

namespace FireBirdTest {
	class CStockDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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
	};

	TEST_F(CStockDayLineTest, TestGetDate) {
		CDayLine dl;
		dl.SetDate(CHINA_MARKET_BEGIN_DATE_);
		EXPECT_EQ(dl.GetDate(), CHINA_MARKET_BEGIN_DATE_);
	}

	TEST_F(CStockDayLineTest, TestGetTime) {
		CDayLine dl;
		EXPECT_EQ(dl.GetDate(), 0);
		dl.SetTime(100100100);
		EXPECT_EQ(dl.GetMarketTime(), 100100100);
	}

	TEST_F(CStockDayLineTest, TestGetStockCode) {
		CDayLine dl;
		EXPECT_EQ(dl.GetStockSymbol(), "");
		dl.SetStockSymbol("600000.SS");
		EXPECT_EQ(dl.GetStockSymbol(), "600000.SS");
	}

	TEST_F(CStockDayLineTest, TestGetLastClose) {
		CDayLine dl;
		EXPECT_EQ(dl.GetLastClose(), 0);
		dl.SetLastClose(10100);
		EXPECT_EQ(dl.GetLastClose(), 10100);
	}

	TEST_F(CStockDayLineTest, TestGetOpen) {
		CDayLine dl;
		EXPECT_EQ(dl.GetOpen(), 0);
		dl.SetOpen(11100);
		EXPECT_EQ(dl.GetOpen(), 11100);
	}

	TEST_F(CStockDayLineTest, TestGetHigh) {
		CDayLine dl;
		EXPECT_EQ(dl.GetHigh(), 0);
		dl.SetHigh(11111);
		EXPECT_EQ(dl.GetHigh(), 11111);
	}

	TEST_F(CStockDayLineTest, TestGetLow) {
		CDayLine dl;
		EXPECT_EQ(dl.GetLow(), 0);
		dl.SetLow(22222);
		EXPECT_EQ(dl.GetLow(), 22222);
		dl.SetLow("12345");
		EXPECT_EQ(dl.GetLow(), 12345);
	}

	TEST_F(CStockDayLineTest, TestGetClose) {
		CDayLine dl;
		EXPECT_EQ(dl.GetClose(), 0);
		dl.SetClose(33333);
		EXPECT_EQ(dl.GetClose(), 33333);
		dl.SetClose("12345");
		EXPECT_EQ(dl.GetClose(), 12345);
	}

	TEST_F(CStockDayLineTest, TestGetUpDown) {
		CDayLine dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
		dl.SetUpDown(10.020);
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
		dl.SetUpDown("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
	}

	TEST_F(CStockDayLineTest, TestGetUpDownRate) {
		CDayLine dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
		dl.SetUpDownRate(40.40);
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
		dl.SetUpDownRate("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
	}

	TEST_F(CStockDayLineTest, TestGetChangeHandRate) {
		CDayLine dl;
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
		dl.SetChangeHandRate(30.30);
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
	}

	TEST_F(CStockDayLineTest, TestGetVolume) {
		CDayLine dl;
		EXPECT_EQ(dl.GetVolume(), 0);
		dl.SetVolume(100100100100100100);
		EXPECT_EQ(dl.GetVolume(), 100100100100100100);
	}

	TEST_F(CStockDayLineTest, TestGetAmount) {
		CDayLine dl;
		EXPECT_EQ(dl.GetAmount(), 0);
		dl.SetAmount(200200200200200200);
		EXPECT_EQ(dl.GetAmount(), 200200200200200200);
	}

	TEST_F(CStockDayLineTest, TestGetTotalValue) {
		CDayLine dl;
		EXPECT_EQ(dl.GetTotalValue(), 0);
		string str = "3.13e+11";
		dl.SetTotalValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetTotalValue()), 3.13e+11);
	}

	TEST_F(CStockDayLineTest, TestGetCurrentValue) {
		CDayLine dl;
		EXPECT_EQ(dl.GetCurrentValue(), 0);
		string str = "3.15e+11";
		dl.SetCurrentValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetCurrentValue()), 3.15e+11);
	}

	TEST_F(CStockDayLineTest, TestGetRS) {
		CDayLine dl;
		EXPECT_DOUBLE_EQ(dl.GetRS(), 0);
		dl.SetRS(50.50);
		EXPECT_DOUBLE_EQ(dl.GetRS(), 50.50);
	}

	TEST_F(CStockDayLineTest, TestGetRSIndex) {
		CDayLine dl;
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 0);
		dl.SetRSIndex(50.5023);
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 50.5023);
	}

	TEST_F(CStockDayLineTest, TestGetRSBackup) {
		CDayLine dl;
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 0);
		dl.SetRSBackup(50.506);
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 50.506);
	}

	TEST_F(CStockDayLineTest, TestGetRSLogarithm) {
		CDayLine dl;
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 0.0);
		dl.SetRSLogarithm(50.50);
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 50.50);
	}

	TEST_F(CStockDayLineTest, TestSaveData) {
		CSetChinaMarketDayLineInfo setDayLineBasicInfo;
		CDayLine id, id2;

		id.SetDate(21100201);
		id.SetStockSymbol("600008.SS");
		id.SetLastClose(34235345);
		id.SetOpen(343452435);
		id.SetHigh(45234543);
		id.SetLow(452345);
		id.SetClose(452435);
		id.SetVolume(34523454);
		id.SetAmount(3245235345);
		id.SetUpDown(12345);
		id.SetUpDownRate(123.45);
		id.SetTotalValue(234523452345);
		id.SetCurrentValue(234145345245);
		id.SetChangeHandRate(54.321);
		id.SetRS(14.5);
		id.SetRSIndex(15.6);
		id.SetRSBackup(20.9);

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setDayLineBasicInfo.m_strFilter = "[ID] = 1";
		setDayLineBasicInfo.Open();
		setDayLineBasicInfo.m_pDatabase->BeginTrans();
		id.AppendBasicData(&setDayLineBasicInfo);
		setDayLineBasicInfo.m_pDatabase->CommitTrans();
		setDayLineBasicInfo.Close();

		setDayLineBasicInfo.m_strFilter = "[Date] = 21100201";
		setDayLineBasicInfo.Open();
		id2.LoadBasicData(&setDayLineBasicInfo);
		EXPECT_EQ(setDayLineBasicInfo.m_Date, id.GetDate());
		EXPECT_TRUE(id.GetStockSymbol() == T2Utf8(setDayLineBasicInfo.m_Symbol));
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_LastClose) * id.GetRatio(), id.GetLastClose());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_Open) * id.GetRatio(), id.GetOpen());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_High) * id.GetRatio(), id.GetHigh());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_Low) * id.GetRatio(), id.GetLow());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_Close) * id.GetRatio(), id.GetClose());
		EXPECT_EQ(_tstoll(setDayLineBasicInfo.m_Volume), id.GetVolume());
		EXPECT_EQ(_tstoll(setDayLineBasicInfo.m_Amount), id.GetAmount());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_UpAndDown), id.GetUpDown());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_UpDownRate), id.GetUpDownRate());
		EXPECT_EQ(_tstoll(setDayLineBasicInfo.m_TotalValue), id.GetTotalValue());
		EXPECT_EQ(_tstoll(setDayLineBasicInfo.m_CurrentValue), id.GetCurrentValue());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_ChangeHandRate), id.GetChangeHandRate());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_RS), id.GetRS());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_RSIndex), id.GetRSIndex());
		EXPECT_DOUBLE_EQ(_tstof(setDayLineBasicInfo.m_RSBackup), id.GetRSBackup());
		setDayLineBasicInfo.Close();

		EXPECT_EQ(id2.GetDate(), id.GetDate());
		EXPECT_EQ(id2.GetStockSymbol(), id.GetStockSymbol());
		EXPECT_DOUBLE_EQ(id2.GetLastClose(), id.GetLastClose());
		EXPECT_DOUBLE_EQ(id2.GetOpen(), id.GetOpen());
		EXPECT_DOUBLE_EQ(id2.GetHigh(), id.GetHigh());
		EXPECT_DOUBLE_EQ(id2.GetLow(), id.GetLow());
		EXPECT_DOUBLE_EQ(id2.GetClose(), id.GetClose());
		EXPECT_EQ(id2.GetVolume(), id.GetVolume());
		EXPECT_EQ(id2.GetAmount(), id.GetAmount());
		EXPECT_DOUBLE_EQ(id2.GetUpDown(), id.GetUpDown());
		EXPECT_DOUBLE_EQ(id2.GetUpDownRate(), id.GetUpDownRate());
		EXPECT_EQ(id2.GetTotalValue(), id.GetTotalValue());
		EXPECT_EQ(id2.GetCurrentValue(), id.GetCurrentValue());
		EXPECT_DOUBLE_EQ(id2.GetChangeHandRate(), id.GetChangeHandRate());
		EXPECT_DOUBLE_EQ(id2.GetRS(), id.GetRS());
		EXPECT_DOUBLE_EQ(id2.GetRSIndex(), id.GetRSIndex());
		EXPECT_DOUBLE_EQ(id2.GetRSBackup(), id.GetRSBackup());

		setDayLineBasicInfo.m_strFilter = "[Date] = 21100201";
		setDayLineBasicInfo.Open();
		setDayLineBasicInfo.m_pDatabase->BeginTrans();
		while (!setDayLineBasicInfo.IsEOF()) {
			setDayLineBasicInfo.Delete();
			setDayLineBasicInfo.MoveNext();
		}
		setDayLineBasicInfo.m_pDatabase->CommitTrans();
		setDayLineBasicInfo.Close();
	}

	TEST_F(CStockDayLineTest, TestLoadChinaMarketData) {
		CSetChinaMarketDayLineInfo setDayLineBasicInfo;
		CDayLine id, idLoaded;

		id.SetDate(21101001);
		id.SetStockSymbol("600008.SS");
		id.SetLastClose(34235345);
		id.SetOpen(343452435);
		id.SetHigh(45234543);
		id.SetLow(3152345);
		id.SetClose(452435);
		id.SetVolume(34523454);
		id.SetAmount(3245235345);
		id.SetUpDown((static_cast<double>(id.GetClose()) - id.GetLastClose()) / id.GetRatio());
		id.SetUpDownRate(123.45);
		id.SetTotalValue(234523452345);
		id.SetCurrentValue(234145345245);
		id.SetChangeHandRate(54.321);
		id.SetRS(14.5);
		id.SetRSIndex(135.6);
		id.SetRSBackup(120.9);

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setDayLineBasicInfo.m_strFilter = "[ID] = 1";
		setDayLineBasicInfo.Open();
		setDayLineBasicInfo.m_pDatabase->BeginTrans();
		id.AppendBasicData(&setDayLineBasicInfo);
		setDayLineBasicInfo.m_pDatabase->CommitTrans();
		setDayLineBasicInfo.Close();

		setDayLineBasicInfo.m_strFilter = "[Date] = 21101001";
		setDayLineBasicInfo.Open();
		idLoaded.LoadBasicData(&setDayLineBasicInfo);
		EXPECT_EQ(idLoaded.GetDate(), id.GetDate());
		EXPECT_EQ(idLoaded.GetStockSymbol(), id.GetStockSymbol());
		EXPECT_EQ(idLoaded.GetLastClose(), id.GetLastClose());
		EXPECT_EQ(idLoaded.GetOpen(), id.GetOpen());
		EXPECT_EQ(idLoaded.GetHigh(), id.GetHigh());
		EXPECT_EQ(idLoaded.GetLow(), id.GetLow());
		EXPECT_EQ(idLoaded.GetClose(), id.GetClose());
		EXPECT_EQ(idLoaded.GetVolume(), id.GetVolume());
		EXPECT_EQ(idLoaded.GetAmount(), id.GetAmount());
		EXPECT_DOUBLE_EQ(idLoaded.GetUpDown(), id.GetUpDown());
		EXPECT_DOUBLE_EQ(idLoaded.GetUpDownRate(), id.GetUpDownRate());
		EXPECT_EQ(idLoaded.GetTotalValue(), id.GetTotalValue());
		EXPECT_EQ(idLoaded.GetCurrentValue(), id.GetCurrentValue());
		EXPECT_DOUBLE_EQ(idLoaded.GetChangeHandRate(), id.GetChangeHandRate());
		EXPECT_DOUBLE_EQ(idLoaded.GetRS(), id.GetRS());
		EXPECT_DOUBLE_EQ(idLoaded.GetRSIndex(), id.GetRSIndex());
		EXPECT_DOUBLE_EQ(idLoaded.GetRSBackup(), id.GetRSBackup());
		setDayLineBasicInfo.Close();

		setDayLineBasicInfo.m_strFilter = "[Date] = 21101001";
		setDayLineBasicInfo.Open();
		setDayLineBasicInfo.m_pDatabase->BeginTrans();
		while (!setDayLineBasicInfo.IsEOF()) {
			setDayLineBasicInfo.Delete();
			setDayLineBasicInfo.MoveNext();
		}
		setDayLineBasicInfo.m_pDatabase->CommitTrans();
		setDayLineBasicInfo.Close();
	}

	TEST_F(CStockDayLineTest, TestIsActive) {
		CDayLine dayLine;
		EXPECT_FALSE(dayLine.IsActive());
		dayLine.SetClose(100);
		EXPECT_TRUE(dayLine.IsActive());
	}

	TEST_F(CStockDayLineTest, TestLoadChinaMarketStockDayLine) {
		CDayLine id, id2;
		CSetChinaMarketDayLineInfo setDayLineBasicInfo;
		id.SetDate(CHINA_MARKET_BEGIN_DATE_);
		id.SetStockSymbol("600000.SS");
		id.SetLastClose(1010);
		id.SetOpen(1100);
		id.SetHigh(1200);
		id.SetLow(1000);
		id.SetClose(1150);
		id.SetVolume(100000);
		id.SetAmount(100000000);
		setDayLineBasicInfo.m_strFilter = "[ID] = 1";
		setDayLineBasicInfo.Open();
		setDayLineBasicInfo.AddNew();
		id.SaveBasicData(&setDayLineBasicInfo);
		setDayLineBasicInfo.Update();
		setDayLineBasicInfo.Close();

		setDayLineBasicInfo.m_strFilter = "[Symbol] = '600000.SS'";
		setDayLineBasicInfo.m_strSort = "[Date]";
		setDayLineBasicInfo.Open();
		id2.LoadBasicData(&setDayLineBasicInfo);
		EXPECT_EQ(id2.GetDate(), CHINA_MARKET_BEGIN_DATE_);
		EXPECT_EQ(id.GetStockSymbol(), id2.GetStockSymbol());
		EXPECT_EQ(id2.GetOpen(), 1100);
		EXPECT_EQ(id2.GetClose(), 1150);

		setDayLineBasicInfo.Delete(); // 清除此新加的数据
		setDayLineBasicInfo.Close();
	}

	TEST_F(CStockDayLineTest, TestLoadHistoryCandle) {
		CSetForexDayLine setForexDayLine;
		CDayLine dayLine;

		setForexDayLine.m_strFilter = "[ID] = 1";
		setForexDayLine.Open();
		dayLine.LoadBasicData(&setForexDayLine);
		setForexDayLine.Close();

		EXPECT_EQ(dayLine.GetDate(), 20200406);
		EXPECT_EQ(dayLine.GetExchange(), "oanda");
		EXPECT_EQ(dayLine.GetStockSymbol(), "OANDA:AUD_SGD");
		EXPECT_EQ(dayLine.GetLastClose(), 0);
		EXPECT_EQ(dayLine.GetOpen(), 863);
		EXPECT_EQ(dayLine.GetHigh(), 874);
		EXPECT_EQ(dayLine.GetLow(), 861);
		EXPECT_EQ(dayLine.GetClose(), 871);
	}
}
