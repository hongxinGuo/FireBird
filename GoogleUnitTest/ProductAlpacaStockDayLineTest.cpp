#include "pch.h"

#include <gtest/gtest.h>

#include "ProductAlpacaStockDayLine.h"
#include "WebData.h"
#include "GeneralCheck.h"
#include "TiingoCandleLine.h"
#include "TiingoStock.h"
#include "TimeConvert.h"

using namespace testing;
using namespace std;
using namespace FireBirdTest;

namespace FireBirdTest {
	class CProductAlpacaStockDayLineTest : public Test {
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
		CProductAlpacaStockDayLine product;
	};

	TEST_F(CProductAlpacaStockDayLineTest, ParseSimpleBars) {
		// 构造一个简单的 JSON 响应，包含两个 bar
		const std::string json = R"({
        "bars": {
            "AAPL": [
                { "t": "2020-01-02T00:00:00Z", "o": 1.1, "h": 1.2, "l": 1.0, "c": 1.15, "v": 100, "vw": 1.12 },
                { "t": "2020-01-03T00:00:00Z", "o": 2.1, "h": 2.2, "l": 2.0, "c": 2.15, "v": 200, "vw": 2.12 }
            ]
        }
    })";

		auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();
		cpr::Response r;
		r.text = json;

		product.Parse(pvDayLine, r, "AAPL");

		ASSERT_EQ(pvDayLine->size(), 2u);

		// 解析会将价格乘以 CTiingoStock::GetRatio() (默认为 1000000)
		CTiingoStock stockPrototype;
		const long long ratio = stockPrototype.GetRatio();

		EXPECT_EQ(pvDayLine->at(0).GetVolume(), 100);
		EXPECT_EQ(pvDayLine->at(1).GetVolume(), 200);

		EXPECT_EQ(pvDayLine->at(0).GetClose(), static_cast<long long>(1.15 * ratio));
		EXPECT_EQ(pvDayLine->at(1).GetClose(), static_cast<long long>(2.15 * ratio));

		EXPECT_EQ(pvDayLine->at(0).GetOpen(), static_cast<long long>(1.1 * ratio));
		EXPECT_EQ(pvDayLine->at(1).GetOpen(), static_cast<long long>(2.1 * ratio));
	}
}
