#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"TiingoStockSymbols.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CTiingoStockSymbolsTest : public ::testing::Test
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
		CTiingoStockSymbols stockSymbols;
	};

	TEST_F(CTiingoStockSymbolsTest, TestInitialize) {
		EXPECT_EQ(stockSymbols.GetIndex(), -1);
		EXPECT_STREQ(stockSymbols.GetInquiringStr(), _T("https://api.tiingo.com/tiingo/fundamentals/meta?"));
	}

	TEST_F(CTiingoStockSymbolsTest, TestCreatMessage) {
		EXPECT_STREQ(stockSymbols.CreatMessage(), _T("https://api.tiingo.com/tiingo/fundamentals/meta?"));
	}

	TEST_F(CTiingoStockSymbolsTest, TestProcessWebData) {
		// not implmented yet
	}
}