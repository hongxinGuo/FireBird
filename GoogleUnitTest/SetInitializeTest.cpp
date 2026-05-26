#include"pch.h"

#include "GeneralCheck.h"

#include"SetChinaMarketDayLineInfo.h"
#include"SetWeekLineInfo.h"

#include"SetChinaStockSymbol.h"

#include"SetFinnhubStockBasicFinancialMetric.h"
#include"SetFinnhubStockBasicFinancialAnnual.h"
#include"SetFinnhubStockBasicFinancialQuarter.h"
#include"SetInsiderSentiment.h"
#include"SetFinnhubStockDayLine.h"
#include"SetCryptoDayLine.h"
#include"SetForexDayLine.h"

#include"SetTiingoStock.h"
#include"SetTiingoCompanyFinancialState.h"

using namespace testing;

namespace FireBirdTest {
	class SetInitializeTest : public Test {
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
	};

	TEST_F(SetInitializeTest, TestChinaStockSymbolInitialize) {
		CSetChinaStockSymbol setChinaStockSymbol;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setChinaStockSymbol.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setChinaStockSymbol.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setChinaStockSymbol.GetDefaultSQL(), _T("[china_stock_code]"));
	}

	TEST_F(SetInitializeTest, TestDayLineBasicInfoInitialize) {
		CSetChinaMarketDayLineInfo setDayLineBasicInfo;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultSQL(), _T("[china_stock_dayline]"));
	}

	TEST_F(SetInitializeTest, TestWeekLineBasicInfoInitialize) {
		CSetWeekLineInfo setWeekLineBasicInfo;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultSQL(), _T("[china_stock_weekline]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockDayLineInitialize) {
		CSetFinnhubStockDayLine setFinnhubStockDayLine;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setFinnhubStockDayLine.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setFinnhubStockDayLine.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setFinnhubStockDayLine.GetDefaultSQL(), _T("[finnhub_stock_dayline]"));
	}

	TEST_F(SetInitializeTest, TestCryptoDayLineInitialize) {
		CSetCryptoDayLine setCryptoDayLine;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setCryptoDayLine.GetDefaultSQL(), _T("[finnhub_crypto_dayline]"));
	}

	TEST_F(SetInitializeTest, TestForexDayLineInitialize) {
		CSetForexDayLine setForexDayLine;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setForexDayLine.GetDefaultSQL(), _T("[finnhub_forex_dayline]"));
	}

	TEST_F(SetInitializeTest, TestInsiderSentimentInitialize) {
		CSetInsiderSentiment setInsiderSentiment;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setInsiderSentiment.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setInsiderSentiment.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setInsiderSentiment.GetDefaultSQL(), _T("[finnhub_insider_sentiment]"));
	}

	TEST_F(SetInitializeTest, TestTiingoCompanyFinancialStateInitialize) {
		CSetTiingoCompanyFinancialState setTiingoCompanyFinancialState;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoCompanyFinancialState.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoCompanyFinancialState.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoCompanyFinancialState.GetDefaultSQL(), _T("[tiingo_company_financial_state]"));
	}

	TEST_F(SetInitializeTest, TestTiingoStockInitialize) {
		CSetTiingoStock setTiingoStock;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoStock.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoStock.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoStock.GetDefaultSQL(), _T("[tiingo_stock_fundamental]"));
	}

	TEST_F(SetInitializeTest, TestTiingoFinancialStateInitialize) {
		CSetTiingoCompanyFinancialState setTiingoFinancialState;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoFinancialState.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoFinancialState.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoFinancialState.GetDefaultSQL(), _T("[tiingo_company_financial_state]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockBasicFinancialMetricInitialize) {
		CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setBasicFinancialMetric.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setBasicFinancialMetric.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setBasicFinancialMetric.GetDefaultSQL(), _T("[finnhub_stock_basic_financials_metric]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockBasicFinancialAnnualInitialize) {
		CSetFinnhubStockBasicFinancialAnnual setBasicFinancialAnnual;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setBasicFinancialAnnual.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setBasicFinancialAnnual.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setBasicFinancialAnnual.GetDefaultSQL(), _T("[finnhub_stock_basic_financials_annual]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockBasicFinancialQuarterlyInitialize) {
		CSetFinnhubStockBasicFinancialQuarter setBasicFinancialQuarter;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setBasicFinancialQuarter.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setBasicFinancialQuarter.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setBasicFinancialQuarter.GetDefaultSQL(), _T("[finnhub_stock_basic_financials_quarterly]"));
	}
}
