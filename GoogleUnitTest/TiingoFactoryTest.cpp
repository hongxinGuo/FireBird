#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"TiingoFactory.h"
#include"ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CTiingoFactoryTest : public ::testing::Test {
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
		CTiingoFactory factory;
	};

	TEST_F(CTiingoFactoryTest, TestCreateProduct) {
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_SYMBOLS__)->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_TRUE(factory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_CANDLES__)->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
	}
}