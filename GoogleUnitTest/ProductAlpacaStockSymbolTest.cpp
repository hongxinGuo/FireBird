#include "pch.h"

#include <gtest/gtest.h>

#include "GeneralCheck.h"
#include "ProductAlpacaStockSymbol.h"
#include "TiingoStock.h"
#include <cpr/response.h>

#include "SystemMessage.h"

using namespace testing;
using namespace std;
using namespace FireBirdTest;

namespace FireBirdTest {
	class CProductAlpacaStockSymbolTest : public Test {
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
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductAlpacaStockSymbol product;
	};

	TEST_F(CProductAlpacaStockSymbolTest, ParsesValidJson) {
		const std::string json = R"([ {
					 "id": "631be973-b807-4344-a470-829a83f7ff2e",
            "class": "us_equity",
            "exchange": "OTC",
            "symbol": "ILLMF",
            "name": "Illumin Holdings Inc. Common Stock Canada",
						"status": "active",
						"tradable" : false
				},
				{
						"id": "d2e2e46f-5bd9-4551-9bb6-466320a3ca89",
						"class" : "us_equity",
						"exchange" : "OTC",
						"symbol" : "NBSTW",
						"name" : "NEWBURY STR ACQUISITION CORP Warrant   04/30/2028",
						"status" : "active",
						"tradable" : false
				}
		])";

		cpr::Response r;
		r.text = json;
		r.status_code = 200;

		CProductAlpacaStockSymbol product;
		auto pv = product.Parse(r.text);

		ASSERT_EQ(pv->size(), 2u);
		EXPECT_EQ(pv->at(0)->GetSymbol(), "ILLMF");
		EXPECT_EQ(pv->at(0)->GetDisplaySymbol(), "Illumin Holdings Inc. Common Stock Canada");
		EXPECT_EQ(pv->at(0)->GetExchange(), "OTC");

		EXPECT_EQ(pv->at(1)->GetSymbol(), "NBSTW");
	}

	TEST_F(CProductAlpacaStockSymbolTest, ReturnsEmptyOnInvalidJson) {
		const std::string bad = "this is not json";

		cpr::Response r;
		r.text = bad;
		r.status_code = 200;

		CProductAlpacaStockSymbol product;
		auto pv = product.Parse(r.text);

		ASSERT_TRUE(pv->empty());

		// 
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
		gl_systemMessage.PopErrorMessage();
	}
}
