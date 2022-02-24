#include"pch.h"
#include"globedef.h"

#include"SetChinaMarketOption.h"
#include"SetChinaStockSymbol.h"
#include"SetCurrentWeekLine.h"
#include"SetDayLineExtendInfo.h"
#include"SetDayLineTodaySaved.h"
#include"SetOption.h"
#include"SetRealTimeData.h"
#include"SetRSOption.h"
#include"SetRSStrong1Stock.h"
#include"SetRSStrong2Stock.h"
#include"SetRSStrongStock.h"
#include"SetStockSection.h"
#include"SetWeekLineExtendInfo.h"

#include"SetCountry.h"
#include"SetEconomicCalendar.h"
#include"SetEPSSurprise.h"
#include"SetInsiderTransaction.h"
#include"SetNaicsIndustry.h"
#include"SetSICIndustry.h"
#include"SetTiingoIndustry.h"
#include"SetTiingoStock.h"
#include"SetWorldmarketOption.h"
#include"SetWorldStock.h"

#include"SetDayLineBasicInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetCryptoDayLine.h"
#include"SetWorldStockDayLine.h"
#include"SetForexDayLine.h"

#include"SetChinaChoicedStock.h"
#include"SetWorldChoicedForex.h"
#include"SetWorldChoicedCrypto.h"
#include"SetWorldChoicedStock.h"

TEST(SetInitializeTest, TestChinaMarketOptionInitialize) {
	CSetChinaMarketOption setChinaMarketOption;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setChinaMarketOption.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setChinaMarketOption.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setChinaMarketOption.GetDefaultSQL(), _T("[option_china_stockmarket]"));
}

TEST(SetInitializeTest, TestChinaStockSymbolInitialize) {
	CSetChinaStockSymbol setChinaStockSymbol;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setChinaStockSymbol.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setChinaStockSymbol.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setChinaStockSymbol.GetDefaultSQL(), _T("[stockcode]"));
}

TEST(SetInitializeTest, TestCurrentWeekLineInitialize) {
	CSetCurrentWeekLine setCurrentWeekLine;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setCurrentWeekLine.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setCurrentWeekLine.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setCurrentWeekLine.GetDefaultSQL(), _T("[CurrentWeekLine]"));
}

TEST(SetInitializeTest, TestDayLineExtendInfoInitialize) {
	CSetDayLineExtendInfo setDayLineExtendInfo;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setDayLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setDayLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setDayLineExtendInfo.GetDefaultSQL(), _T("[daylineinfo]"));
}

TEST(SetInitializeTest, TestDayLineTempInitialize) {
	CSetDayLineTodaySaved setDayLineTemp;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setDayLineTemp.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setDayLineTemp.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setDayLineTemp.GetDefaultSQL(), _T("[today]"));
}

TEST(SetInitializeTest, TestOptionInitialize) {
	CSetOption setOption;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setOption.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setOption.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setOption.GetDefaultSQL(), _T("[options]"));
}

TEST(SetInitializeTest, TestRealTimeDataInitialize) {
	CSetRealTimeData setRealTimeData;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setRealTimeData.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setRealTimeData.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setRealTimeData.GetDefaultSQL(), _T("[RealTimeData]"));
}

TEST(SetInitializeTest, TestRSOptionInitialize) {
	CSetRSOption setRSOption;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setRSOption.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setRSOption.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setRSOption.GetDefaultSQL(), _T("[option_calculatingrs]"));
}

TEST(SetInitializeTest, TestRSStrong1StockInitialize) {
	CSetRSStrong1Stock setRSStrong1Stock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setRSStrong1Stock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setRSStrong1Stock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setRSStrong1Stock.GetDefaultSQL(), _T("[10rs1above55]"));
}

TEST(SetInitializeTest, TestRSStrong2StockInitialize) {
	CSetRSStrong2Stock setRSStrong2Stock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setRSStrong2Stock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setRSStrong2Stock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setRSStrong2Stock.GetDefaultSQL(), _T("[10rs2above55]"));
}

TEST(SetInitializeTest, TestRSStrongStockInitialize) {
	CSetRSStrongStock setRSStrongStock(1);

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setRSStrongStock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setRSStrongStock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setRSStrongStock.GetDefaultSQL(), _T("[selected_rs_1]"));
}

TEST(SetInitializeTest, TestStockSectionInitialize) {
	CSetStockSection setStockSection;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setStockSection.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setStockSection.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setStockSection.GetDefaultSQL(), _T("[Stock_Code_Section]"));
}

TEST(SetInitializeTest, TestWeekLineExtendInfoInitialize) {
	CSetWeekLineExtendInfo setWeekLineExtendInfo;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWeekLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWeekLineExtendInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setWeekLineExtendInfo.GetDefaultSQL(), _T("[WeekLineinfo]"));
}

TEST(SetInitializeTest, TestDayLineBasicInfoInitialize) {
	CSetDayLineBasicInfo setDayLineBasicInfo;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setDayLineBasicInfo.GetDefaultSQL(), _T("[dayline]"));
}

TEST(SetInitializeTest, TestWeekLineBasicInfoInitialize) {
	CSetWeekLineBasicInfo setWeekLineBasicInfo;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultSQL(), _T("[weekline]"));
}

TEST(SetInitializeTest, TestWorldStockDayLineInitialize) {
	CSetWorldStockDayLine setWorldStockDayLine;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldStockDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldStockDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setWorldStockDayLine.GetDefaultSQL(), _T("[stock_dayline]"));
}

TEST(SetInitializeTest, TestCryptoDayLineInitialize) {
	CSetCryptoDayLine setCryptoDayLine;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setCryptoDayLine.GetDefaultSQL(), _T("[crypto_dayline]"));
}

TEST(SetInitializeTest, TestForexDayLineInitialize) {
	CSetForexDayLine setForexDayLine;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setForexDayLine.GetDefaultSQL(), _T("[forex_dayline]"));
}

TEST(SetInitializeTest, TestChinaChoicedStockInitialize) {
	CSetChinaChoicedStock setChinaChoicedStock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setChinaChoicedStock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setChinaChoicedStock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setChinaChoicedStock.GetDefaultSQL(), _T("[choiced_stock]"));
}

TEST(SetInitializeTest, TestWorldChoicedForexInitialize) {
	CSetWorldChoicedForex setWorldChoicedForex;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldChoicedForex.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldChoicedForex.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setWorldChoicedForex.GetDefaultSQL(), _T("[choiced_forex]"));
}

TEST(SetInitializeTest, TestWorldChoicedCryptoInitialize) {
	CSetWorldChoicedCrypto setWorldChoicedCrypto;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldChoicedCrypto.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldChoicedCrypto.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setWorldChoicedCrypto.GetDefaultSQL(), _T("[choiced_crypto]"));
}

TEST(SetInitializeTest, TestWorldChoicedStockInitialize) {
	CSetWorldChoicedStock setWorldChoicedStock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldChoicedStock.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldChoicedStock.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setWorldChoicedStock.GetDefaultSQL(), _T("[choiced_stock]"));
}

TEST(SetInitializeTest, TestCountryInitialize) {
	CSetCountry setCountry;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setCountry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setCountry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setCountry.GetDefaultSQL(), _T("[country_list]"));
}

TEST(SetInitializeTest, TestEconomicCalendarInitialize) {
	CSetEconomicCalendar setEconomicCalendar;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setEconomicCalendar.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setEconomicCalendar.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setEconomicCalendar.GetDefaultSQL(), _T("[economic_calendar]"));
}

TEST(SetInitializeTest, TestEPSSurpriseInitialize) {
	CSetEPSSurprise setEPSSurprise;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setEPSSurprise.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setEPSSurprise.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setEPSSurprise.GetDefaultSQL(), _T("[eps_surprise]"));
}

TEST(SetInitializeTest, TestInsiderTransactionInitialize) {
	CSetInsiderTransaction setInsiderTransaction;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setInsiderTransaction.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setInsiderTransaction.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setInsiderTransaction.GetDefaultSQL(), _T("[insider_transaction]"));
}

TEST(SetInitializeTest, TestNaicsIndustryInitialize) {
	CSetNaicsIndustry setNaicsIndustry;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setNaicsIndustry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setNaicsIndustry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setNaicsIndustry.GetDefaultSQL(), _T("[naics_industry]"));
}

TEST(SetInitializeTest, TestSICIndustryInitialize) {
	CSetSICIndustry setSICIndustry;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setSICIndustry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setSICIndustry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setSICIndustry.GetDefaultSQL(), _T("[sic_industry]"));
}

TEST(SetInitializeTest, TestTiingoIndustryInitialize) {
	CSetTiingoIndustry setTiingoIndustry;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setTiingoIndustry.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setTiingoIndustry.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setTiingoIndustry.GetDefaultSQL(), _T("[tiingo_industry]"));
}

TEST(SetInitializeTest, TestTiingoStockInitialize) {
	CSetTiingoStock setTiingoStock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setTiingoStock.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setTiingoStock.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setTiingoStock.GetDefaultSQL(), _T("[tiingo_stock_fundanmental]"));
}

TEST(SetInitializeTest, TestWorldStockInitialize) {
	CSetWorldStock setWorldStock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldStock.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldStock.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;

	EXPECT_STREQ(setWorldStock.GetDefaultSQL(), _T("[stock_profile]"));
}