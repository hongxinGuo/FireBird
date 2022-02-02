#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubInquiryType.h"

#include"FinnhubFactory.h"

#include"ProductFinnhubEconomicCountryList.h"
#include"ProductFinnhubEconomicCalendar.h"
#include"ProductFinnhubCryptoExchange.h"
#include"ProductFinnhubForexExchange.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"PRoductFinnhubStockPriceQuote.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"

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
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubEconomicCountryList)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubEconomicCountryList)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_CALENDAR__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubEconomicCalendar)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_EXCHANGE__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoExchange)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_EXCHANGE__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexExchange)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockSymbol)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoSymbol)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_PROFILE__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfile)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_PROFILE_CONCISE__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __PEERS__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __INSIDER_TRANSACTION__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_CANDLES__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoDayLine)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_CANDLES__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_QUOTE__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_ESTIMATES_EPS_SURPRISE__)->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
	}
}