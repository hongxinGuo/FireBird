#include"pch.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class CVirtualHistoryCandleTest : public ::testing::Test {
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

	TEST_F(CVirtualHistoryCandleTest, TestGetDate) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CVirtualHistoryCandle dl;
		dl.SetDate(19900101);
		EXPECT_EQ(dl.GetDate(), 19900101);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetTime) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetDate(), 0);
		dl.SetTime(100100100);
		EXPECT_EQ(dl.GetMarketTime(), 100100100);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetStockCode) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetStockSymbol(), "");
		dl.SetStockSymbol("600000.SS");
		EXPECT_EQ(dl.GetStockSymbol(), "600000.SS");
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetLastClose) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetLastClose(), 0);
		dl.SetLastClose(10100);
		EXPECT_EQ(dl.GetLastClose(), 10100);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetOpen) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetOpen(), 0);
		dl.SetOpen(11100);
		EXPECT_EQ(dl.GetOpen(), 11100);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetHigh) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetHigh(), 0);
		dl.SetHigh(11111);
		EXPECT_EQ(dl.GetHigh(), 11111);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetLow) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetLow(), 0);
		dl.SetLow(22222);
		EXPECT_EQ(dl.GetLow(), 22222);
		dl.SetLow("12345");
		EXPECT_EQ(dl.GetLow(), 12345);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetClose) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetClose(), 0);
		dl.SetClose(33333);
		EXPECT_EQ(dl.GetClose(), 33333);
		dl.SetClose("12345");
		EXPECT_EQ(dl.GetClose(), 12345);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetUpDown) {
		CVirtualHistoryCandle dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
		dl.SetUpDown(10.020);
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
		dl.SetUpDown("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetUpDownRate) {
		CVirtualHistoryCandle dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
		dl.SetUpDownRate(40.40);
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
		dl.SetUpDownRate("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetChangeHandRate) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CVirtualHistoryCandle dl;
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
		dl.SetChangeHandRate(30.30);
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetVolume) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetVolume(), 0);
		dl.SetVolume(100100100100100100);
		EXPECT_EQ(dl.GetVolume(), 100100100100100100);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetAmount) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetAmount(), 0);
		dl.SetAmount(200200200200200200);
		EXPECT_EQ(dl.GetAmount(), 200200200200200200);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetTotalValue) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetTotalValue(), 0);
		string str = "3.13e+11";
		dl.SetTotalValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetTotalValue()), 3.13e+11);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetCurrentValue) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.GetCurrentValue(), 0);
		string str = "3.15e+11";
		dl.SetCurrentValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetCurrentValue()), 3.15e+11);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetRS) {
		CVirtualHistoryCandle dl;
		EXPECT_DOUBLE_EQ(dl.GetRS(), 0);
		dl.SetRS(50.50);
		EXPECT_DOUBLE_EQ(dl.GetRS(), 50.50);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetRSIndex) {
		CVirtualHistoryCandle dl;
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 0);
		dl.SetRSIndex(50.5023);
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 50.5023);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetRSBackup) {
		CVirtualHistoryCandle dl;
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 0);
		dl.SetRSBackup(50.506);
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 50.506);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGetRSLogarithm) {
		CVirtualHistoryCandle dl;
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 0.0);
		dl.SetRSLogarithm(50.50);
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 50.50);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGet3RS) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.Get3RS(), 0);
		dl.Set3RS(10);
		EXPECT_EQ(dl.Get3RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGet5RS) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.Get5RS(), 0);
		dl.Set5RS(10);
		EXPECT_EQ(dl.Get5RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGet10RS) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.Get10RS(), 0);
		dl.Set10RS(10);
		EXPECT_EQ(dl.Get10RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGet30RS) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.Get30RS(), 0);
		dl.Set30RS(10);
		EXPECT_EQ(dl.Get30RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGet60RS) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.Get60RS(), 0);
		dl.Set60RS(10);
		EXPECT_EQ(dl.Get60RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleTest, TestGet120RS) {
		CVirtualHistoryCandle dl;
		EXPECT_EQ(dl.Get120RS(), 0);
		dl.Set120RS(10);
		EXPECT_EQ(dl.Get120RS(), 10);
	}
}
