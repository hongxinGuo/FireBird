#include"pch.h"

#include "ChinaMarket.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"VirtualWebProduct.h"

using namespace testing;

namespace FireBirdTest {
	class CVirtualWebProductTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE(""); GeneralCheck();
		}

	protected:
		CVirtualWebProduct webProduct;
	};

	TEST_F(CVirtualWebProductTest, TestInitialize) {
		EXPECT_EQ(webProduct.GetMarket(), nullptr);
		EXPECT_EQ(webProduct.GetIndex(), -1);
		EXPECT_STREQ(typeid(webProduct).name(), _T("class CVirtualWebProduct"));
		EXPECT_STREQ(webProduct.GetInquiry(), _T(""));
		EXPECT_STREQ(webProduct.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(webProduct.GetInquiringExchange(), _T("ALL"));
		EXPECT_EQ(webProduct.GetProductType(), 0);
	}

	TEST_F(CVirtualWebProductTest, TestIsVoidJson1) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->SetJSonContentType(false);

		EXPECT_FALSE(pWebData->IsVoidJson());

		pWebData->SetJSonContentType(true);

		EXPECT_TRUE(pWebData->IsVoidJson());
	}

	TEST_F(CVirtualWebProductTest, TestIsVoidJson2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{abcdefg}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->SetJSonContentType(true);

		EXPECT_FALSE(pWebData->IsVoidJson());
	}

	TEST_F(CVirtualWebProductTest, TestCheckVoidJson1) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->SetJSonContentType(true);
		EXPECT_TRUE(pWebData->IsVoidJson());

		EXPECT_TRUE(webProduct.CheckVoidJson(pWebData));

		EXPECT_TRUE(webProduct.IsVoidData());
	}

	TEST_F(CVirtualWebProductTest, TestCheckVoidJson2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{abc}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->SetJSonContentType(true);
		EXPECT_FALSE(pWebData->IsVoidJson());

		EXPECT_FALSE(webProduct.CheckVoidJson(pWebData));

		EXPECT_FALSE(webProduct.IsVoidData());
	}
}
