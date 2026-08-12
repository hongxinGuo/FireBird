#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"

#include"SystemMessage.h"

#include"AccessoryDataSource.h"
#include"ProductIndexNasdaq100Stocks.h"
#include "WebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductIndexNasdaq100StocksTest : public Test {
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
		CProductIndexNasdaq100Stocks nasdaq100Stocks;
	};

	TEST_F(CProductIndexNasdaq100StocksTest, TestInitialize) {
		EXPECT_EQ(nasdaq100Stocks.GetIndex(), 0);
		EXPECT_EQ(nasdaq100Stocks.GetInquiryFunction(), "https://www.slickcharts.com/nasdaq100");
	}

	TEST_F(CProductIndexNasdaq100StocksTest, TestCreatMessage) {
		EXPECT_EQ(nasdaq100Stocks.CreateMessage()->front(), nasdaq100Stocks.GetInquiryFunction());
	}

	TEST_F(CProductIndexNasdaq100StocksTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks());
		nasdaq100Stocks.UpdateSystemStatus();

		EXPECT_FALSE(gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks());
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInnerSystemInformationMessage(), "Nasdaq 100 stock list updated");

		// 恢复原状
		gl_pAccessoryDataSource->SetUpdateIndexNasdaq100Stocks(true);
	}

	TEST_F(CProductIndexNasdaq100StocksTest, ParsesSymbolsCorrectly) {
		auto pWebData = std::make_shared<CWebData>();

		// 构造页面片段，必须包含函数中查找的精确前缀
		const std::string jsonArray = R"([{"name":"Nvidia","symbol":"NVDA","cik":null},{"name":"Microsoft","symbol":"MSFT","cik":null},{"name":"Apple","symbol":"AAPL","cik":null}])";
		const std::string page =
		std::string("prefix[null,{type:\"data\",data:{nasdaq100List:") + jsonArray + "}}]suffix";

		pWebData->Test_SetBuffer_(page);

		CProductIndexNasdaq100Stocks product;
		auto symbols = product.ParseIndexNasdaq100Stocks(pWebData);

		ASSERT_EQ(symbols.size(), 3u);
		EXPECT_EQ(symbols[0], "NVDA");
		EXPECT_EQ(symbols[1], "MSFT");
		EXPECT_EQ(symbols[2], "AAPL");
	}
}
