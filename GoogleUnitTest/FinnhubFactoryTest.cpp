#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubInquiryType.h"

#include"FinnhubFactory.h"
#include"VirtualWebProduct.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubFactoryTest : public Test {
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
		CFinnhubFactory factory;
	};

	TEST_F(CFinnhubFactoryTest, TestCreateProduct) {
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, ECONOMIC_COUNTRY_LIST_)).name(), "class CProductFinnhubEconomicCountryList");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, ECONOMIC_CALENDAR_)).name(), "class CProductFinnhubEconomicCalendar");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, CRYPTO_EXCHANGE_)).name(), "class CProductFinnhubCryptoExchange");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, FOREX_EXCHANGE_)).name(), "class CProductFinnhubForexExchange");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), "class CProductFinnhubStockSymbol");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_)).name(), "class CProductFinnhubCryptoSymbol");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, FOREX_SYMBOLS_)).name(), "class CProductFinnhubForexSymbol");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, COMPANY_PROFILE_)).name(), "class CProductFinnhubCompanyProfile");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, COMPANY_PROFILE_CONCISE_)).name(), "class CProductFinnhubCompanyProfileConcise");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, COMPANY_NEWS_)).name(), "class CProductFinnhubCompanyNews");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, PEERS_)).name(), "class CProductFinnhubCompanyPeer");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, BASIC_FINANCIALS_)).name(), "class CProductFinnhubCompanyBasicFinancial");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, INSIDER_TRANSACTION_)).name(), "class CProductFinnhubCompanyInsiderTransaction");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, INSIDER_SENTIMENT_)).name(), "class CProductFinnhubCompanyInsiderSentiment");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, CRYPTO_CANDLES_)).name(), "class CProductFinnhubCryptoDayLine");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, FOREX_CANDLES_)).name(), "class CProductFinnhubForexDayLine");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_QUOTE_)).name(), "class CProductFinnhubStockPriceQuote");
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_ESTIMATES_EPS_SURPRISE_)).name(), "class CProductFinnhubStockEstimatesEPSSurprise");
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), "class CProductTiingoStockProfile");
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), "class CProductTiingoStockProfile");
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), "class CProductTiingoStockProfile");
	}
}
