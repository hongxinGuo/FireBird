#include"pch.h"

#include"TimeConvert.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"ContainerChinaWeekLine.h"

#include"SetCurrentWeekLine.h"

namespace FireBirdTest {
	class CStockDataChinaWeekLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CStockDataChinaWeekLineTest, TestUpdateData1) {
		const auto pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(20200101);
		pWeekLine->SetHigh(1000);
		pWeekLine->SetLow(200);
		auto pWeekLine2 = make_shared<CWeekLine>();
		pWeekLine2->SetStockSymbol(_T("600001.SS"));
		pWeekLine2->SetDate(20200201);
		pWeekLine2->SetHigh(11000);
		pWeekLine2->SetLow(1200);

		CContainerChinaWeekLine dataChinaWeekLine;
		vector<CWeekLinePtr> vWeekLine;

		vWeekLine.push_back(pWeekLine);
		EXPECT_EQ(dataChinaWeekLine.Size(), 0);
		dataChinaWeekLine.StoreData(pWeekLine2);
		EXPECT_EQ(dataChinaWeekLine.Size(), 1);
		EXPECT_FALSE(dataChinaWeekLine.IsDataLoaded());

		dataChinaWeekLine.UpdateData(vWeekLine);

		EXPECT_EQ(dataChinaWeekLine.Size(), 1);
		pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(0));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), 20200101);
		EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
		EXPECT_EQ(pWeekLine2->GetLow(), 200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());
	}

	TEST_F(CStockDataChinaWeekLineTest, TestUpdateData2) {
		const auto pDayLine = make_shared<CDayLine>();
		pDayLine->SetStockSymbol(_T("600000.SS"));
		pDayLine->SetDate(20200101);
		pDayLine->SetHigh(10000);
		pDayLine->SetLow(100);
		pDayLine->SetTransactionNumber(101);
		const auto pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetHigh(1000);
		pWeekLine->SetLow(200);
		pWeekLine->SetTransactionNumber(1);

		CContainerChinaWeekLine dataChinaWeekLine;

		dataChinaWeekLine.StoreData(pWeekLine);
		dataChinaWeekLine.UpdateData(pDayLine);
		const CWeekLinePtr pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(0));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), GetCurrentMonday(20200101));
		EXPECT_EQ(pWeekLine2->GetHigh(), 10000);
		EXPECT_EQ(pWeekLine2->GetLow(), 100);
		EXPECT_EQ(pWeekLine2->GetTransactionNumber(), 102);
	}

	TEST_F(CStockDataChinaWeekLineTest, TestStoreData1) {
		const auto pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(20200101);
		pWeekLine->SetHigh(1000);
		pWeekLine->SetLow(200);
		auto pWeekLine2 = make_shared<CWeekLine>();
		pWeekLine2->SetStockSymbol(_T("600001.SS"));
		pWeekLine2->SetDate(20200201);
		pWeekLine2->SetHigh(11000);
		pWeekLine2->SetLow(1200);

		CContainerChinaWeekLine dataChinaWeekLine;
		vector<CWeekLinePtr> vWeekLine;

		vWeekLine.push_back(pWeekLine);
		EXPECT_FALSE(dataChinaWeekLine.IsDataLoaded());
		EXPECT_EQ(dataChinaWeekLine.Size(), 0);
		dataChinaWeekLine.StoreData(pWeekLine2);
		EXPECT_EQ(dataChinaWeekLine.Size(), 1);

		dataChinaWeekLine.StoreVectorData(vWeekLine);

		EXPECT_EQ(dataChinaWeekLine.Size(), 2);
		pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(0));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), 20200201);
		EXPECT_EQ(pWeekLine2->GetHigh(), 11000);
		EXPECT_EQ(pWeekLine2->GetLow(), 1200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());

		pWeekLine2 = static_pointer_cast<CWeekLine>(dataChinaWeekLine.GetData(1));
		EXPECT_EQ(pWeekLine2->GetMarketDate(), 20200101);
		EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
		EXPECT_EQ(pWeekLine2->GetLow(), 200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());
	}

	TEST_F(CStockDataChinaWeekLineTest, TestSaveLoadCurrentWeekLine) {
		CSetCurrentWeekLine setCurrentWeekLine, setCurrentWeekLine2;
		auto pWeekLine = make_shared<CWeekLine>();
		CContainerChinaWeekLine dataChinaWeekLine, weekLineContainer2;

		pWeekLine->SetStockSymbol(_T("600000.SS"));
		pWeekLine->SetDate(GetCurrentMonday(20200101)); // 此日期为星期三，20191230为星期一。
		dataChinaWeekLine.StoreData(pWeekLine);

		gl_pChinaMarket->DeleteCurrentWeekWeekLine();
		dataChinaWeekLine.SaveCurrentWeekLine();

		weekLineContainer2.LoadCurrentWeekLine();
		pWeekLine = static_pointer_cast<CWeekLine>(weekLineContainer2.GetData(0));
		EXPECT_STREQ(pWeekLine->GetStockSymbol(), _T("600000.SS"));
		EXPECT_EQ(pWeekLine->GetMarketDate(), 20191230) << "20200101之前的星期一";

		// 恢复原态
		gl_pChinaMarket->DeleteCurrentWeekWeekLine();
	}

	TEST_F(CStockDataChinaWeekLineTest, TestSaveDB) {
		vector<CWeekLinePtr> vWeekLine;
		CContainerChinaWeekLine dataChinaWeekLine;

		const CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
		pWeekLine->SetDate(19901224); // 测试数据库中000003.SZ最早的日期为19901231，故此数据位于最前面
		pWeekLine->SetStockSymbol(_T("000003.SZ"));
		pWeekLine->SetClose(100);
		vWeekLine.push_back(pWeekLine);
		dataChinaWeekLine.UpdateData(vWeekLine);

		dataChinaWeekLine.SaveDB(_T("000003.SZ"));

		dataChinaWeekLine.LoadDB(_T("000003.SZ"));
		EXPECT_EQ(dataChinaWeekLine.GetData(0)->GetMarketDate(), 19901224) << "新存储数据的日期";

		// 恢复原状
		CSetWeekLineBasicInfo setChinaStockWeekLineBasic;
		setChinaStockWeekLineBasic.m_strFilter = _T("[Symbol] = '000003.SZ'");
		setChinaStockWeekLineBasic.m_strSort = _T("[Date]");
		setChinaStockWeekLineBasic.Open();
		setChinaStockWeekLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setChinaStockWeekLineBasic.m_Date, 19901224) << "新存储数据的日期";
		setChinaStockWeekLineBasic.Delete();
		setChinaStockWeekLineBasic.m_pDatabase->CommitTrans();
		setChinaStockWeekLineBasic.Close();
	}
}
