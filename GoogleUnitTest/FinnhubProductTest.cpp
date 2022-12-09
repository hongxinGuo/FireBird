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
		finnhubProduct.SetProductType(_STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AA")); // 新的交易所代码

		EXPECT_TRUE(finnhubProduct.AddInaccessibleExchangeIfNeeded());

		CInaccessibleExchangesPtr pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(_STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AA")));
		EXPECT_EQ(pExchange->ExchangeSize(), 1) << "增加了一个交易所";

		finnhubProduct.SetProductType(_STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // 新的交易所代码
		EXPECT_TRUE(finnhubProduct.AddInaccessibleExchangeIfNeeded());

		pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(_STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AB")));
		EXPECT_EQ(pExchange->ExchangeSize(), 2) << "增加第二个交易所";

		finnhubProduct.SetProductType(_STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // 已存在于数据集中的交易所代码
		EXPECT_FALSE(finnhubProduct.AddInaccessibleExchangeIfNeeded()) << "存在于数据集中的交易所不再重复添加";

		pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(_STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AB")));
		EXPECT_EQ(pExchange->ExchangeSize(), 2) << "AB交易所已存在于数据集中，故而没有增加";

		// 恢复原状
		pExchange->DeleteExchange(_T("AA"));
		pExchange->DeleteExchange(_T("AB"));
	}
}