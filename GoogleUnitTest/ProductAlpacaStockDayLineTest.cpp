#include "pch.h"

#include <gtest/gtest.h>

#include "ProductAlpacaStockDayLine.h"
#include "GeneralCheck.h"
#include "TiingoCandleLine.h"
#include "TiingoStock.h"

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
		const std::string json1 = R"({
        "bars": {
            "AAPL": [
                { "t": "2020-01-02T00:00:00Z", "o": 1.1, "h": 1.2, "l": 1.0, "c": 1.15, "v": 100, "vw": 1.12 },
                { "t": "2020-01-03T00:00:00Z", "o": 2.1, "h": 2.2, "l": 2.0, "c": 2.15, "v": 200, "vw": 2.12 }
            ]
        },
				"next_page_token": null
    })";

		const std::string json2 = R"({
        "bars": {
            "AAPL": [
                { "t": "2020-01-02T00:00:00Z", "o": 1.1, "h": 1.2, "l": 1.0, "c": 1.15, "v": 100, "vw": 1.12 },
                { "t": "2020-01-03T00:00:00Z", "o": 2.1, "h": 2.2, "l": 2.0, "c": 2.15, "v": 200, "vw": 2.12 }
            ]
        },
				"next_page_token": "UklHfER8MTc4NTkwMjQwMDAwMDAwMDAwMA"
    })";

		auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();
		cpr::Response r;
		r.text = json1;

		product.Parse(pvDayLine, r, "AAPL");
		EXPECT_TRUE(product.IsDataEnded());

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

		r.text = json2;

		product.Parse(pvDayLine, r, "AAPL");
		EXPECT_FALSE(product.IsDataEnded());
	}

	// Helper to create a CTiingoCandleLine with date and close
	static CTiingoCandleLine MakeCandle(unsigned ymd, long close) {
		CTiingoCandleLine c;
		c.SetDate(ymd);
		c.SetClose(static_cast<long long>(close));
		return c;
	}

	TEST_F(CProductAlpacaStockDayLineTest, Identity_NoSplit) {
		CProductAlpacaStockDayLine product;

		vector<CTiingoCandleLine> raw;
		vector<CTiingoCandleLine> withSplit;

		raw.push_back(MakeCandle(20230103, 100));
		raw.push_back(MakeCandle(20230104, 110));

		// withSplit is identical -> no split expected (factor == 1.0)
		withSplit.push_back(MakeCandle(20230103, 100));
		withSplit.push_back(MakeCandle(20230104, 110));

		product.CalculateSplitFactor(raw, withSplit);

		ASSERT_EQ(raw.size(), withSplit.size());
		for (size_t i = 0; i < raw.size(); ++i) {
			double factor = raw[i].GetSplitFactor();
			// applying factor to raw should match withSplit
			raw[i].AdjustByFactor(factor);
			EXPECT_EQ(raw[i].GetClose(), withSplit[i].GetClose());
		}
	}

	TEST_F(CProductAlpacaStockDayLineTest, UniformTwoForOneSplit) {
		CProductAlpacaStockDayLine product;

		vector<CTiingoCandleLine> raw;
		vector<CTiingoCandleLine> withSplit;

		// prepare two-day series where raw closes are twice the adjusted closes
		raw.push_back(MakeCandle(20230103, 200)); // before adjustment factor applied
		raw.push_back(MakeCandle(20230104, 150));

		withSplit.push_back(MakeCandle(20230103, 100)); // after forward split adjustment
		withSplit.push_back(MakeCandle(20230104, 75));

		product.CalculateSplitFactor(raw, withSplit);

		ASSERT_EQ(raw.size(), withSplit.size());

		EXPECT_EQ(raw.at(0).GetClose(), withSplit.at(0).GetClose());
		EXPECT_EQ(raw.at(1).GetClose(), withSplit.at(1).GetClose());
		EXPECT_EQ(raw.at(1).GetSplitFactor(), 2); // split factor should be 2 for the second day
	}
} // namespace FireBirdTest
