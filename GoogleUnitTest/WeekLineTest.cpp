#include"pch.h"

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

	TEST_F(CWeekLineTest, TestUpdateWeekLine1) {
		CWeekLine weekLine;
		CDayLine dayLine;
		dayLine.SetTime(100100100100);
		dayLine.SetDate(20200202);
		dayLine.SetStockSymbol("600000.SS");

		dayLine.SetOpen(1010);
		dayLine.SetClose(2020);
		dayLine.SetHigh(3030);
		dayLine.SetLow(4040);
		dayLine.SetLastClose(5050);
		dayLine.SetUpDown(static_cast<double>(dayLine.GetClose() - dayLine.GetLastClose()) / dayLine.GetRatio());
		dayLine.SetVolume(1010101010101010);
		dayLine.SetAmount(2020202020202020);
		dayLine.SetCurrentValue(303030303030303);
		dayLine.SetTotalValue(4040404040404040);

		weekLine.UpdateWeekLine(&dayLine);

		EXPECT_EQ(weekLine.GetDate(), GetCurrentMonday(dayLine.GetDate())) << "周线日期总是当周的星期一";
		EXPECT_EQ(weekLine.GetMarketTime(), dayLine.GetMarketTime());
		EXPECT_TRUE(weekLine.GetStockSymbol() == dayLine.GetStockSymbol());
		EXPECT_EQ(weekLine.GetOpen(), dayLine.GetOpen());
		EXPECT_EQ(weekLine.GetClose(), dayLine.GetClose());
		EXPECT_EQ(weekLine.GetHigh(), dayLine.GetHigh());
		EXPECT_EQ(weekLine.GetLow(), dayLine.GetLow());
		EXPECT_EQ(weekLine.GetLastClose(), dayLine.GetLastClose());
		EXPECT_DOUBLE_EQ(weekLine.GetUpDown(), dayLine.GetUpDown());

		EXPECT_EQ(weekLine.GetVolume(), dayLine.GetVolume());
		EXPECT_EQ(weekLine.GetAmount(), dayLine.GetAmount());
		EXPECT_EQ(weekLine.GetCurrentValue(), dayLine.GetCurrentValue());
		EXPECT_EQ(weekLine.GetTotalValue(), dayLine.GetTotalValue());
	}

	TEST_F(CWeekLineTest, TestUpdateWeekLine2) {
		CWeekLine weekLine;
		CDayLine dayLine1, dayLine2;
		dayLine1.SetTime(100100100100);
		dayLine1.SetDate(20200727);
		dayLine1.SetStockSymbol("60000.SS");

		dayLine1.SetOpen(1010);
		dayLine1.SetClose(2020);
		dayLine1.SetHigh(3030);
		dayLine1.SetLow(4040);
		dayLine1.SetLastClose(5050);
		dayLine1.SetUpDown(static_cast<double>(dayLine1.GetClose() - dayLine1.GetLastClose()) / dayLine1.GetRatio());

		dayLine1.SetVolume(1010101010101010);
		dayLine1.SetAmount(2020202020202020);
		dayLine1.SetCurrentValue(303030303030303);
		dayLine1.SetTotalValue(4040404040404040);

		dayLine2.SetTime(10010010010); // 与第一个数据pDayLine1时间不同。
		dayLine2.SetDate(20200728); // 与pDayLine1处于同一个星期中,但不同
		dayLine2.SetStockSymbol("600000.SS"); // 与第一个数据pDayLine1不同。
		dayLine2.SetOpen(10100);
		dayLine2.SetClose(20200);
		dayLine2.SetHigh(30300);
		dayLine2.SetLow(40400);
		dayLine2.SetLastClose(50600);
		dayLine2.SetUpDown(static_cast<double>(dayLine2.GetClose() - dayLine2.GetLastClose()) / dayLine2.GetRatio());

		dayLine2.SetVolume(1010101010101010);
		dayLine2.SetAmount(2020202020202020);
		dayLine2.SetCurrentValue(303030303030303);
		dayLine2.SetTotalValue(4040404040404040);

		weekLine.UpdateWeekLine(&dayLine1);
		weekLine.UpdateWeekLine(&dayLine2);

		EXPECT_EQ(weekLine.GetDate(), dayLine1.GetDate()) << "使用第一个数据的日期";
		EXPECT_NE(weekLine.GetDate(), dayLine2.GetDate()) << "使用第一个数据的日期";
		EXPECT_EQ(weekLine.GetMarketTime(), dayLine1.GetMarketTime()) << "使用第一个数据的时间";
		EXPECT_NE(weekLine.GetMarketTime(), dayLine2.GetMarketTime()) << "使用第一个数据的时间";
		EXPECT_TRUE(weekLine.GetStockSymbol() == dayLine1.GetStockSymbol()) << "股票代码不为空时，不更改";
		EXPECT_FALSE(weekLine.GetStockSymbol() == dayLine2.GetStockSymbol()) << "股票代码不为空时，不更改";

		EXPECT_EQ(weekLine.GetOpen(), dayLine1.GetOpen());
		EXPECT_NE(weekLine.GetOpen(), dayLine2.GetOpen());
		EXPECT_EQ(weekLine.GetClose(), dayLine2.GetClose());
		EXPECT_EQ(weekLine.GetHigh(), dayLine1.GetHigh() > dayLine2.GetHigh() ? dayLine1.GetHigh() : dayLine2.GetHigh());
		EXPECT_EQ(weekLine.GetLow(), dayLine1.GetLow() < dayLine2.GetLow() ? dayLine1.GetLow() : dayLine2.GetLow());
		EXPECT_EQ(weekLine.GetLastClose(), dayLine1.GetLastClose());
		EXPECT_NE(weekLine.GetLastClose(), dayLine2.GetLastClose());
		EXPECT_DOUBLE_EQ(weekLine.GetUpDown(), static_cast<double>(dayLine2.GetClose() - dayLine1.GetLastClose()) / dayLine2.GetRatio());
		EXPECT_EQ(weekLine.GetVolume(), dayLine1.GetVolume() + dayLine2.GetVolume());
		EXPECT_EQ(weekLine.GetAmount(), dayLine1.GetAmount() + dayLine2.GetAmount());
		EXPECT_EQ(weekLine.GetCurrentValue(), dayLine2.GetCurrentValue());
		EXPECT_EQ(weekLine.GetTotalValue(), dayLine2.GetTotalValue());
	}
}
