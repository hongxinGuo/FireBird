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

namespace FireBirdTest {
	class CFinnhubFactoryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CFinnhubFactory factory;
	};

	TEST_F(CFinnhubFactoryTest, TestCreateProduct) {
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), ECONOMIC_COUNTRY_LIST_)).name(), _T("class CProductFinnhubEconomicCountryList"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), ECONOMIC_CALENDAR_)).name(), _T("class CProductFinnhubEconomicCalendar"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_EXCHANGE_)).name(), _T("class CProductFinnhubCryptoExchange"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), FOREX_EXCHANGE_)).name(), _T("class CProductFinnhubForexExchange"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_SYMBOLS_)).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_SYMBOLS_)).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), FOREX_SYMBOLS_)).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), COMPANY_PROFILE_)).name(), _T("class CProductFinnhubCompanyProfile"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), COMPANY_PROFILE_CONCISE_)).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), COMPANY_NEWS_)).name(), _T("class CProductFinnhubCompanyNews"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), PEERS_)).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), BASIC_FINANCIALS_)).name(), _T("class CProductFinnhubCompanyBasicFinancial"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), INSIDER_TRANSACTION_)).name(), _T("class CProductFinnhubCompanyInsiderTransaction"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), INSIDER_SENTIMENT_)).name(), _T("class CProductFinnhubCompanyInsiderSentiment"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_CANDLES_)).name(), _T("class CProductFinnhubCryptoDayLine"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), FOREX_CANDLES_)).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_PRICE_QUOTE_)).name(), _T("class CProductFinnhubStockPriceQuote"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_ESTIMATES_EPS_SURPRISE_)).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockSymbol"));
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockSymbol"));
		//EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockSymbol"));
	}
}
