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

	TEST_F(CVirtualDataHistoryCandleTest, TestSaveDB) {
		CVirtualDataHistoryCandle id;
		EXPECT_FALSE(id.SaveDB("600000.SS"));
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestLoadDB) {
		CVirtualDataHistoryCandle id;
		EXPECT_FALSE(id.LoadDB("600000.SS"));
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetContainer) {
		CVirtualDataHistoryCandle id;
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600008.SS");
		data.SetDate(20202020);
		id.Add(data);

		const auto pContainer = id.GetContainer();
		EXPECT_EQ((*pContainer)[0].GetDate(), 20202020);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS1) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.SetRSIndex(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.SetRSIndex(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRSIndex) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.SetRSIndex(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.SetRSIndex(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRSIndex1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRSLogarithm) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.SetRSLogarithm(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.SetRSLogarithm(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRSLogarithm1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS3) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.Set3RS(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.Set3RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS3(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS5) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.Set5RS(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.Set5RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS5(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS10) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.Set10RS(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.Set10RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS10(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS30) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.Set30RS(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.Set30RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS30(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS60) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.Set60RS(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.Set60RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS60(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS120) {
		CVirtualHistoryCandle data;
		data.SetStockSymbol("600000.SS");
		data.SetDate(20200101);
		data.Set120RS(1.1);
		CVirtualHistoryCandle data2;
		data2.SetStockSymbol("600001.SS");
		data2.SetDate(20200201);
		data2.Set120RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(data);
		HistoryDataContainer.Add(data2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS120(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
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
		EXPECT_EQ(pHistoryData2->GetDate(), 20200101);
		EXPECT_EQ(pHistoryData2->GetHigh(), 1000);
		EXPECT_EQ(pHistoryData2->GetLow(), 200);
		EXPECT_EQ(pHistoryData2->GetClose(), 100);
		EXPECT_TRUE(HistoryDataContainer.IsDataLoaded());
	}
}
