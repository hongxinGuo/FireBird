#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCompanySymbolProduct.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanySymbolProductTest : public ::testing::Test
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
		CFinnhubCompanySymbolProduct companySymbolProduct;
	};

	TEST_F(CFinnhubCompanySymbolProductTest, TestInitialize) {
		EXPECT_EQ(companySymbolProduct.GetIndex(), -1);
		EXPECT_STREQ(companySymbolProduct.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/symbol?exchange="));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestCreatMessage) {
		companySymbolProduct.SetMarket(gl_pWorldMarket.get());
		companySymbolProduct.SetIndex(1);
		EXPECT_STREQ(companySymbolProduct.CreatMessage(), companySymbolProduct.GetInquiringStr() + gl_pWorldMarket->GetExchangeCode(1));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}