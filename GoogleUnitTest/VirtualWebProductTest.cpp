#include"pch.h"

#include "ChinaMarket.h"
#include "FinnhubInaccessibleExchange.h"
#include "FinnhubInquiryType.h"
#include"GeneralCheck.h"
#include "ProductFinnhub.h"

#include"WorldMarket.h"

#include"VirtualWebProduct.h"

using namespace testing;

namespace FireBirdTest {
	TEST(CallableFunctionTest, TestCompareDayLine) {
		const auto p1 = make_shared<CDayLine>();
		const auto p2 = make_shared<CDayLine>();
		p1->SetDate(0);
		p2->SetDate(1);
		EXPECT_TRUE(CompareDayLineDate(p1, p2));
		p1->SetDate(1);
		p2->SetDate(1);
		EXPECT_FALSE(CompareDayLineDate(p1, p2));
		p1->SetDate(2);
		p2->SetDate(1);
		EXPECT_FALSE(CompareDayLineDate(p1, p2));
	}

	class CVirtualWebProductTest : public ::testing::Test {
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
			//EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsNeedUpdate()); //todo
			gl_finnhubInaccessibleExchange.SetUpdate(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualWebProduct webProduct;
		CProductFinnhub finnhubWebProduct;
	};

	TEST_F(CVirtualWebProductTest, TestInitialize) {
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

		EXPECT_TRUE(webProduct.IsVoidJson(pWebData));

		EXPECT_TRUE(webProduct.IsVoidData());
	}

	TEST_F(CVirtualWebProductTest, TestCheckVoidJson2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{abc}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->SetJSonContentType(true);
		EXPECT_FALSE(pWebData->IsVoidJson());

		EXPECT_FALSE(webProduct.IsVoidJson(pWebData));

		EXPECT_FALSE(webProduct.IsVoidData());
	}

	TEST_F(CVirtualWebProductTest, TestGetReceivedDataStatus) {
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), 0);

		webProduct.SetReceivedDataStatus(NO_ACCESS_RIGHT_);
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), NO_ACCESS_RIGHT_);

		webProduct.SetReceivedDataStatus(VOID_DATA_);
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), VOID_DATA_);
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible1) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		pWebData->SetParsed(false);

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible(pWebData));
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{\"error1\": \"You don't have access to this resourc.\"}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->CreateJson();
		pWebData->SetJSonContentType(true);
		pWebData->SetParsed(true);

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible(pWebData)) << "非拒绝提供信息";
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible3) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{\"error\": \"You don't have access to this resource.\"}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->CreateJson();
		pWebData->SetJSonContentType(true);
		pWebData->SetParsed(true);
		finnhubWebProduct.SetInquiringExchange(_T("US"));

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible(pWebData)) << "US交易所";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible4) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, _T("SZ"))) << "未加入SZ交易所";

		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{\"error\": \"You don't have access to this resource.\"}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->CreateJson();
		pWebData->SetJSonContentType(true);
		pWebData->SetParsed(true);
		finnhubWebProduct.SetInquiringExchange(_T("SZ"));
		finnhubWebProduct.SetProductType(STOCK_PRICE_CANDLES_);

		EXPECT_TRUE(finnhubWebProduct.CheckInaccessible(pWebData)) << "将SZ交易所列入禁入名单";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, _T("SZ"))) << "已加入SZ交易所";

		// 恢复原状
		gl_finnhubInaccessibleExchange.GetExchange(STOCK_PRICE_CANDLES_)->DeleteExchange(_T("SZ"));
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible5) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, _T("US"))) << "未加入US交易所";

		const CWebDataPtr pWebData = make_shared<CWebData>();
		const CString strData = _T("{\"error\": \"You don't have access to this resource.\"}");
		pWebData->Test_SetBuffer_(strData);
		pWebData->CreateJson();
		pWebData->SetJSonContentType(true);
		pWebData->SetParsed(true);
		finnhubWebProduct.SetInquiringExchange(_T("AD"));
		finnhubWebProduct.SetProductType(STOCK_PRICE_CANDLES_);
		finnhubWebProduct.CheckInaccessible(pWebData); // 重置内部静态数据

		finnhubWebProduct.SetInquiringExchange(_T("US"));

		for (int i = 0; i < 10; i++) {
			finnhubWebProduct.CheckInaccessible(pWebData);
		}

		EXPECT_TRUE(finnhubWebProduct.CheckInaccessible(pWebData)) << "连续10次后，将US交易所列入禁入名单";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, _T("US"))) << "已加入US交易所";

		// 恢复原状
		gl_finnhubInaccessibleExchange.GetExchange(STOCK_PRICE_CANDLES_)->DeleteExchange(_T("US"));
		gl_finnhubInaccessibleExchange.GetExchange(STOCK_PRICE_CANDLES_)->DeleteExchange(_T("AD"));
	}
}
