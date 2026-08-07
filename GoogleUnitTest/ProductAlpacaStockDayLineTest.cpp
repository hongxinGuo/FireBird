#include "pch.h"

#include <gtest/gtest.h>

#include "ProductAlpacaStockDayLine.h"
#include "WebData.h"
#include "GeneralCheck.h"
#include "TiingoCandleLine.h"
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

	TEST_F(CProductAlpacaStockDayLineTest, ParseBarsFormat) {
		const string json = R"({
    "bars": {
        "RIG": [
            {
                "c": 4.24,
                "h": 4.285,
                "l": 4.085,
                "n": 1426,
                "o": 4.155,
                "t": "2026-01-02T05:00:00Z",
                "v": 1219479,
                "vw": 4.226218
            },
            {
                "c": 4.315,
                "h": 4.415,
                "l": 4.105,
                "n": 2731,
                "o": 4.415,
                "t": "2026-01-05T05:00:00Z",
                "v": 1584278,
                "vw": 4.261628
            }
        ]
    },
    "next_page_token": "UklHfER8MTc2NzY3NTYwMDAwMDAwMDAwMA=="
})";

		auto pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(json);

		auto pvDayLine = product.ParseWebData(pWebData);

		// Expect two bars parsed
		ASSERT_EQ(pvDayLine->size(), 2u);

		// Tiingo stock ratio is 1,000,000 -> prices are multiplied by 1,000,000 in ParseWebData
		const int ratio = 1000000;

		// First bar assertions (2026-01-02)
		{
			const auto& dl = pvDayLine->at(0);
			EXPECT_EQ(dl.GetDate(), toLocalDays(20260102));
			EXPECT_EQ(dl.GetClose(), static_cast<long long>(4.24 * ratio));
			EXPECT_EQ(dl.GetHigh(), static_cast<long long>(4.285 * ratio));
			EXPECT_EQ(dl.GetLow(), static_cast<long long>(4.085 * ratio));
			EXPECT_EQ(dl.GetOpen(), static_cast<long long>(4.155 * ratio));
			EXPECT_EQ(dl.GetVolume(), static_cast<int64_t>(1219479));
			EXPECT_DOUBLE_EQ(dl.GetDividend(), 0.0);
			EXPECT_DOUBLE_EQ(dl.GetSplitFactor(), 1.0);
			EXPECT_EQ(dl.GetLastClose(), 0);
		}

		// Second bar assertions (2026-01-05)
		{
			const auto& dl = pvDayLine->at(1);
			EXPECT_EQ(dl.GetDate(), toLocalDays(20260105));
			EXPECT_EQ(dl.GetClose(), static_cast<long long>(4.315 * ratio));
			EXPECT_EQ(dl.GetHigh(), static_cast<long long>(4.415 * ratio));
			EXPECT_EQ(dl.GetLow(), static_cast<long long>(4.105 * ratio));
			EXPECT_EQ(dl.GetOpen(), static_cast<long long>(4.415 * ratio));
			EXPECT_EQ(dl.GetVolume(), static_cast<int64_t>(1584278));
			EXPECT_DOUBLE_EQ(dl.GetDividend(), 0.0);
			EXPECT_DOUBLE_EQ(dl.GetSplitFactor(), 1.0);
			EXPECT_EQ(dl.GetLastClose(), 0);
		}
	}

	TEST_F(CProductAlpacaStockDayLineTest, ParseWebData_NewBarsFormat) {
		CProductAlpacaStockDayLine product;

		// JSON in the "bars" new format
		const std::string json = R"({
        "bars": {
            "RIG": [
                {
                    "t": "2026-01-02T05:00:00Z",
                    "c": 4.24,
                    "h": 4.285,
                    "l": 4.085,
                    "o": 4.155,
                    "v": 1219479,
                    "vw": 4.226218,
                    "divCash": 0.0,
                    "splitFactor": 1.0
                }
            ]
        }
    })";

		auto pWeb = std::make_shared<CWebData>();
		pWeb->Test_SetBuffer_(json);
		pWeb->SetStockCode("RIG");

		auto pvDayLines = product.ParseWebData(pWeb);

		ASSERT_NE(pvDayLines, nullptr);
		ASSERT_EQ(pvDayLines->size(), 1u);

		const auto& dl = pvDayLines->at(0);

		// Date parsed from "2026-01-02T05:00:00Z" => 20260102
		EXPECT_EQ(dl.GetDate(), toLocalDays(20260102));

		// CTiingoStock default ratio is 1,000,000 (see TiingoStock::GetRatio).
		const long long ratio = 1000000LL;
		EXPECT_EQ(dl.GetClose(), static_cast<long long>(4.24 * ratio));
		EXPECT_EQ(dl.GetHigh(), static_cast<long long>(4.285 * ratio));
		EXPECT_EQ(dl.GetLow(), static_cast<long long>(4.085 * ratio));
		EXPECT_EQ(dl.GetOpen(), static_cast<long long>(4.155 * ratio));
		EXPECT_EQ(dl.GetVolume(), static_cast<long long>(1219479));
		EXPECT_DOUBLE_EQ(dl.GetDividend(), 0.0);
		EXPECT_DOUBLE_EQ(dl.GetSplitFactor(), 1.0);
	}
}
