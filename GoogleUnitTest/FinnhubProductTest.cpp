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
		const CString str = _T("{\"no error\":\"You don't have access to this resource.\"}");
		pWebData->Test_SetBuffer_(str);
		EXPECT_FALSE(finnhubProduct.IsNoRightToAccess());
		finnhubProduct.SetReceivedDataStatus(NO_ACCESS_RIGHT_);
		EXPECT_TRUE(finnhubProduct.IsNoRightToAccess());

		EXPECT_TRUE(finnhubProduct.CheckAccessRight(pWebData)) << "��Ȩ��ȡ";
		EXPECT_FALSE(finnhubProduct.IsNoRightToAccess());
	}

	TEST_F(CFinnhubProductTest, TestCheckNoRightToAccess2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString str = _T("{\"error\":\"You don't have access to this resource.\"}");
		pWebData->Test_SetBuffer_(str);
		EXPECT_FALSE(finnhubProduct.IsNoRightToAccess());

		EXPECT_FALSE(finnhubProduct.CheckAccessRight(pWebData)) << "��Ȩ��ȡ";
		EXPECT_TRUE(finnhubProduct.IsNoRightToAccess()) << "���ô�״̬";
	}

	TEST_F(CFinnhubProductTest, TestAddInaccessibleExchange) {
		finnhubProduct.SetInquireType(STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AA")); // �µĽ���������

		finnhubProduct.AddInaccessibleExchange();

		CInaccessibleExchangesPtr pExchange = gl_finnhubInaccessibleExchange.GetExchange(STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveSymbol(_T("AA")));
		EXPECT_EQ(pExchange->SymbolSize(), 1) << "������һ��������";

		finnhubProduct.SetInquireType(STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // �µĽ���������

		finnhubProduct.AddInaccessibleExchange();

		pExchange = gl_finnhubInaccessibleExchange.GetExchange(STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveSymbol(_T("AB")));
		EXPECT_EQ(pExchange->SymbolSize(), 2) << "���ӵڶ���������";

		finnhubProduct.SetInquireType(STOCK_SYMBOLS_);
		finnhubProduct.SetInquiringExchange(_T("AB")); // �Ѵ��������ݼ��еĽ���������

		finnhubProduct.AddInaccessibleExchange();

		pExchange = gl_finnhubInaccessibleExchange.GetExchange(STOCK_SYMBOLS_);
		EXPECT_TRUE(pExchange->HaveSymbol(_T("AB")));
		EXPECT_EQ(pExchange->SymbolSize(), 2) << "AB�������Ѵ��������ݼ��У��ʶ�û������";

		// �ָ�ԭ״
		pExchange->DeleteSymbol(_T("AA"));
		pExchange->DeleteSymbol(_T("AB"));

		gl_finnhubInaccessibleExchange.SetUpdateDB(false);
	}
}
