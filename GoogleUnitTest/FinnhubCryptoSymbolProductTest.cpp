#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCryptoSymbolProduct.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCryptoSymbolProductTest : public ::testing::Test
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
		CFinnhubCryptoSymbolProduct cryptoSymbolProduct;
	};

	TEST_F(CFinnhubCryptoSymbolProductTest, TestInitialize) {
		EXPECT_EQ(cryptoSymbolProduct.GetIndex(), -1);
		EXPECT_STREQ(cryptoSymbolProduct.GetInquiringStr(), _T("https://finnhub.io/api/v1/crypto/symbol?exchange="));
	}

	TEST_F(CFinnhubCryptoSymbolProductTest, TestCreatMessage) {
		cryptoSymbolProduct.SetMarket(gl_pWorldMarket.get());
		cryptoSymbolProduct.SetIndex(1);
		EXPECT_STREQ(cryptoSymbolProduct.CreatMessage(), cryptoSymbolProduct.GetInquiringStr() + gl_pWorldMarket->GetCryptoExchange(1));
	}

	TEST_F(CFinnhubCryptoSymbolProductTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}