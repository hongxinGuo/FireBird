#include"pch.h"

#include "GeneralCheck.h"

#include"SetChinaMarketDayLineInfo.h"
#include"SetWeekLineInfo.h"

#include"SetChinaChosenStock.h"
#include"SetWorldChosenForex.h"
#include"SetWorldChosenCrypto.h"
#include"SetWorldChosenStock.h"

#include"SetChinaStockSymbol.h"
#include"SetCurrentWeekLine.h"
#include"SetOption.h"
#include"SetStockSection.h"

#include"SetFinnhubStockBasicFinancialMetric.h"
#include"SetFinnhubStockBasicFinancialAnnual.h"
#include"SetFinnhubStockBasicFinancialQuarter.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"
#include"SetInsiderTransaction.h"
#include"SetInsiderSentiment.h"
#include"SetNaicsIndustry.h"
#include"SetSICIndustry.h"
#include"SetFinnhubStock.h"
#include"SetFinnhubStockDayLine.h"
#include"SetCryptoDayLine.h"
#include"SetForexDayLine.h"
#include "SetIndexNasdaq100.h"
#include "SetIndexNasdaq100MA200UpDownRate.h"

#include "SetTiingoMarketNews.h"
#include "SetTiingoStockDayLine.h"
#include"SetTiingoStock.h"
#include"SetTiingoCompanyFinancialState.h"
#include"SetTiingoIndustry.h"
#include "SetTiingoStockCurrentTrace.h"

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

	TEST_F(SetInitializeTest, TestCurrentWeekLineInitialize) {
		CSetCurrentWeekLine setCurrentWeekLine;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setCurrentWeekLine.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setCurrentWeekLine.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setCurrentWeekLine.GetDefaultSQL(), _T("[china_current_weekline]"));
	}

	TEST_F(SetInitializeTest, TestOptionInitialize) {
		CSetOption setOption;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setOption.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setOption.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setOption.GetDefaultSQL(), _T("[china_market_options]"));
	}

	TEST_F(SetInitializeTest, TestStockSectionInitialize) {
		CSetStockSection setStockSection;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setStockSection.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setStockSection.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setStockSection.GetDefaultSQL(), _T("[china_stock_code_section]"));
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

	TEST_F(SetInitializeTest, TestChinaChosenStockInitialize) {
		CSetChinaChosenStock setChinaChosenStock;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setChinaChosenStock.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setChinaChosenStock.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setChinaChosenStock.GetDefaultSQL(), _T("[china_choice_stock]"));
	}

	TEST_F(SetInitializeTest, TestWorldChosenForexInitialize) {
		CSetWorldChosenForex setWorldChosenForex;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setWorldChosenForex.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setWorldChosenForex.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setWorldChosenForex.GetDefaultSQL(), _T("[world_choice_forex]"));
	}

	TEST_F(SetInitializeTest, TestWorldChosenCryptoInitialize) {
		CSetWorldChosenCrypto setWorldChosenCrypto;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setWorldChosenCrypto.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setWorldChosenCrypto.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setWorldChosenCrypto.GetDefaultSQL(), _T("[world_choice_crypto]"));
	}

	TEST_F(SetInitializeTest, TestWorldChosenStockInitialize) {
		CSetWorldChosenStock setWorldChosenStock;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setWorldChosenStock.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setWorldChosenStock.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setWorldChosenStock.GetDefaultSQL(), _T("[world_choice_stock]"));
	}

	TEST_F(SetInitializeTest, TestEconomicCalendarInitialize) {
		CSetEconomicCalendar setEconomicCalendar;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setEconomicCalendar.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setEconomicCalendar.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setEconomicCalendar.GetDefaultSQL(), _T("[finnhub_economic_calendar]"));
	}

	TEST_F(SetInitializeTest, TestEPSSurpriseInitialize) {
		CSetEPSSurprise setEPSSurprise;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setEPSSurprise.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setEPSSurprise.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setEPSSurprise.GetDefaultSQL(), _T("[finnhub_stock_estimates_eps_surprise]"));
	}

	TEST_F(SetInitializeTest, TestInsiderTransactionInitialize) {
		CSetInsiderTransaction setInsiderTransaction;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setInsiderTransaction.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setInsiderTransaction.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setInsiderTransaction.GetDefaultSQL(), _T("[finnhub_insider_transaction]"));
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

	TEST_F(SetInitializeTest, TestNaicsIndustryInitialize) {
		CSetNaicsIndustry setNaicsIndustry;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setNaicsIndustry.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setNaicsIndustry.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setNaicsIndustry.GetDefaultSQL(), _T("[naics_industry]"));
	}

	TEST_F(SetInitializeTest, TestSICIndustryInitialize) {
		CSetSICIndustry setSICIndustry;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setSICIndustry.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setSICIndustry.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setSICIndustry.GetDefaultSQL(), _T("[sic_industry]"));
	}

	TEST_F(SetInitializeTest, TestTiingoIndustryInitialize) {
		CSetTiingoIndustry setTiingoIndustry;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoIndustry.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoIndustry.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoIndustry.GetDefaultSQL(), _T("[tiingo_industry]"));
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

	TEST_F(SetInitializeTest, TestTiingoStockCurrentTraceInitialize) {
		CSetTiingoStockCurrentTrace setTiingoStockCurrentTrace;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoStockCurrentTrace.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoStockCurrentTrace.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoStockCurrentTrace.GetDefaultSQL(), _T("[tiingo_stock_current_trace]"));
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

	TEST_F(SetInitializeTest, TestTiingoFundamentalDefinitionInitialize) {
		CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoFundamentalDefinition.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoFundamentalDefinition.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoFundamentalDefinition.GetDefaultSQL(), _T("[tiingo_fundamental_definitions]"));
	}

	TEST_F(SetInitializeTest, TestTiingoMarketNewsInitialize) {
		CSetTiingoMarketNews setTiingoMarketNews;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoMarketNews.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoMarketNews.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoMarketNews.GetDefaultSQL(), _T("[tiingo_market_news]"));
	}

	TEST_F(SetInitializeTest, TestTiingoStockDayLineInitialize) {
		CSetTiingoStockDayLine setTiingoStockDayLine;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setTiingoStockDayLine.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoStockDayLine.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoStockDayLine.GetDefaultSQL(), _T("[tiingo_stock_dayline]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockInitialize) {
		CSetFinnhubStock setFinnhubStock;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setFinnhubStock.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setFinnhubStock.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setFinnhubStock.GetDefaultSQL(), _T("[finnhub_stock_profile]"));
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

	TEST_F(SetInitializeTest, TestIndexNasdaq100Initialize) {
		CSetIndexNasdaq100 setIndexNasdaq100;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setIndexNasdaq100.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setIndexNasdaq100.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setIndexNasdaq100.GetDefaultSQL(), _T("[index_nasdaq100]"));
	}

	TEST_F(SetInitializeTest, TestIndexNasdaq100MA200UpDownRateInitialize) {
		CSetIndexNasdaq100MA200UpDownRate setIndexNasdaq100MA200UpDownRate;

		EXPECT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_STREQ(setIndexNasdaq100MA200UpDownRate.GetDefaultConnect(), _T("DSN=stock_market_test;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(true);
		EXPECT_STREQ(setIndexNasdaq100MA200UpDownRate.GetDefaultConnect(), _T("DSN=stock_market;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
		gl_systemConfiguration.SetWorkingMode(false);

		EXPECT_STREQ(setIndexNasdaq100MA200UpDownRate.GetDefaultSQL(), _T("[index_nasdaq100_200ma_updown_rate]"));
	}
}
