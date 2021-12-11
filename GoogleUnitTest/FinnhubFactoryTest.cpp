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
#include"FinnhubCompanyProfileConcise.h"
#include"FinnhubCompanyPeer.h"
#include"FinnhubCompanyInsiderTransaction.h"
#include"FinnhubCryptoDayLine.h"
#include"FinnhubForexDayLine.h"

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
		EXPECT_TRUE(factory.CreateProduct(__ECONOMIC_COUNTRY_LIST__)->IsKindOf(RUNTIME_CLASS(CFinnhubEconomicCountryList)));
		EXPECT_TRUE(factory.CreateProduct(__CRYPTO_EXCHANGE__)->IsKindOf(RUNTIME_CLASS(CFinnhubCryptoExchange)));
		EXPECT_TRUE(factory.CreateProduct(__FOREX_EXCHANGE__)->IsKindOf(RUNTIME_CLASS(CFinnhubForexExchange)));
		EXPECT_TRUE(factory.CreateProduct(__STOCK_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanySymbolProduct)));
		EXPECT_TRUE(factory.CreateProduct(__CRYPTO_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CFinnhubCryptoSymbolProduct)));
		EXPECT_TRUE(factory.CreateProduct(__FOREX_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CFinnhubForexSymbolProduct)));
		EXPECT_TRUE(factory.CreateProduct(__COMPANY_PROFILE_CONCISE__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanyProfileConcise)));
		EXPECT_TRUE(factory.CreateProduct(__PEERS__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanyPeer)));
		EXPECT_TRUE(factory.CreateProduct(__INSIDER_TRANSACTION__)->IsKindOf(RUNTIME_CLASS(CFinnhubCompanyInsiderTransaction)));
		EXPECT_TRUE(factory.CreateProduct(__CRYPTO_CANDLES__)->IsKindOf(RUNTIME_CLASS(CFinnhubCryptoDayLine)));
		EXPECT_TRUE(factory.CreateProduct(__FOREX_CANDLES__)->IsKindOf(RUNTIME_CLASS(CFinnhubForexDayLine)));

		EXPECT_EQ(factory.CreateProduct(__COMPANY_PROFILE__), nullptr) << "Finnhub factory目前只有两种产品";
		//EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Finnhub product未实现"));
	}
}