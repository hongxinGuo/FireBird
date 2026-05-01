#include"pch.h"

#include "CharSetTransfer.h"
#include"TimeConvert.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"WeekLine.h"

namespace FireBirdTest {
	class CWeekLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			// 本测试类的初始化函数
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

	TEST_F(CWeekLineTest, TestGetDate) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CWeekLine dl;
		dl.SetDate(CHINA_MARKET_BEGIN_DATE_);
		EXPECT_EQ(dl.GetDate(), CHINA_MARKET_BEGIN_DATE_);
	}

	TEST_F(CWeekLineTest, TestGetTime) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetDate(), 0);
		dl.SetTime(100100100);
		EXPECT_EQ(dl.GetMarketTime(), 100100100);
	}

	TEST_F(CWeekLineTest, TestGetStockSymbol) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetStockSymbol(), "");
		dl.SetStockSymbol("600000.SS");
		EXPECT_EQ(dl.GetStockSymbol(), "600000.SS");
	}
	TEST_F(CWeekLineTest, TestGetLastClose) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetLastClose(), 0);
		dl.SetLastClose(10100);
		EXPECT_EQ(dl.GetLastClose(), 10100);
	}

	TEST_F(CWeekLineTest, TestGetOpen) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CWeekLine dl;
		EXPECT_EQ(dl.GetOpen(), 0);
		dl.SetOpen(11100);
		EXPECT_EQ(dl.GetOpen(), 11100);
	}

	TEST_F(CWeekLineTest, TestGetHigh) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetHigh(), 0);
		dl.SetHigh(11111);
		EXPECT_EQ(dl.GetHigh(), 11111);
	}

	TEST_F(CWeekLineTest, TestGetLow) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetLow(), 0);
		dl.SetLow(22222);
		EXPECT_EQ(dl.GetLow(), 22222);
		dl.SetLow("12345");
		EXPECT_EQ(dl.GetLow(), 12345);
	}

	TEST_F(CWeekLineTest, TestGetClose) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetClose(), 0);
		dl.SetClose(33333);
		EXPECT_EQ(dl.GetClose(), 33333);
		dl.SetClose("12345");
		EXPECT_EQ(dl.GetClose(), 12345);
	}

	TEST_F(CWeekLineTest, TestGetUpDown) {
		CWeekLine dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
		dl.SetUpDown(10.020);
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
		dl.SetUpDown("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
	}

	TEST_F(CWeekLineTest, TestGetUpDownRate) {
		CWeekLine dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
		dl.SetUpDownRate(40.40);
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
		dl.SetUpDownRate("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
	}

	TEST_F(CWeekLineTest, TestGetChangeHandRate) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CWeekLine dl;
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
		dl.SetChangeHandRate(30.30);
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
	}

	TEST_F(CWeekLineTest, TestGetVolume) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetVolume(), 0);
		dl.SetVolume(100100100100100100);
		EXPECT_EQ(dl.GetVolume(), 100100100100100100);
	}

	TEST_F(CWeekLineTest, TestGetAmount) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetAmount(), 0);
		dl.SetAmount(200200200200200200);
		EXPECT_EQ(dl.GetAmount(), 200200200200200200);
	}

	TEST_F(CWeekLineTest, TestGetTotalValue) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetTotalValue(), 0);
		string str = "3.13e+11";
		dl.SetTotalValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetTotalValue()), 3.13e+11);
	}

	TEST_F(CWeekLineTest, TestGetCurrentValue) {
		CWeekLine dl;
		EXPECT_EQ(dl.GetCurrentValue(), 0);
		string str = "3.15e+11";
		dl.SetCurrentValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetCurrentValue()), 3.15e+11);
	}

	TEST_F(CWeekLineTest, TestGetRS) {
		CWeekLine dl;
		EXPECT_DOUBLE_EQ(dl.GetRS(), 0);
		dl.SetRS(50.50);
		EXPECT_DOUBLE_EQ(dl.GetRS(), 50.50);
	}

	TEST_F(CWeekLineTest, TestGetRSIndex) {
		CWeekLine dl;
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 0);
		dl.SetRSIndex(50.5023);
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 50.5023);
	}

	TEST_F(CWeekLineTest, TestGetRSBackup) {
		CWeekLine dl;
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 0);
		dl.SetRSBackup(50.506);
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 50.506);
	}

	TEST_F(CWeekLineTest, TestGetRSLogarithm) {
		CWeekLine dl;
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 0.0);
		dl.SetRSLogarithm(50.50);
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 50.50);
	}

	TEST_F(CWeekLineTest, TestGet3RS) {
		CWeekLine dl;
		EXPECT_EQ(dl.Get3RS(), 0);
		dl.Set3RS(10);
		EXPECT_EQ(dl.Get3RS(), 10);
	}

	TEST_F(CWeekLineTest, TestGet5RS) {
		CWeekLine dl;
		EXPECT_EQ(dl.Get5RS(), 0);
		dl.Set5RS(10);
		EXPECT_EQ(dl.Get5RS(), 10);
	}

	TEST_F(CWeekLineTest, TestGet10RS) {
		CWeekLine dl;
		EXPECT_EQ(dl.Get10RS(), 0);
		dl.Set10RS(10);
		EXPECT_EQ(dl.Get10RS(), 10);
	}

	TEST_F(CWeekLineTest, TestGet30RS) {
		CWeekLine dl;
		EXPECT_EQ(dl.Get30RS(), 0);
		dl.Set30RS(10);
		EXPECT_EQ(dl.Get30RS(), 10);
	}

	TEST_F(CWeekLineTest, TestGet60RS) {
		CWeekLine dl;
		EXPECT_EQ(dl.Get60RS(), 0);
		dl.Set60RS(10);
		EXPECT_EQ(dl.Get60RS(), 10);
	}

	TEST_F(CWeekLineTest, TestGet120RS) {
		CWeekLine dl;
		EXPECT_EQ(dl.Get120RS(), 0);
		dl.Set120RS(10);
		EXPECT_EQ(dl.Get120RS(), 10);
	}

	TEST_F(CWeekLineTest, TestSaveData) {
		CSetWeekLineInfo setWeekLineBasicInfo;
		CWeekLine id, id2;

		id.SetDate(21100101);
		id.SetStockSymbol("600008.SS");
		id.SetLastClose(342345);
		id.SetOpen(343435);
		id.SetHigh(452543);
		id.SetLow(345345);
		id.SetClose(452435);
		id.SetVolume(3453454);
		id.SetAmount(32435345);
		id.SetUpDown(12345);
		id.SetUpDownRate(123.45);
		id.SetTotalValue(234523452345);
		id.SetCurrentValue(234145345245);
		id.SetChangeHandRate(54.321);
		id.SetRS(14.5);
		id.SetRSIndex(15.6);
		id.SetRSBackup(20.9);

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setWeekLineBasicInfo.m_strFilter = "[ID] = 1";
		setWeekLineBasicInfo.Open();
		setWeekLineBasicInfo.m_pDatabase->BeginTrans();
		id.AppendBasicData(&setWeekLineBasicInfo);
		setWeekLineBasicInfo.m_pDatabase->CommitTrans();
		setWeekLineBasicInfo.Close();

		setWeekLineBasicInfo.m_strFilter = "[Date] = 21100101";
		setWeekLineBasicInfo.Open();
		id2.LoadBasicData(&setWeekLineBasicInfo);
		EXPECT_EQ(setWeekLineBasicInfo.m_Date, id.GetDate());
		EXPECT_TRUE(id.GetStockSymbol() == T2Utf8(setWeekLineBasicInfo.m_Symbol));
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_LastClose) * id.GetRatio(), id.GetLastClose());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_Open) * id.GetRatio(), id.GetOpen());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_High) * id.GetRatio(), id.GetHigh());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_Low) * id.GetRatio(), id.GetLow());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_Close) * id.GetRatio(), id.GetClose());
		EXPECT_EQ(_tstoll(setWeekLineBasicInfo.m_Volume), id.GetVolume());
		EXPECT_EQ(_tstoll(setWeekLineBasicInfo.m_Amount), id.GetAmount());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_UpAndDown), id.GetUpDown());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_UpDownRate), id.GetUpDownRate());
		EXPECT_EQ(_tstoll(setWeekLineBasicInfo.m_TotalValue), id.GetTotalValue());
		EXPECT_EQ(_tstoll(setWeekLineBasicInfo.m_CurrentValue), id.GetCurrentValue());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_ChangeHandRate), id.GetChangeHandRate());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_RS), id.GetRS());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_RSIndex), id.GetRSIndex());
		EXPECT_DOUBLE_EQ(_tstof(setWeekLineBasicInfo.m_RSBackup), id.GetRSBackup());
		setWeekLineBasicInfo.Close();

		EXPECT_EQ(id2.GetDate(), id.GetDate());
		EXPECT_TRUE(id2.GetStockSymbol() == id.GetStockSymbol());
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

		setWeekLineBasicInfo.m_strFilter = "[Date] = 21100101";
		setWeekLineBasicInfo.Open();
		setWeekLineBasicInfo.m_pDatabase->BeginTrans();
		while (!setWeekLineBasicInfo.IsEOF()) {
			setWeekLineBasicInfo.Delete();
			setWeekLineBasicInfo.MoveNext();
		}
		setWeekLineBasicInfo.m_pDatabase->CommitTrans();
		setWeekLineBasicInfo.Close();
	}

	TEST_F(CWeekLineTest, TestLoadData) {
		CSetWeekLineInfo setWeekLineBasicInfo;
		CWeekLine id, idLoaded;

		id.SetDate(21100901);
		id.SetStockSymbol("600008.SS");
		id.SetLastClose(34235345);
		id.SetOpen(343452435);
		id.SetHigh(45234543);
		id.SetLow(3452343);
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
		setWeekLineBasicInfo.m_strFilter = "[ID] = 1";
		setWeekLineBasicInfo.Open();
		setWeekLineBasicInfo.m_pDatabase->BeginTrans();
		id.AppendBasicData(&setWeekLineBasicInfo);
		setWeekLineBasicInfo.m_pDatabase->CommitTrans();
		setWeekLineBasicInfo.Close();

		setWeekLineBasicInfo.m_strFilter = "[Date] = 21100901";
		setWeekLineBasicInfo.Open();
		idLoaded.LoadBasicData(&setWeekLineBasicInfo);
		EXPECT_EQ(idLoaded.GetDate(), id.GetDate());
		EXPECT_TRUE(idLoaded.GetStockSymbol() == id.GetStockSymbol());
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
		setWeekLineBasicInfo.Close();

		setWeekLineBasicInfo.m_strFilter = "[Date] = 21100901";
		setWeekLineBasicInfo.Open();
		setWeekLineBasicInfo.m_pDatabase->BeginTrans();
		while (!setWeekLineBasicInfo.IsEOF()) {
			setWeekLineBasicInfo.Delete();
			setWeekLineBasicInfo.MoveNext();
		}
		setWeekLineBasicInfo.m_pDatabase->CommitTrans();
		setWeekLineBasicInfo.Close();
	}

	TEST_F(CWeekLineTest, TestLoadWeekLine) {
		CWeekLine id, id2;
		CSetWeekLineInfo setWeekLineBasicInfo;
		id.SetDate(CHINA_MARKET_BEGIN_DATE_);
		id.SetStockSymbol("600000.SS");
		id.SetLastClose(1010);
		id.SetOpen(1100);
		id.SetHigh(1200);
		id.SetLow(1000);
		id.SetClose(1150);
		id.SetVolume(100000);
		id.SetAmount(100000000);
		setWeekLineBasicInfo.m_strFilter = "[ID] = 1";
		setWeekLineBasicInfo.Open();
		setWeekLineBasicInfo.AddNew();
		id.SaveBasicData(&setWeekLineBasicInfo);
		setWeekLineBasicInfo.Update();
		setWeekLineBasicInfo.Close();

		setWeekLineBasicInfo.m_strFilter = "[Date] = 19900101";
		setWeekLineBasicInfo.Open();
		id2.LoadBasicData(&setWeekLineBasicInfo);
		EXPECT_EQ(id.GetDate(), id2.GetDate());
		EXPECT_TRUE(id.GetStockSymbol() == id2.GetStockSymbol());
		EXPECT_EQ(id.GetOpen(), id2.GetOpen());

		setWeekLineBasicInfo.Delete();
		setWeekLineBasicInfo.Close();
	}

	TEST_F(CWeekLineTest, TestUpdateWeekLine1) {
		CWeekLine weekLine;
		auto pDayLine = make_shared<CDayLine>();
		pDayLine->SetTime(100100100100);
		pDayLine->SetDate(20200202);
		pDayLine->SetStockSymbol("600000.SS");

		pDayLine->SetOpen(1010);
		pDayLine->SetClose(2020);
		pDayLine->SetHigh(3030);
		pDayLine->SetLow(4040);
		pDayLine->SetLastClose(5050);
		pDayLine->SetUpDown(static_cast<double>(pDayLine->GetClose() - pDayLine->GetLastClose()) / pDayLine->GetRatio());
		pDayLine->SetVolume(1010101010101010);
		pDayLine->SetAmount(2020202020202020);
		pDayLine->SetCurrentValue(303030303030303);
		pDayLine->SetTotalValue(4040404040404040);

		weekLine.UpdateWeekLine(pDayLine);

		EXPECT_EQ(weekLine.GetDate(), GetCurrentMonday(pDayLine->GetDate())) << "周线日期总是当周的星期一";
		EXPECT_EQ(weekLine.GetMarketTime(), pDayLine->GetMarketTime());
		EXPECT_TRUE(weekLine.GetStockSymbol() == pDayLine->GetStockSymbol());

		EXPECT_EQ(weekLine.GetOpen(), pDayLine->GetOpen());
		EXPECT_EQ(weekLine.GetClose(), pDayLine->GetClose());
		EXPECT_EQ(weekLine.GetHigh(), pDayLine->GetHigh());
		EXPECT_EQ(weekLine.GetLow(), pDayLine->GetLow());
		EXPECT_EQ(weekLine.GetLastClose(), pDayLine->GetLastClose());
		EXPECT_DOUBLE_EQ(weekLine.GetUpDown(), pDayLine->GetUpDown());

		EXPECT_EQ(weekLine.GetVolume(), pDayLine->GetVolume());
		EXPECT_EQ(weekLine.GetAmount(), pDayLine->GetAmount());
		EXPECT_EQ(weekLine.GetCurrentValue(), pDayLine->GetCurrentValue());
		EXPECT_EQ(weekLine.GetTotalValue(), pDayLine->GetTotalValue());
	}

	TEST_F(CWeekLineTest, TestUpdateWeekLine2) {
		CWeekLine weekLine;
		auto pDayLine1 = make_shared<CDayLine>(), pDayLine2 = make_shared<CDayLine>();
		pDayLine1->SetTime(100100100100);
		pDayLine1->SetDate(20200727);
		pDayLine1->SetStockSymbol("60000.SS");

		pDayLine1->SetOpen(1010);
		pDayLine1->SetClose(2020);
		pDayLine1->SetHigh(3030);
		pDayLine1->SetLow(4040);
		pDayLine1->SetLastClose(5050);
		pDayLine1->SetUpDown(static_cast<double>(pDayLine1->GetClose() - pDayLine1->GetLastClose()) / pDayLine1->GetRatio());

		pDayLine1->SetVolume(1010101010101010);
		pDayLine1->SetAmount(2020202020202020);
		pDayLine1->SetCurrentValue(303030303030303);
		pDayLine1->SetTotalValue(4040404040404040);

		pDayLine2->SetTime(10010010010); // 与第一个数据pDayLine1时间不同。
		pDayLine2->SetDate(20200728); // 与pDayLine1处于同一个星期中,但不同
		pDayLine2->SetStockSymbol("600000.SS"); // 与第一个数据pDayLine1不同。

		pDayLine2->SetOpen(10100);
		pDayLine2->SetClose(20200);
		pDayLine2->SetHigh(30300);
		pDayLine2->SetLow(40400);
		pDayLine2->SetLastClose(50600);
		pDayLine2->SetUpDown(static_cast<double>(pDayLine2->GetClose() - pDayLine2->GetLastClose()) / pDayLine2->GetRatio());

		pDayLine2->SetVolume(1010101010101010);
		pDayLine2->SetAmount(2020202020202020);
		pDayLine2->SetCurrentValue(303030303030303);
		pDayLine2->SetTotalValue(4040404040404040);

		weekLine.UpdateWeekLine(pDayLine1);
		weekLine.UpdateWeekLine(pDayLine2);

		EXPECT_EQ(weekLine.GetDate(), pDayLine1->GetDate()) << "使用第一个数据的日期";
		EXPECT_NE(weekLine.GetDate(), pDayLine2->GetDate()) << "使用第一个数据的日期";
		EXPECT_EQ(weekLine.GetMarketTime(), pDayLine1->GetMarketTime()) << "使用第一个数据的时间";
		EXPECT_NE(weekLine.GetMarketTime(), pDayLine2->GetMarketTime()) << "使用第一个数据的时间";
		EXPECT_TRUE(weekLine.GetStockSymbol() == pDayLine1->GetStockSymbol()) << "股票代码不为空时，不更改";
		EXPECT_FALSE(weekLine.GetStockSymbol() == pDayLine2->GetStockSymbol()) << "股票代码不为空时，不更改";

		EXPECT_EQ(weekLine.GetOpen(), pDayLine1->GetOpen());
		EXPECT_NE(weekLine.GetOpen(), pDayLine2->GetOpen());
		EXPECT_EQ(weekLine.GetClose(), pDayLine2->GetClose());
		EXPECT_EQ(weekLine.GetHigh(), pDayLine1->GetHigh() > pDayLine2->GetHigh() ? pDayLine1->GetHigh() : pDayLine2->GetHigh());
		EXPECT_EQ(weekLine.GetLow(), pDayLine1->GetLow() < pDayLine2->GetLow() ? pDayLine1->GetLow() : pDayLine2->GetLow());
		EXPECT_EQ(weekLine.GetLastClose(), pDayLine1->GetLastClose());
		EXPECT_NE(weekLine.GetLastClose(), pDayLine2->GetLastClose());
		EXPECT_DOUBLE_EQ(weekLine.GetUpDown(), static_cast<double>(pDayLine2->GetClose() - pDayLine1->GetLastClose()) / pDayLine2->GetRatio());

		EXPECT_EQ(weekLine.GetVolume(), pDayLine1->GetVolume() + pDayLine2->GetVolume());
		EXPECT_EQ(weekLine.GetAmount(), pDayLine1->GetAmount() + pDayLine2->GetAmount());
		EXPECT_EQ(weekLine.GetCurrentValue(), pDayLine2->GetCurrentValue());
		EXPECT_EQ(weekLine.GetTotalValue(), pDayLine2->GetTotalValue());
	}
}
