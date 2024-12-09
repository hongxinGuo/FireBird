#include"pch.h"

 

#include"GeneralCheck.h"

#include"WorldMarket.h"
import FireBird.FinnhubInquiryType;

import FireBird.Factory.Tiingo;

using namespace testing;

namespace FireBirdTest {
	class CTiingoFactoryTest : public Test {
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
		CTiingoFactory factory;
	};

	TEST_F(CTiingoFactoryTest, TestCreateProduct) {
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_)).name(), _T("class CProductTiingoStockProfile"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_)).name(), _T("class CProductTiingoCryptoSymbol"));
		EXPECT_STREQ(typeid(*factory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_CANDLES_)).name(), _T("class CProductTiingoStockDayLine"));
	}
}
