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
		const auto pData = make_shared<CVirtualHistoryCandle>();
		pData->SetStockSymbol("600008.SS");
		pData->SetDate(20202020);
		id.Add(pData);

		const auto pContainer = id.GetContainer();
		EXPECT_EQ((*pContainer)[0]->GetDate(), 20202020);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS1) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSIndex(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSIndex(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRSIndex) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSIndex(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSIndex(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRSIndex1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRSLogarithm) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSLogarithm(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSLogarithm(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRSLogarithm1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS3) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->Set3RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set3RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS3(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS5) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->Set5RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set5RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS5(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS10) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->Set10RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set10RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS10(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS30) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->Set30RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set30RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS30(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS60) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->Set60RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set60RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS60(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestGetRS120) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->Set120RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set120RS(2.2);

		CVirtualDataHistoryCandle HistoryDataContainer;
		HistoryDataContainer.Add(pHistoryData);
		HistoryDataContainer.Add(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS120(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleTest, TestUpdateData1) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandle>();
		pHistoryData->SetStockSymbol("600000.SS");
		pHistoryData->SetDate(20200101);
		pHistoryData->SetHigh(1000);
		pHistoryData->SetLow(200);
		pHistoryData->SetClose(100); // 确保此数据有效
		auto pHistoryData2 = make_shared<CVirtualHistoryCandle>();
		pHistoryData2->SetStockSymbol("600001.SS");
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetHigh(11000);
		pHistoryData2->SetLow(1200);
		pHistoryData2->SetClose(100); // 确保此数据有效

		CVirtualDataHistoryCandle HistoryDataContainer;
		vector<CVirtualHistoryCandlePtr> vHistoryData;

		vHistoryData.push_back(pHistoryData);
		EXPECT_EQ(HistoryDataContainer.Size(), 0);
		HistoryDataContainer.Add(pHistoryData2);
		EXPECT_EQ(HistoryDataContainer.Size(), 1);
		HistoryDataContainer.UpdateData(vHistoryData);
		EXPECT_EQ(HistoryDataContainer.Size(), 1);
		pHistoryData2 = HistoryDataContainer.GetData(0);
		EXPECT_EQ(pHistoryData2->GetDate(), 20200101);
		EXPECT_EQ(pHistoryData2->GetHigh(), 1000);
		EXPECT_EQ(pHistoryData2->GetLow(), 200);
		EXPECT_EQ(pHistoryData2->GetClose(), 100);
		EXPECT_TRUE(HistoryDataContainer.IsDataLoaded());
	}
}
