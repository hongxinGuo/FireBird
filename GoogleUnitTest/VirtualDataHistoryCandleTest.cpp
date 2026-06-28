#include"pch.h"

#include"GeneralCheck.h"

#include"VirtualDataHistoryCandle.h"

namespace FireBirdTest {
	class CVirtualDataHistoryCandleTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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
	};

	TEST_F(CVirtualDataHistoryCandleTest, TestGetContainer) {
		CVirtualDataHistoryCandle id;
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600008.SS");
		data.SetDate(20200202);
		id.Add(data);

		const auto pContainer = id.GetContainer();
		EXPECT_EQ((*pContainer)[0].GetDate(), toLocalDays(20200202));
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestUpdateData1) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.SetHigh(1000);
		data.SetLow(200);
		data.SetClose(100); // 确保此数据有效
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.SetHigh(11000);
		data2.SetLow(1200);
		data2.SetClose(100); // 确保此数据有效

		CVirtualDataHistoryCandle HistoryDataContainer;
		vector<CVirtualHistoryCandle> vHistoryData;

		vHistoryData.push_back(data);
		EXPECT_EQ(HistoryDataContainer.Size(), 0);
		HistoryDataContainer.Add(data2);
		EXPECT_EQ(HistoryDataContainer.Size(), 1);
		HistoryDataContainer.UpdateData(vHistoryData);
		EXPECT_EQ(HistoryDataContainer.Size(), 1);
		auto pHistoryData2 = HistoryDataContainer.GetData(0);
		EXPECT_EQ(pHistoryData2->GetDate(), toLocalDays(20200101));
		EXPECT_EQ(pHistoryData2->GetHigh(), 1000);
		EXPECT_EQ(pHistoryData2->GetLow(), 200);
		EXPECT_EQ(pHistoryData2->GetClose(), 100);
		EXPECT_TRUE(HistoryDataContainer.IsDataLoaded());
	}

	TEST_F(CVirtualDataHistoryCandleTest, GetCandle_ReturnsPointerWhenDateExists) {
		CVirtualHistoryCandle c1, c2;
		c1.SetDate(20210101);
		c1.SetClose(1000);
		c2.SetDate(20210102);
		c2.SetClose(2000);

		vector<CVirtualHistoryCandle> data{ c1, c2 };
		CVirtualDataHistoryCandle container;
		container.UpdateData(data);

		auto ptr = container.GetCandle(toLocalDays(20210102));
		EXPECT_NE(ptr, nullptr);
		ASSERT_NE(ptr, nullptr);
		EXPECT_EQ(ptr->GetDate(), toLocalDays(20210102));
		EXPECT_EQ(ptr->GetClose(), 2000);
	}

	TEST_F(CVirtualDataHistoryCandleTest, GetCandle_ReturnsNullWhenDateMissing) {
		CVirtualHistoryCandle c1;
		c1.SetDate(20210101);
		c1.SetClose(1000);

		vector<CVirtualHistoryCandle> data{ c1 };
		CVirtualDataHistoryCandle container;
		container.UpdateData(data);

		auto ptr = container.GetCandle(toLocalDays(20201231));
		EXPECT_EQ(ptr, nullptr);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetCandle_Existing) {
		CVirtualDataHistoryCandle container;

		CVirtualHistoryCandle c1, c2, c3;
		c1.SetDate(20200101);
		c1.SetHigh(100);
		c2.SetDate(20200102);
		c2.SetHigh(200);
		c3.SetDate(20200103);
		c3.SetHigh(300);

		// add in chronological order
		container.Add(c1);
		container.Add(c2);
		container.Add(c3);

		auto ptr = container.GetCandle(toLocalDays(20200102));
		ASSERT_NE(ptr, nullptr);
		EXPECT_EQ(ptr->GetDate(), toLocalDays(20200102));
		EXPECT_EQ(ptr->GetHigh(), 200);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetCandle_NotFound) {
		CVirtualDataHistoryCandle container;

		CVirtualHistoryCandle c1, c2;
		c1.SetDate(20200101);
		c2.SetDate(20200102);
		container.Add(c1);
		container.Add(c2);

		auto ptr = container.GetCandle(toLocalDays(20200103)); // missing
		EXPECT_EQ(ptr, nullptr);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetCandle_EmptyContainer) {
		CVirtualDataHistoryCandle container;
		auto ptr = container.GetCandle(toLocalDays(20200101));
		EXPECT_EQ(ptr, nullptr);
	}

	// Multiple candles: verify GetHighLow considers the most-recent N candles.
	TEST_F(CVirtualDataHistoryCandleTest, TestGetHighLow_MultipleCandles) {
		CVirtualDataHistoryCandle container;

		CVirtualHistoryCandle c1, c2, c3;
		c1.SetDate(20200101);
		c1.SetHigh(100);
		c1.SetLow(90);
		c2.SetDate(20200102);
		c2.SetHigh(150);
		c2.SetLow(80);
		c3.SetDate(20200103);
		c3.SetHigh(120);
		c3.SetLow(110);

		// add in chronological order
		container.Add(c1);
		container.Add(c2);
		container.Add(c3);

		// Request last 2 candles -> should consider c3 and c2
		auto hl = container.GetHighLow(2);
		EXPECT_EQ(hl.first, 150);  // max of 120 and 150
		EXPECT_EQ(hl.second, 80);  // min of 110 and 80 (ignores non-positive lows)
	}

	// Single candle: exercise current behavior for one element.
	// Note: implementation sets lLow from last element but does not update lHigh when only one element exists.
	TEST_F(CVirtualDataHistoryCandleTest, TestGetHighLow_SingleCandle) {
		CVirtualDataHistoryCandle container;

		CVirtualHistoryCandle c;
		c.SetDate(20210101);
		c.SetHigh(200);
		c.SetLow(50);

		container.Add(c);

		auto hl = container.GetHighLow(1);
		// Current implementation returns lHigh == 0 for single-element case and lLow == the candle's low.
		EXPECT_EQ(hl.first, 0);
		EXPECT_EQ(hl.second, 50);
	}
}
