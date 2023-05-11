#include"pch.h"

#include"GeneralCheck.h"

#include"VirtualDataHistoryCandleExtend.h"

#include<memory>
using std::make_shared;

namespace FireBirdTest {
	class CVirtualDataHistoryCandleExtendTest : public ::testing::Test {
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

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestSaveDB) {
		CVirtualDataHistoryCandleExtend id;
		EXPECT_FALSE(id.SaveDB(_T("600000.SS")));
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestLoadDB) {
		CVirtualDataHistoryCandleExtend id;
		EXPECT_FALSE(id.LoadDB(_T("600000.SS")));
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetContainer) {
		CVirtualDataHistoryCandleExtend id;
		const auto pData = make_shared<CVirtualHistoryCandleExtend>();
		pData->SetStockSymbol(_T("600008.SS"));
		pData->SetDate(20202020);
		id.StoreData(pData);

		const auto pContainer = id.GetContainer();
		EXPECT_EQ((*pContainer)[0]->GetMarketDate(), 20202020);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRS1) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSIndex(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSIndex(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRSIndex) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSIndex(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSIndex(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRSIndex1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRSLogarithm) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSLogarithm(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSLogarithm(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRSLogarithm1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRS3) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set3RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set3RS(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS3(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRS5) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set5RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set5RS(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS5(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRS10) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set10RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set10RS(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS10(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRS30) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set30RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set30RS(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS30(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRS60) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set60RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set60RS(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS60(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestGetRS120) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set120RS(1.1);
		const auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set120RS(2.2);

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.Size());

		HistoryDataContainer.GetRS120(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CVirtualDataHistoryCandleExtendTest, TestUpdateData1) {
		const auto pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetHigh(1000);
		pHistoryData->SetLow(200);
		pHistoryData->SetClose(100); // 确保此数据有效
		auto pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetHigh(11000);
		pHistoryData2->SetLow(1200);
		pHistoryData2->SetClose(100); // 确保此数据有效

		CVirtualDataHistoryCandleExtend HistoryDataContainer;
		vector<CVirtualHistoryCandleExtendPtr> vHistoryData;

		vHistoryData.push_back(pHistoryData);
		EXPECT_EQ(HistoryDataContainer.Size(), 0);
		HistoryDataContainer.StoreData(pHistoryData2);
		EXPECT_EQ(HistoryDataContainer.Size(), 1);
		HistoryDataContainer.UpdateData(vHistoryData);
		EXPECT_EQ(HistoryDataContainer.Size(), 1);
		pHistoryData2 = HistoryDataContainer.GetData(0);
		EXPECT_EQ(pHistoryData2->GetMarketDate(), 20200101);
		EXPECT_EQ(pHistoryData2->GetHigh(), 1000);
		EXPECT_EQ(pHistoryData2->GetLow(), 200);
		EXPECT_EQ(pHistoryData2->GetClose(), 100);
		EXPECT_TRUE(HistoryDataContainer.IsDataLoaded());
	}
}
