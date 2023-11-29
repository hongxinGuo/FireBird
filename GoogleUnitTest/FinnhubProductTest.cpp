#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubInquiryType.h"

#include"ProductFinnhub.h"
#include"finnhubInaccessibleExchange.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubProductTest : public Test {
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
		CProductFinnhub finnhubProduct;
	};

	TEST_F(CFinnhubProductTest, TestCheckNoRightToAccess1) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString str = _T("{\"error2\":\"You don't have access to this resource.\"}");
		pWebData->Test_SetBuffer_(str);
		pWebData->CreateJson();
		EXPECT_TRUE(pWebData->IsParsed());

		EXPECT_FALSE(finnhubProduct.CheckNoRightToAccess(pWebData));
	}

	TEST_F(CFinnhubProductTest, TestCheckNoRightToAccess2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString str = _T("{\"error\":\"You don't have access to this resource.\"}");
		pWebData->Test_SetBuffer_(str);
		pWebData->CreateJson();
		EXPECT_TRUE(pWebData->IsParsed());

		EXPECT_TRUE(finnhubProduct.CheckNoRightToAccess(pWebData));
	}

	TEST_F(CFinnhubProductTest, TestAddInaccessibleExchange) {
		finnhubProduct.SetProductType(STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AA")); // �µĽ���������

		finnhubProduct.CheckAndAddInaccessibleExchange();

		CInaccessibleExchangesPtr pExchange = gl_finnhubInaccessibleExchange.GetExchange(STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AA")));
		EXPECT_EQ(pExchange->ExchangeSize(), 1) << "������һ��������";

		finnhubProduct.SetProductType(STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // �µĽ���������

		finnhubProduct.CheckAndAddInaccessibleExchange();

		pExchange = gl_finnhubInaccessibleExchange.GetExchange(STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AB")));
		EXPECT_EQ(pExchange->ExchangeSize(), 2) << "���ӵڶ���������";

		finnhubProduct.SetProductType(STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // �Ѵ��������ݼ��еĽ���������

		finnhubProduct.CheckAndAddInaccessibleExchange();

		pExchange = gl_finnhubInaccessibleExchange.GetExchange(STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveExchange(_T("AB")));
		EXPECT_EQ(pExchange->ExchangeSize(), 2) << "AB�������Ѵ��������ݼ��У��ʶ�û������";

		// �ָ�ԭ״
		pExchange->DeleteExchange(_T("AA"));
		pExchange->DeleteExchange(_T("AB"));
	}
}
