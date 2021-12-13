#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubFactory.h"

#include"FinnhubEconomicCountryList.h"
#include"FinnhubCryptoExchange.h"
#include"FinnhubForexExchange.h"
#include"FinnhubCryptoSymbolProduct.h"
#include"FinnhubCompanySymbolProduct.h"
#include"FinnhubForexSymbolProduct.h"
#include"FinnhubCompanyProfile.h"
#include"FinnhubCompanyProfileConcise.h"
#include"FinnhubCompanyPeer.h"
#include"FinnhubCompanyInsiderTransaction.h"
#include"FinnhubCryptoDayLine.h"
#include"FinnhubForexDayLine.h"
#include"FinnhubStockPriceQuote.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubFactoryTest : public ::testing::Test
	{
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
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__)->GetName(), _T("Finnhub economic country list"));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__)->IsKindOf(RUNTIME_CLASS(CFinnhubEconomicCountryList)));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_CALENDAR__)->GetName(), _T("Finnhub economic calendar"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_EXCHANGE__)->GetName(), _T("Finnhub crypto exchange"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_EXCHANGE__)->GetName(), _T("Finnhub forex exchange"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_SYMBOLS__)->GetName(), _T("Finnhub company symbols"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_SYMBOLS__)->GetName(), _T("Finnhub crypto symbols"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_SYMBOLS__)->GetName(), _T("Finnhub forex symbols"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_PROFILE__)->GetName(), _T("Finnhub company profile"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_PROFILE_CONCISE__)->GetName(), _T("Finnhub company profile concise"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __PEERS__)->GetName(), _T("Finnhub company peer"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __INSIDER_TRANSACTION__)->GetName(), _T("Finnhub company insider transaction"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_CANDLES__)->GetName(), _T("Finnhub crypto dayline"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __FOREX_CANDLES__)->GetName(), _T("Finnhub forex dayline"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_QUOTE__)->GetName(), _T("Finnhub stock price quote"));
		EXPECT_STREQ(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_ESTIMATES_EPS_SURPRISE__)->GetName(), _T("Finnhub estimates EPS surprise"));

		EXPECT_EQ(factory.CreateProduct(gl_pWorldMarket.get(), 0), nullptr) << "Finnhub factory目前只有两种产品";
		//EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Finnhub product未实现"));

		EXPECT_EQ(gl_systemMessage.GetErrorMessageDequeSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Finnhub product未实现"));
	}
}