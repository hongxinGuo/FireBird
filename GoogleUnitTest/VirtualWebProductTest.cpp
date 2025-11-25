#include"pch.h"

#include "ChinaMarket.h"
#include "FinnhubInaccessibleExchange.h"
#include "FinnhubInquiryType.h"
#include"GeneralCheck.h"
#include "ProductFinnhub.h"
#include"WorldMarket.h"
#include"VirtualWebProduct.h"
#include"TestWebData.h"

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
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualWebProduct webProduct;
		CProductFinnhub finnhubWebProduct;
	};

	TEST_F(CVirtualWebProductTest, TestInitialize) {
		EXPECT_EQ(webProduct.GetIndex(), 0);
		EXPECT_STREQ(typeid(webProduct).name(), "class CVirtualWebProduct");
		EXPECT_EQ(webProduct.GetInquiry(), "");
		EXPECT_EQ(webProduct.GetInquiryFunction(), "");
		EXPECT_EQ(webProduct.GetInquiringExchange(), "ALL");
		EXPECT_EQ(webProduct.GetInquireType(), -1);
	}

	TEST_F(CVirtualWebProductTest, TestIsVoidJson1) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{}";
		pWebData->Test_SetBuffer_(strData);

		EXPECT_TRUE(pWebData->IsVoidJson());
	}

	TEST_F(CVirtualWebProductTest, TestIsVoidJson2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "[]";
		pWebData->Test_SetBuffer_(strData);

		EXPECT_TRUE(pWebData->IsVoidJson());
	}

	TEST_F(CVirtualWebProductTest, TestIsVoidJson3) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{abcdefg}";
		pWebData->Test_SetBuffer_(strData);

		EXPECT_FALSE(pWebData->IsVoidJson());
	}

	TEST_F(CVirtualWebProductTest, TestCheckVoidJson1) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{}";
		pWebData->Test_SetBuffer_(strData);
		EXPECT_TRUE(pWebData->IsVoidJson());

		EXPECT_TRUE(webProduct.IsVoidJson(pWebData));

		EXPECT_TRUE(webProduct.IsVoidData());
	}

	TEST_F(CVirtualWebProductTest, TestCheckVoidJson2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{abc}";
		pWebData->Test_SetBuffer_(strData);
		EXPECT_FALSE(pWebData->IsVoidJson());

		EXPECT_FALSE(webProduct.IsVoidJson(pWebData));

		EXPECT_FALSE(webProduct.IsVoidData());
	}

	TEST_F(CVirtualWebProductTest, TestGetReceivedDataStatus) {
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), GOOD_DATA__);

		webProduct.SetReceivedDataStatus(NO_ACCESS_RIGHT_);
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), NO_ACCESS_RIGHT_);

		webProduct.SetReceivedDataStatus(VOID_DATA_);
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), VOID_DATA_);
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible1) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		finnhubWebProduct.__Test_checkAccessRight(pWebData);

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible());
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible2) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{\"error1\":\"You don't have access to this resourc.\"}";
		pWebData->Test_SetBuffer_(strData);
		finnhubWebProduct.__Test_checkAccessRight(pWebData);

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible()) << "非拒绝提供信息";
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible3) {
		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{\"error\":\"You don't have access to this resource.\"}";
		pWebData->Test_SetBuffer_(strData);
		finnhubWebProduct.SetInquiringExchange("US");
		finnhubWebProduct.__Test_checkAccessRight(pWebData);

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible()) << "US交易所";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible4) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, "SZ")) << "未加入SZ交易所";

		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{\"error\":\"You don't have access to this resource.\"}";
		pWebData->Test_SetBuffer_(strData);
		finnhubWebProduct.SetInquiringExchange("SZ");
		finnhubWebProduct.SetInquireType(STOCK_PRICE_CANDLES_);
		finnhubWebProduct.__Test_checkAccessRight(pWebData);

		EXPECT_TRUE(finnhubWebProduct.CheckInaccessible()) << "将SZ交易所列入禁入名单";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, "SZ")) << "已加入SZ交易所";

		// 恢复原状
		gl_finnhubInaccessibleExchange.GetExchange(STOCK_PRICE_CANDLES_)->DeleteSymbol("SZ");
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible5) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, "US")) << "未加入US交易所";

		const CWebDataPtr pWebData = make_shared<CWebData>();
		const string strData = "{\"error\":\"You don't have access to this resource.\"}";
		pWebData->Test_SetBuffer_(strData);
		finnhubWebProduct.SetInquiringExchange("AD");
		finnhubWebProduct.SetInquireType(STOCK_PRICE_CANDLES_);
		finnhubWebProduct.__Test_checkAccessRight(pWebData);

		finnhubWebProduct.CheckInaccessible(); // 重置内部静态数据

		finnhubWebProduct.SetInquiringExchange("US");

		for (int i = 0; i < 10; i++) {
			finnhubWebProduct.__Test_checkAccessRight(pWebData);

			finnhubWebProduct.CheckInaccessible();
		}
		finnhubWebProduct.__Test_checkAccessRight(pWebData);

		EXPECT_TRUE(finnhubWebProduct.CheckInaccessible()) << "连续10次后，将US交易所列入禁入名单";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.HaveExchange(STOCK_PRICE_CANDLES_, "US")) << "已加入US交易所";

		// 恢复原状
		gl_finnhubInaccessibleExchange.GetExchange(STOCK_PRICE_CANDLES_)->DeleteSymbol("US");
		gl_finnhubInaccessibleExchange.GetExchange(STOCK_PRICE_CANDLES_)->DeleteSymbol("AD");
	}
}
