#include"pch.h"

#include <gtest/gtest.h>

#include "ChinaMarket.h"
#include "InaccessibleSymbol.h"
#include "FinnhubInquiryType.h"
#include"GeneralCheck.h"
#include "ProductFinnhub.h"
#include"VirtualWebProduct.h"
#include"DayLine.h"

using namespace testing;
using namespace std;

namespace FireBirdTest {
	TEST(CallableFunctionTest, TestCompareDayLine) {
		CDayLine p1;
		CDayLine p2;
		p1.SetDate(0);
		p2.SetDate(1);
		EXPECT_TRUE(CompareDayLineDate(p1, p2));
		p1.SetDate(1);
		p2.SetDate(1);
		EXPECT_FALSE(CompareDayLineDate(p1, p2));
		p1.SetDate(2);
		p2.SetDate(1);
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
			gl_finnhubInaccessibleExchange.Clear();
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

	TEST_F(CVirtualWebProductTest, TestGetReceivedDataStatus) {
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), GOOD_DATA_);

		webProduct.SetReceivedDataStatus(NO_ACCESS_RIGHT_);
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), NO_ACCESS_RIGHT_);

		webProduct.SetReceivedDataStatus(VOID_DATA_);
		EXPECT_EQ(webProduct.GetReceivedDataStatus(), VOID_DATA_);
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible1) {
		finnhubWebProduct.Test_checkAccessRight_("");

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible());
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible2) {
		const string strData = R"({"error1":"You don't have access to this resource."})";
		finnhubWebProduct.Test_checkAccessRight_(strData);

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible()) << "非拒绝提供信息";
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible3) {
		const string strData = R"({"error":"You don't have access to this resource."})";
		finnhubWebProduct.SetInquiringExchange("US");
		finnhubWebProduct.Test_checkAccessRight_(strData);

		EXPECT_FALSE(finnhubWebProduct.CheckInaccessible()) << "US交易所";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible4) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(STOCK_PRICE_CANDLES_, "SZ")) << "未加入SZ交易所";

		const string strData = R"({"error":"You don't have access to this resource."})";
		finnhubWebProduct.SetInquiringExchange("SZ");
		finnhubWebProduct.SetInquireType(STOCK_PRICE_CANDLES_);
		finnhubWebProduct.Test_checkAccessRight_(strData);

		EXPECT_TRUE(finnhubWebProduct.CheckInaccessible()) << "将SZ交易所列入禁入名单";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.IsInaccessible(STOCK_PRICE_CANDLES_, "SZ")) << "已加入SZ交易所";

		// 恢复原状
		gl_finnhubInaccessibleExchange.GetInaccessible(STOCK_PRICE_CANDLES_)->DeleteSymbol("SZ");
	}

	TEST_F(CVirtualWebProductTest, TestCheckInaccessible5) {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsInaccessible(STOCK_PRICE_CANDLES_, "US")) << "未加入US交易所";

		const string strData = R"({"error":"You don't have access to this resource."})";
		finnhubWebProduct.SetInquiringExchange("AD");
		finnhubWebProduct.SetInquireType(STOCK_PRICE_CANDLES_);
		finnhubWebProduct.Test_checkAccessRight_(strData);

		finnhubWebProduct.CheckInaccessible(); // 重置内部静态数据

		finnhubWebProduct.SetInquiringExchange("US");

		for (int i = 0; i < 100; i++) {
			finnhubWebProduct.Test_checkAccessRight_(strData);

			finnhubWebProduct.CheckInaccessible();
		}
		finnhubWebProduct.Test_checkAccessRight_(strData);

		EXPECT_TRUE(finnhubWebProduct.CheckInaccessible()) << "连续100次后，将US交易所列入禁入名单";
		EXPECT_TRUE(finnhubWebProduct.IsNoRightToAccess());
		EXPECT_TRUE(gl_finnhubInaccessibleExchange.IsInaccessible(STOCK_PRICE_CANDLES_, "US")) << "已加入US交易所";

		// 恢复原状
		gl_finnhubInaccessibleExchange.GetInaccessible(STOCK_PRICE_CANDLES_)->DeleteSymbol("US");
		gl_finnhubInaccessibleExchange.GetInaccessible(STOCK_PRICE_CANDLES_)->DeleteSymbol("AD");
	}
}
