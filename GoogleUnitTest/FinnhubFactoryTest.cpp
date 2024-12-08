#include"pch.h"

////#include"gtest/gtest.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

import FireBird.FinnhubInquiryType;

import FireBird.Factory.Finnhub;

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
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, ECONOMIC_COUNTRY_LIST_)).name(), _T("class CProductFinnhubEconomicCountryList"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, ECONOMIC_CALENDAR_)).name(), _T("class CProductFinnhubEconomicCalendar"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, CRYPTO_EXCHANGE_)).name(), _T("class CProductFinnhubCryptoExchange"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, FOREX_EXCHANGE_)).name(), _T("class CProductFinnhubForexExchange"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_)).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, FOREX_SYMBOLS_)).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, COMPANY_PROFILE_)).name(), _T("class CProductFinnhubCompanyProfile"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, COMPANY_PROFILE_CONCISE_)).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, COMPANY_NEWS_)).name(), _T("class CProductFinnhubCompanyNews"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, PEERS_)).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, BASIC_FINANCIALS_)).name(), _T("class CProductFinnhubCompanyBasicFinancial"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, INSIDER_TRANSACTION_)).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, INSIDER_SENTIMENT_)).name(), _T("class CProductFinnhubCompanyInsiderSentiment"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, CRYPTO_CANDLES_)).name(), _T("class CProductFinnhubCryptoDayLine"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, FOREX_CANDLES_)).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_QUOTE_)).name(), _T("class CProductFinnhubStockPriceQuote"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_ESTIMATES_EPS_SURPRISE_)).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockProfile"));
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockProfile"));
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockProfile"));
	}
}
