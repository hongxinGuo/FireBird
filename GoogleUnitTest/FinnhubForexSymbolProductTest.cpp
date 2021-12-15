#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubForexSymbolProduct.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubForexSymbolProductTest : public ::testing::Test
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
		CFinnhubForexSymbolProduct forexSymbolProduct;
	};

	TEST_F(CFinnhubForexSymbolProductTest, TestInitialize) {
		EXPECT_EQ(forexSymbolProduct.GetIndex(), -1);
		EXPECT_STREQ(forexSymbolProduct.GetInquiringStr(), _T("https://finnhub.io/api/v1/forex/symbol?exchange="));
	}

	TEST_F(CFinnhubForexSymbolProductTest, TestCreatMessage) {
		forexSymbolProduct.SetMarket(gl_pWorldMarket.get());
		forexSymbolProduct.SetIndex(1);
		EXPECT_STREQ(forexSymbolProduct.CreatMessage(), forexSymbolProduct.GetInquiringStr() + gl_pWorldMarket->GetForexExchange(1));
	}

	TEST_F(CFinnhubForexSymbolProductTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}