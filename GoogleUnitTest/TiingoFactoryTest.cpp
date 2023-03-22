#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubInquiryType.h"

#include"TiingoFactory.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoFactoryTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CTiingoFactory factory;
	};

	TEST_F(CTiingoFactoryTest, TestCreateProduct) {
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_SYMBOLS_)).name(), _T("class CProductTiingoCryptoSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket.get(), STOCK_PRICE_CANDLES_)).name(), _T("class CProductTiingoStockDayLine"));
	}
}
