#include"pch.h"

#include"GeneralCheck.h"

#include"TimeConvert.h"
#include"WorldMarket.h"

#include"FinnhubInquiryType.h"

#include"ProductFinnhub.h"
#include"finnhubInaccessibleExchange.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubProductTest : public ::testing::Test {
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
		CProductFinnhub finnhubProduct;
	};

	TEST_F(CFinnhubProductTest, TestAddInaccessibleExchange) {
		finnhubProduct.SetProductType(__STOCK_SYMBOLS__);
		finnhubProduct.SetInquiringExchange(_T("AA")); // 新的交易所代码

		finnhubProduct.AddInaccessibleExchange();

		CInaccessibleExchangesPtr pExchange = gl_finnhubInaccessibleExchange.m_mapInaccessibleExchange.at(__STOCK_SYMBOLS__);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AA")));

		finnhubProduct.SetProductType(__STOCK_SYMBOLS__);
		finnhubProduct.SetInquiringExchange(_T("AB")); // 新的交易所代码
		finnhubProduct.AddInaccessibleExchange();

		pExchange = gl_finnhubInaccessibleExchange.m_mapInaccessibleExchange.at(__STOCK_SYMBOLS__);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AB")));

		// 恢复原状
		pExchange->DeleteExchange(_T("AA"));
		pExchange->DeleteExchange(_T("AB"));
	}
}