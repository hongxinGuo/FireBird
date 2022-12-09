#include"pch.h"

#include"GeneralCheck.h"

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
#include"ProductFinnhubCompanyNews.h"
#include"ProductFinnhubCompanyPeer.h"
#include"ProductFinnhubCompanyBasicFinancial.h"
#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCompanyInsiderSentiment.h"
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
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _ECONOMIC_COUNTRY_LIST_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubEconomicCountryList)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _ECONOMIC_COUNTRY_LIST_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubEconomicCountryList)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _ECONOMIC_CALENDAR_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubEconomicCalendar)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _CRYPTO_EXCHANGE_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoExchange)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _FOREX_EXCHANGE_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexExchange)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _STOCK_SYMBOLS_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockSymbol)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _CRYPTO_SYMBOLS_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoSymbol)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _FOREX_SYMBOLS_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _COMPANY_PROFILE_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfile)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _COMPANY_PROFILE_CONCISE_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _COMPANY_NEWS_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyNews)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _PEERS_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _BASIC_FINANCIALS_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyBasicFinancial)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _INSIDER_TRANSACTION_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderTransaction)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _INSIDER_SENTIMENT_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyInsiderSentiment)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _CRYPTO_CANDLES_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoDayLine)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _FOREX_CANDLES_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _STOCK_PRICE_QUOTE_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), _STOCK_ESTIMATES_EPS_SURPRISE_)->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
	}
}