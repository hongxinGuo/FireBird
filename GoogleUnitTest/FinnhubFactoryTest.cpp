#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubFactory.h"

#include"FinnhubEconomicCountryList.h"
#include"FinnhubEconomicCalendar.h"
#include"FinnhubCryptoExchange.h"
#include"FinnhubForexExchange.h"
#include"FinnhubCryptoSymbolProduct.h"
#include"FinnhubStockSymbolProduct.h"
#include"FinnhubForexSymbolProduct.h"
#include"FinnhubCompanyProfile.h"
#include"FinnhubCompanyProfileConcise.h"
#include"FinnhubCompanyPeer.h"
#include"FinnhubCompanyInsiderTransaction.h"
#include"FinnhubCryptoDayLine.h"
#include"FinnhubForexDayLine.h"
#include"FinnhubStockPriceQuote.h"
#include"FinnhubStockEstimatesEPSSurprise.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubFactoryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CFinnhubFactory factory;
	};

	TEST_F(CFinnhubFactoryTest, TestCreateProduct) {
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__)->IsKindOf(RUNTIME_CLASS(CFinnhubEconomicCountryList)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__)->IsKindOf(RUNTIME_CLASS(CFinnhubEconomicCountryList)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_CALENDAR__)->IsKindOf(RUNTIME_CLASS(CFinnhubEconomicCalendar)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_EXCHANGE__)->IsKindOf(RUNTIME_CLASS(CFinnhubCryptoExchange)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_EXCHANGE__)->IsKindOf(RUNTIME_CLASS(CFinnhubForexExchange)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CFinnhubStockSymbolProduct)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CFinnhubCryptoSymbolProduct)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbolProduct)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_PROFILE__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanyProfile)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_PROFILE_CONCISE__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanyProfileConcise)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __PEERS__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanyPeer)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __INSIDER_TRANSACTION__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanyInsiderTransaction)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_CANDLES__)->IsKindOf(RUNTIME_CLASS(CFinnhubCryptoDayLine)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_CANDLES__)->IsKindOf(RUNTIME_CLASS(CFinnhubForexDayLine)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_QUOTE__)->IsKindOf(RUNTIME_CLASS(CFinnhubStockPriceQuote)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_ESTIMATES_EPS_SURPRISE__)->IsKindOf(RUNTIME_CLASS(CFinnhubStockEstimatesEPSSurprise)));
	}
}