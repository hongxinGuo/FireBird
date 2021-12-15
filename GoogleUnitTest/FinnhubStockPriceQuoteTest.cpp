#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubStockPriceQuote.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubStockPriceQuoteTest : public ::testing::Test
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
		CFinnhubStockPriceQuote stockPriceQuote;
	};

	TEST_F(CFinnhubStockPriceQuoteTest, TestInitialize) {
		EXPECT_EQ(stockPriceQuote.GetIndex(), -1);
		EXPECT_STREQ(stockPriceQuote.GetInquiringStr(), _T("https://finnhub.io/api/v1/quote?symbol="));
	}

	TEST_F(CFinnhubStockPriceQuoteTest, TestCreatMessage) {
		stockPriceQuote.SetMarket(gl_pWorldMarket.get());
		stockPriceQuote.SetIndex(1);
		EXPECT_STREQ(stockPriceQuote.CreatMessage(), stockPriceQuote.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
	}

	TEST_F(CFinnhubStockPriceQuoteTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}