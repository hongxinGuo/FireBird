#include"pch.h"
#include"globedef.h"

#include"SetPotenDailyBriefing.h"
#include"SetCrweberIndex.h"

#include"SetDayLineBasicInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetCryptoDayLine.h"
#include"SetWorldStockDayLine.h"
#include"SetForexDayLine.h"

#include"SetChinaChoicedStock.h"
#include"SetWorldChoicedForex.h"
#include"SetWorldChoicedCrypto.h"
#include"SetWorldChoicedStock.h"

TEST(SetInitializeTest, TestPotenDailyBriefingInitialize) {
	CSetPotenDailyBriefing setPotenDailyBriefing;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setPotenDailyBriefing.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setPotenDailyBriefing.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setPotenDailyBriefing.GetDefaultSQL(), _T("[potendailybriefing]"));
}

TEST(SetInitializeTest, TestCrweberIndexInitialize) {
	CSetCrweberIndex setCrweberIndex;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setCrweberIndex.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setCrweberIndex.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setCrweberIndex.GetDefaultSQL(), _T("[crweberIndex]"));
}

TEST(SetInitializeTest, TestDayLineBasicInfoInitialize) {
	CSetDayLineBasicInfo setDayLineBasicInfo;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setDayLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setDayLineBasicInfo.GetDefaultSQL(), _T("[dayline]"));
}

TEST(SetInitializeTest, TestWeekLineBasicInfoInitialize) {
	CSetWeekLineBasicInfo setWeekLineBasicInfo;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setWeekLineBasicInfo.GetDefaultSQL(), _T("[weekline]"));
}

TEST(SetInitializeTest, TestWorldStockDayLineInitialize) {
	CSetWorldStockDayLine setWorldStockDayLine;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldStockDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldStockDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setWorldStockDayLine.GetDefaultSQL(), _T("[stock_dayline]"));
}

TEST(SetInitializeTest, TestCryptoDayLineInitialize) {
	CSetCryptoDayLine setCryptoDayLine;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setCryptoDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setCryptoDayLine.GetDefaultSQL(), _T("[crypto_dayline]"));
}

TEST(SetInitializeTest, TestForexDayLineInitialize) {
	CSetForexDayLine setForexDayLine;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setForexDayLine.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setForexDayLine.GetDefaultSQL(), _T("[forex_dayline]"));
}

TEST(SetInitializeTest, TestChinaChoicedStockInitialize) {
	CSetChinaChoicedStock setChinaChoicedStock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setChinaChoicedStock.GetDefaultConnect(), _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setChinaChoicedStock.GetDefaultConnect(), _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setChinaChoicedStock.GetDefaultSQL(), _T("[choiced_stock]"));
}

TEST(SetInitializeTest, TestWorldChoicedForexInitialize) {
	CSetWorldChoicedForex setWorldChoicedForex;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldChoicedForex.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldChoicedForex.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setWorldChoicedForex.GetDefaultSQL(), _T("[choiced_forex]"));
}

TEST(SetInitializeTest, TestWorldChoicedCryptoInitialize) {
	CSetWorldChoicedCrypto setWorldChoicedCrypto;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldChoicedCrypto.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldChoicedCrypto.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setWorldChoicedCrypto.GetDefaultSQL(), _T("[choiced_crypto]"));
}

TEST(SetInitializeTest, TestWorldChoicedStockInitialize) {
	CSetWorldChoicedStock setWorldChoicedStock;

	EXPECT_FALSE(gl_fNormalMode);
	EXPECT_STREQ(setWorldChoicedStock.GetDefaultConnect(), _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"));
	gl_fNormalMode = true;
	EXPECT_STREQ(setWorldChoicedStock.GetDefaultConnect(), _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	gl_fNormalMode = false;
	gl_fTestMode = true;

	EXPECT_STREQ(setWorldChoicedStock.GetDefaultSQL(), _T("[choiced_stock]"));
}