#include "pch.h"

#include <gtest/gtest.h>

#include "ProductAlpacaStockDayLine.h"
#include "GeneralCheck.h"
#include "TiingoCandleLine.h"
#include "TiingoStock.h"
#include"ContainerTiingoStock.h"
#include"TimeConvert.h"
#include"WorldMarket.h"
#include"AlpacaDataSource.h"

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

	TEST(ProductAlpacaStockDayLine_Parse2, ParsesTwoBarsSingleSymbol) {
		CProductAlpacaStockDayLine prod;
		cpr::Response r;
		r.status_code = 200;
		r.text = R"({
      "bars": {
        "RIG": [
          { "c": 5.14, "h": 5.24, "l": 5.095, "o": 5.195, "t": "2026-08-03T04:00:00Z", "v": 1398585, "vw": 5.172631 },
          { "c": 5.22, "h": 5.28, "l": 5.045, "o": 5.055, "t": "2026-08-04T04:00:00Z", "v": 2681580, "vw": 5.225845 }
        ],
        "AAPL": [
          { "c": 5.34, "h": 5.24, "l": 5.095, "o": 5.195, "t": "2026-08-03T04:00:00Z", "v": 1398585, "vw": 5.172631 },
          { "c": 5.22, "h": 5.28, "l": 5.045, "o": 5.055, "t": "2026-08-04T04:00:00Z", "v": 2681580, "vw": 5.225845 }
        ]
      },
      "next_page_token": null
    })";

		auto pv = prod.Parse(r.text, "RIG");

		ASSERT_EQ(pv->size(), 2u);
		EXPECT_EQ(pv->at(0).m_symbol, "AAPL") << "无论在json中的位置先后，解析后总是按symbol排序";
		ASSERT_EQ(pv->at(0).m_dayLine.size(), 2u);
		ASSERT_EQ(pv->at(0).m_dayLine.at(0).GetClose(), 5340000);

		EXPECT_EQ(pv->at(1).m_symbol, "RIG");
		ASSERT_EQ(pv->at(1).m_dayLine.size(), 2u);
		ASSERT_EQ(pv->at(1).m_dayLine.at(0).GetClose(), 5140000);

		const auto& first = pv->at(0).m_dayLine[0];
		EXPECT_EQ(first.GetVolume(), 1398585);

		// CTiingoStock::GetRatio() is used in Parse2; use same to compute expected scaled close value.
		long long expectedClose = static_cast<long long>(5.34 * CTiingoStock().GetRatio());
		EXPECT_EQ(first.GetClose(), expectedClose);
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

	TEST_F(CProductAlpacaStockDayLineTest, CreateMessageInternal2_ChunksWhenTotalDaysExceeds1000) {
		// 添加一个测试股票，使其起始日期足够早，触发分段（countNumber > 0）
		auto pStock = make_shared<CTiingoStock>();
		pStock->SetSymbol(string{ "Test" });
		pStock->SetActive(true);
		// 设为 1980-01-01，使 totalDays 很大，从而产生多个 limit=1000 的查询片段
		pStock->SetDayLineEndDate(toLocalDays(19800101));

		// 添加到容器
		gl_dataContainerTiingoStock.Add(pStock);

		// 创建 product 并设置 index 到刚添加的股票
		CProductAlpacaStockDayLine product;
		product.SetIndex(gl_dataContainerTiingoStock.GetOffset("Test"));

		// 调用待测试函数
		auto urls = product.CreateMessageInternal("&adjustment=raw");

		ASSERT_NE(urls, nullptr);
		EXPECT_GT(urls->size(), 1u) << "Expected CreateMessageInternal2 to produce multiple query URLs when totalDays > 1000";

		// 基本检查：URL 包含查询前缀、symbol 与 limit=1000
		for (const auto& u : *urls) {
			EXPECT_NE(u.find("https://data.alpaca.markets/v2/stocks/bars?"), string::npos);
			EXPECT_NE(u.find("symbols=Test"), string::npos);
			EXPECT_NE(u.find("limit=1000"), string::npos);
		}

		// 清理
		gl_dataContainerTiingoStock.Delete("Test");
	}

	TEST_F(CProductAlpacaStockDayLineTest, CreateMessageInternal2) {
		vector<local_days> vEndDate;
		for (size_t i = 0; i < 10; i++) { // 测试数据库中的日线结束日期较早，人工设置为较近的日期以生成多个股票的查询串。共十个。
			vEndDate.push_back(gl_dataContainerTiingoStock.GetStock(i)->GetDayLineEndDate());
			gl_dataContainerTiingoStock.GetStock(i)->SetDayLineEndDate(toLocalDays(year_month_day(gl_pWorldMarket->GetMarketDate() - std::chrono::days(5))));
		}
		gl_pAlpacaDataSource->SetInquireStockNumber(100);

		// 创建 product
		CProductAlpacaStockDayLine product;
		product.SetIndex(gl_dataContainerTiingoStock.GetOffset("A"));

		// 调用待测试函数
		auto urls = product.CreateMessageInternal("&adjustment=raw");

		ASSERT_NE(urls, nullptr);
		EXPECT_EQ(urls->size(), 1);
		EXPECT_EQ(urls->at(0).substr(0, 112), "https://data.alpaca.markets/v2/stocks/bars?symbols=A,AA,AACG,AAL&timeframe=1D&limit=1000&adjustment=raw&sort=asc") << "Alpaca代码集中不存在的股票AACT,AADI等没有申请";

		// 恢复原状
		for (size_t i = 0; i < 10; i++) {
			gl_dataContainerTiingoStock.GetStock(i)->SetDayLineEndDate(vEndDate.at(i));
		}
	}
} // namespace FireBirdTest
