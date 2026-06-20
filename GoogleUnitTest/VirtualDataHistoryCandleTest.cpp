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
}
