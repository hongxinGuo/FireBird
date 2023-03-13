#include"pch.h"

#include "GeneralCheck.h"

#include"SetChinaStockSymbol.h"
#include"SetCurrentWeekLine.h"
#include"SetDayLineExtendInfo.h"
#include"SetDayLineTodaySaved.h"
#include"SetOption.h"
#include"SetRSOption.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrong2Stock.h"
#include"SetRSStrongStock.h"
#include"SetStockSection.h"
#include"SetWeekLineExtendInfo.h"

#include"SetCountry.h"
#include"SetFinnhubStockBasicFinancialMetric.h"
#include"SetFinnhubStockBasicFinancialAnnual.h"
#include"SetFinnhubStockBasicFinancialQuarter.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"
#include"SetInsiderTransaction.h"
#include"SetInsiderSentiment.h"
#include"SetNaicsIndustry.h"
#include"SetSICIndustry.h"
#include"SetTiingoIndustry.h"
#include"SetTiingoStock.h"
#include"SetWorldStock.h"

#include"SetDayLineBasicInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetCryptoDayLine.h"
#include"SetWorldStockDayLine.h"
#include"SetForexDayLine.h"

#include"SetChinaChosenStock.h"
#include"SetWorldChosenForex.h"
#include"SetWorldChosenCrypto.h"
#include"SetWorldChosenStock.h"

using namespace testing;

namespace FireBirdTest {
	class SetInitializeTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
	};

	TEST_F(SetInitializeTest, TestChinaStockSymbolInitialize) {
		CSetChinaStockSymbol setChinaStockSymbol;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setChinaStockSymbol.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setChinaStockSymbol.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setChinaStockSymbol.GetDefaultSQL(), _T("[stockcode]"));
	}

	TEST_F(SetInitializeTest, TestCurrentWeekLineInitialize) {
		CSetCurrentWeekLine setCurrentWeekLine;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setCurrentWeekLine.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setCurrentWeekLine.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setCurrentWeekLine.GetDefaultSQL(), _T("[CurrentWeekLine]"));
	}

	TEST_F(SetInitializeTest, TestDayLineExtendInfoInitialize) {
		CSetDayLineExtendInfo setDayLineExtendInfo;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setDayLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setDayLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setDayLineExtendInfo.GetDefaultSQL(), _T("[daylineinfo]"));
	}

	TEST_F(SetInitializeTest, TestDayLineTempInitialize) {
		CSetDayLineTodaySaved setDayLineTemp;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setDayLineTemp.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setDayLineTemp.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setDayLineTemp.GetDefaultSQL(), _T("[today]"));
	}

	TEST_F(SetInitializeTest, TestOptionInitialize) {
		CSetOption setOption;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setOption.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setOption.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setOption.GetDefaultSQL(), _T("[options]"));
	}

	TEST_F(SetInitializeTest, TestRSOptionInitialize) {
		CSetRSOption setRSOption;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setRSOption.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setRSOption.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setRSOption.GetDefaultSQL(), _T("[option_calculatingrs]"));
	}

	TEST_F(SetInitializeTest, TestRSStrong1StockInitialize) {
		CSetRSStrong1Stock setRSStrong1Stock;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setRSStrong1Stock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setRSStrong1Stock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setRSStrong1Stock.GetDefaultSQL(), _T("[10rs1above55]"));
	}

	TEST_F(SetInitializeTest, TestRSStrong2StockInitialize) {
		CSetRSStrong2Stock setRSStrong2Stock;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setRSStrong2Stock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setRSStrong2Stock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setRSStrong2Stock.GetDefaultSQL(), _T("[10rs2above55]"));
	}

	TEST_F(SetInitializeTest, TestRSStrongStockInitialize) {
		CSetRSStrongStock setRSStrongStock(1);

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setRSStrongStock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setRSStrongStock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setRSStrongStock.GetDefaultSQL(), _T("[selected_rs_1]"));
	}

	TEST_F(SetInitializeTest, TestStockSectionInitialize) {
		CSetStockSection setStockSection;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setStockSection.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setStockSection.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setStockSection.GetDefaultSQL(), _T("[Stock_Code_Section]"));
	}

	TEST_F(SetInitializeTest, TestWeekLineExtendInfoInitialize) {
		CSetWeekLineExtendInfo setWeekLineExtendInfo;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setWeekLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setWeekLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setWeekLineExtendInfo.GetDefaultSQL(), _T("[WeekLineinfo]"));
	}

	TEST_F(SetInitializeTest, TestDayLineBasicInfoInitialize) {
		CSetDayLineBasicInfo setDayLineBasicInfo;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setDayLineBasicInfo.GetDefaultSQL(), _T("[dayline]"));
	}

	TEST_F(SetInitializeTest, TestWeekLineBasicInfoInitialize) {
		CSetWeekLineBasicInfo setWeekLineBasicInfo;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultSQL(), _T("[weekline]"));
	}

	TEST_F(SetInitializeTest, TestWorldStockDayLineInitialize) {
		CSetWorldStockDayLine setWorldStockDayLine;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setWorldStockDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setWorldStockDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setWorldStockDayLine.GetDefaultSQL(), _T("[stock_dayline]"));
	}

	TEST_F(SetInitializeTest, TestCryptoDayLineInitialize) {
		CSetCryptoDayLine setCryptoDayLine;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setCryptoDayLine.GetDefaultSQL(), _T("[crypto_dayline]"));
	}

	TEST_F(SetInitializeTest, TestForexDayLineInitialize) {
		CSetForexDayLine setForexDayLine;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setForexDayLine.GetDefaultSQL(), _T("[forex_dayline]"));
	}

	TEST_F(SetInitializeTest, TestChinaChosenStockInitialize) {
		CSetChinaChosenStock setChinaChosenStock;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setChinaChosenStock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setChinaChosenStock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setChinaChosenStock.GetDefaultSQL(), _T("[choiced_stock]"));
	}

	TEST_F(SetInitializeTest, TestWorldChosenForexInitialize) {
		CSetWorldChosenForex setWorldChosenForex;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setWorldChosenForex.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setWorldChosenForex.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setWorldChosenForex.GetDefaultSQL(), _T("[choiced_forex]"));
	}

	TEST_F(SetInitializeTest, TestWorldChosenCryptoInitialize) {
		CSetWorldChosenCrypto setWorldChosenCrypto;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setWorldChosenCrypto.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setWorldChosenCrypto.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setWorldChosenCrypto.GetDefaultSQL(), _T("[choiced_crypto]"));
	}

	TEST_F(SetInitializeTest, TestWorldChosenStockInitialize) {
		CSetWorldChosenStock setWorldChosenStock;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setWorldChosenStock.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setWorldChosenStock.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setWorldChosenStock.GetDefaultSQL(), _T("[choiced_stock]"));
	}

	TEST_F(SetInitializeTest, TestCountryInitialize) {
		CSetCountry setCountry;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setCountry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setCountry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setCountry.GetDefaultSQL(), _T("[country_list]"));
	}

	TEST_F(SetInitializeTest, TestEconomicCalendarInitialize) {
		CSetEconomicCalendar setEconomicCalendar;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setEconomicCalendar.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setEconomicCalendar.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setEconomicCalendar.GetDefaultSQL(), _T("[economic_calendar]"));
	}

	TEST_F(SetInitializeTest, TestEPSSurpriseInitialize) {
		CSetEPSSurprise setEPSSurprise;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setEPSSurprise.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setEPSSurprise.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setEPSSurprise.GetDefaultSQL(), _T("[eps_surprise]"));
	}

	TEST_F(SetInitializeTest, TestInsiderTransactionInitialize) {
		CSetInsiderTransaction setInsiderTransaction;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setInsiderTransaction.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setInsiderTransaction.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setInsiderTransaction.GetDefaultSQL(), _T("[insider_transaction]"));
	}

	TEST_F(SetInitializeTest, TestInsiderSentimentInitialize) {
		CSetInsiderSentiment setInsiderSentiment;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setInsiderSentiment.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setInsiderSentiment.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setInsiderSentiment.GetDefaultSQL(), _T("[insider_sentiment]"));
	}

	TEST_F(SetInitializeTest, TestNaicsIndustryInitialize) {
		CSetNaicsIndustry setNaicsIndustry;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setNaicsIndustry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setNaicsIndustry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setNaicsIndustry.GetDefaultSQL(), _T("[naics_industry]"));
	}

	TEST_F(SetInitializeTest, TestSICIndustryInitialize) {
		CSetSICIndustry setSICIndustry;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setSICIndustry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setSICIndustry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setSICIndustry.GetDefaultSQL(), _T("[sic_industry]"));
	}

	TEST_F(SetInitializeTest, TestTiingoIndustryInitialize) {
		CSetTiingoIndustry setTiingoIndustry;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setTiingoIndustry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoIndustry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoIndustry.GetDefaultSQL(), _T("[tiingo_industry]"));
	}

	TEST_F(SetInitializeTest, TestTiingoStockInitialize) {
		CSetTiingoStock setTiingoStock;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setTiingoStock.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setTiingoStock.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setTiingoStock.GetDefaultSQL(), _T("[tiingo_stock_fundanmental]"));
	}

	TEST_F(SetInitializeTest, TestWorldStockInitialize) {
		CSetWorldStock setWorldStock;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setWorldStock.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setWorldStock.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setWorldStock.GetDefaultSQL(), _T("[stock_profile]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockBasicFinancialMetricInitialize) {
		CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setBasicFinancialMetric.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setBasicFinancialMetric.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setBasicFinancialMetric.GetDefaultSQL(), _T("[finnhub_stock_basic_financials_metric]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockBasicFinancialAnnualInitialize) {
		CSetFinnhubStockBasicFinancialAnnual setBasicFinancialAnnual;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setBasicFinancialAnnual.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setBasicFinancialAnnual.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setBasicFinancialAnnual.GetDefaultSQL(), _T("[finnhub_stock_basic_financials_annual]"));
	}

	TEST_F(SetInitializeTest, TestFinnhubStockBasicFinancialQuarterlyInitialize) {
		CSetFinnhubStockBasicFinancialQuarter setBasicFinancialQuarter;

		EXPECT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_STREQ(setBasicFinancialQuarter.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(true);
		EXPECT_STREQ(setBasicFinancialQuarter.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
		gl_systemStatus.SetWorkingMode(false);

		EXPECT_STREQ(setBasicFinancialQuarter.GetDefaultSQL(), _T("[finnhub_stock_basic_financials_quarterly]"));
	}
}
