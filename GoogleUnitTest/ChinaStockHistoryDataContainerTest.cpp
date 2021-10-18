#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"VirtualHistoryCandleExtend.h"
#include"VirtualHistoryCandleExtendContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CHistoryDataContainerTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
		}
	};

	TEST_F(CHistoryDataContainerTest, TestSaveData) {
		CVirtualHistoryCandleExtendContainer id;
		EXPECT_FALSE(id.SaveData(_T("600000.SS")));
	}

	TEST_F(CHistoryDataContainerTest, TestLOadData) {
		CVirtualHistoryCandleExtendContainer id;
		EXPECT_FALSE(id.LoadData(_T("600000.SS")));
	}

	TEST_F(CHistoryDataContainerTest, TestGetContainer) {
		CVirtualHistoryCandleExtendContainer id;
		CVirtualHistoryCandleExtendPtr pData = make_shared<CVirtualHistoryCandleExtend>();
		pData->SetStockSymbol(_T("600008.SS"));
		pData->SetDate(20202020);
		id.StoreData(pData);

		auto pContainer = id.GetContainer();
		EXPECT_EQ((*pContainer)[0]->GetMarketDate(), 20202020);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRS1) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSIndex(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSIndex(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRS1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRSIndex) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSIndex(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSIndex(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRSIndex1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRSLogarithm) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetRSLogarithm(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetRSLogarithm(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRSLogarithm1(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRS3) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set3RS(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set3RS(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRS3(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRS5) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set5RS(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set5RS(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRS5(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRS10) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set10RS(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set10RS(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRS10(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRS30) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set30RS(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set30RS(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRS30(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRS60) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set60RS(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set60RS(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRS60(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestGetRS120) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->Set120RS(1.1);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->Set120RS(2.2);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		HistoryDataContainer.StoreData(pHistoryData);
		HistoryDataContainer.StoreData(pHistoryData2);

		vector<double> vRS;
		vRS.resize(HistoryDataContainer.GetDataSize());

		HistoryDataContainer.GetRS120(vRS);
		EXPECT_DOUBLE_EQ(vRS[0], 1.1);
		EXPECT_DOUBLE_EQ(vRS[1], 2.2);
	}

	TEST_F(CHistoryDataContainerTest, TestUpdateData1) {
		CVirtualHistoryCandleExtendPtr pHistoryData = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData->SetStockSymbol(_T("600000.SS"));
		pHistoryData->SetDate(20200101);
		pHistoryData->SetHigh(1000);
		pHistoryData->SetLow(200);
		CVirtualHistoryCandleExtendPtr pHistoryData2 = make_shared<CVirtualHistoryCandleExtend>();
		pHistoryData2->SetStockSymbol(_T("600001.SS"));
		pHistoryData2->SetDate(20200201);
		pHistoryData2->SetHigh(11000);
		pHistoryData2->SetLow(1200);

		CVirtualHistoryCandleExtendContainer HistoryDataContainer;
		vector<CVirtualHistoryCandleExtendPtr> vHistoryData;

		vHistoryData.push_back(pHistoryData);
		EXPECT_EQ(HistoryDataContainer.GetDataSize(), 0);
		HistoryDataContainer.StoreData(pHistoryData2);
		EXPECT_EQ(HistoryDataContainer.GetDataSize(), 1);
		HistoryDataContainer.UpdateData(vHistoryData);
		EXPECT_EQ(HistoryDataContainer.GetDataSize(), 1);
		pHistoryData2 = HistoryDataContainer.GetData(0);
		EXPECT_EQ(pHistoryData2->GetMarketDate(), 20200101);
		EXPECT_EQ(pHistoryData2->GetHigh(), 1000);
		EXPECT_EQ(pHistoryData2->GetLow(), 200);
		EXPECT_TRUE(HistoryDataContainer.IsDataLoaded());
	}
}