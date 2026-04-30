#include"pch.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class CVirtualHistoryCandleBasicTest : public ::testing::Test {
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

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetRatio) {
		const CVirtualHistoryCandleBasic dl;

		EXPECT_EQ(dl.GetRatio(), 1000) << "默认比率为1000";
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetDate) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CVirtualHistoryCandleBasic dl;
		dl.SetDate(19900101);
		EXPECT_EQ(dl.GetDate(), 19900101);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetTime) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetDate(), 0);
		dl.SetTime(100100100);
		EXPECT_EQ(dl.GetMarketTime(), 100100100);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetStockCode) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetStockSymbol(), "");
		dl.SetStockSymbol("600000.SS");
		EXPECT_EQ(dl.GetStockSymbol(), "600000.SS");
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetLastClose) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetLastClose(), 0);
		dl.SetLastClose(10100);
		EXPECT_EQ(dl.GetLastClose(), 10100);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetOpen) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetOpen(), 0);
		dl.SetOpen(11100);
		EXPECT_EQ(dl.GetOpen(), 11100);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetHigh) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetHigh(), 0);
		dl.SetHigh(11111);
		EXPECT_EQ(dl.GetHigh(), 11111);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetLow) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetLow(), 0);
		dl.SetLow(22222);
		EXPECT_EQ(dl.GetLow(), 22222);
		dl.SetLow("12345");
		EXPECT_EQ(dl.GetLow(), 12345);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetClose) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetClose(), 0);
		dl.SetClose(33333);
		EXPECT_EQ(dl.GetClose(), 33333);
		dl.SetClose("12345");
		EXPECT_EQ(dl.GetClose(), 12345);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetUpDown) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
		dl.SetUpDown(10.020);
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
		dl.SetUpDown("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetUpDownRate) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
		dl.SetUpDownRate(40.40);
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
		dl.SetUpDownRate("12.345");
		EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetChangeHandRate) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		CVirtualHistoryCandleBasic dl;
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
		dl.SetChangeHandRate(30.30);
		EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetVolume) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetVolume(), 0);
		dl.SetVolume(100100100100100100);
		EXPECT_EQ(dl.GetVolume(), 100100100100100100);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetAmount) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetAmount(), 0);
		dl.SetAmount(200200200200200200);
		EXPECT_EQ(dl.GetAmount(), 200200200200200200);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetTotalValue) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetTotalValue(), 0);
		string str = "3.13e+11";
		dl.SetTotalValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetTotalValue()), 3.13e+11);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetCurrentValue) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.GetCurrentValue(), 0);
		string str = "3.15e+11";
		dl.SetCurrentValue(str);
		EXPECT_DOUBLE_EQ(static_cast<double>(dl.GetCurrentValue()), 3.15e+11);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetRS) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_DOUBLE_EQ(dl.GetRS(), 0);
		dl.SetRS(50.50);
		EXPECT_DOUBLE_EQ(dl.GetRS(), 50.50);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetRSIndex) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 0);
		dl.SetRSIndex(50.5023);
		EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 50.5023);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetRSBackup) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 0);
		dl.SetRSBackup(50.506);
		EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 50.506);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGetRSLogarithm) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 0.0);
		dl.SetRSLogarithm(50.50);
		EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 50.50);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGet3RS) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.Get3RS(), 0);
		dl.Set3RS(10);
		EXPECT_EQ(dl.Get3RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGet5RS) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.Get5RS(), 0);
		dl.Set5RS(10);
		EXPECT_EQ(dl.Get5RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGet10RS) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.Get10RS(), 0);
		dl.Set10RS(10);
		EXPECT_EQ(dl.Get10RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGet30RS) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.Get30RS(), 0);
		dl.Set30RS(10);
		EXPECT_EQ(dl.Get30RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGet60RS) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.Get60RS(), 0);
		dl.Set60RS(10);
		EXPECT_EQ(dl.Get60RS(), 10);
	}

	TEST_F(CVirtualHistoryCandleBasicTest, TestGet120RS) {
		CVirtualHistoryCandleBasic dl;
		EXPECT_EQ(dl.Get120RS(), 0);
		dl.Set120RS(10);
		EXPECT_EQ(dl.Get120RS(), 10);
	}
}
