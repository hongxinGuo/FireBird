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
		finnhubProduct.SetInquiringExchange(_T("AA")); // �µĽ���������

		EXPECT_TRUE(finnhubProduct.AddInaccessibleExchangeIfNeeded());

		CInaccessibleExchangesPtr pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(_STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AA")));
		EXPECT_EQ(pExchange->ExchangeSize(), 1) << "������һ��������";

		finnhubProduct.SetProductType(_STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // �µĽ���������
		EXPECT_TRUE(finnhubProduct.AddInaccessibleExchangeIfNeeded());

		pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(_STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AB")));
		EXPECT_EQ(pExchange->ExchangeSize(), 2) << "���ӵڶ���������";

		finnhubProduct.SetProductType(_STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // �Ѵ��������ݼ��еĽ���������
		EXPECT_FALSE(finnhubProduct.AddInaccessibleExchangeIfNeeded()) << "���������ݼ��еĽ����������ظ����";

		pExchange = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(_STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AB")));
		EXPECT_EQ(pExchange->ExchangeSize(), 2) << "AB�������Ѵ��������ݼ��У��ʶ�û������";

		// �ָ�ԭ״
		pExchange->DeleteExchange(_T("AA"));
		pExchange->DeleteExchange(_T("AB"));
	}
}