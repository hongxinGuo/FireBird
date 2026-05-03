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

	TEST_F(CStockDataChinaWeekLineTest, TestUpdateData1) {
		CWeekLine weekLine, weekLine2;
		weekLine.SetStockSymbol("600000.SS");
		weekLine.SetDate(20200101);
		weekLine.SetHigh(1000);
		weekLine.SetLow(200);
		weekLine2.SetStockSymbol("600001.SS");
		weekLine2.SetDate(20200201);
		weekLine2.SetHigh(11000);
		weekLine2.SetLow(1200);

		CContainerChinaWeekLine dataChinaWeekLine;
		vector<CWeekLine> vWeekLine;

		vWeekLine.push_back(weekLine);
		EXPECT_EQ(dataChinaWeekLine.Size(), 0);
		dataChinaWeekLine.Add(static_cast<CVirtualHistoryCandle>(weekLine2));
		EXPECT_EQ(dataChinaWeekLine.Size(), 1);
		EXPECT_FALSE(dataChinaWeekLine.IsDataLoaded());

		dataChinaWeekLine.UpdateData(vWeekLine);

		EXPECT_EQ(dataChinaWeekLine.Size(), 1);
		const auto pWeekLine2 = dataChinaWeekLine.GetData(0);
		EXPECT_EQ(pWeekLine2->GetDate(), 20200101);
		EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
		EXPECT_EQ(pWeekLine2->GetLow(), 200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());
	}

	TEST_F(CStockDataChinaWeekLineTest, TestUpdateData2) {
		CDayLine dayLine;
		dayLine.SetStockSymbol("600000.SS");
		dayLine.SetDate(20200101);
		dayLine.SetHigh(10000);
		dayLine.SetLow(100);
		CWeekLine weekLine;
		weekLine.SetStockSymbol("600000.SS");
		weekLine.SetHigh(1000);
		weekLine.SetLow(200);

		CContainerChinaWeekLine dataChinaWeekLine;

		dataChinaWeekLine.Add(static_cast<CVirtualHistoryCandle>(weekLine));
		dataChinaWeekLine.UpdateData(&dayLine);
		const CWeekLine* pWeekLine2 = dataChinaWeekLine.GetData(0);
		EXPECT_EQ(pWeekLine2->GetDate(), GetCurrentMonday(20200101));
		EXPECT_EQ(pWeekLine2->GetHigh(), 10000);
		EXPECT_EQ(pWeekLine2->GetLow(), 100);
	}

	TEST_F(CStockDataChinaWeekLineTest, TestStoreData1) {
		CWeekLine weekLine;
		weekLine.SetStockSymbol("600000.SS");
		weekLine.SetDate(20200101);
		weekLine.SetHigh(1000);
		weekLine.SetLow(200);
		CWeekLine weekLine2;
		weekLine2.SetStockSymbol("600001.SS");
		weekLine2.SetDate(20200201);
		weekLine2.SetHigh(11000);
		weekLine2.SetLow(1200);
		CContainerChinaWeekLine dataChinaWeekLine;
		vector<CWeekLine> vWeekLine;

		vWeekLine.push_back(weekLine);
		EXPECT_FALSE(dataChinaWeekLine.IsDataLoaded());
		EXPECT_EQ(dataChinaWeekLine.Size(), 0);
		dataChinaWeekLine.Add(weekLine2);
		EXPECT_EQ(dataChinaWeekLine.Size(), 1);

		dataChinaWeekLine.StoreVectorData(vWeekLine);

		EXPECT_EQ(dataChinaWeekLine.Size(), 2);
		auto pWeekLine2 = dataChinaWeekLine.GetData(0);
		EXPECT_EQ(pWeekLine2->GetDate(), 20200201);
		EXPECT_EQ(pWeekLine2->GetHigh(), 11000);
		EXPECT_EQ(pWeekLine2->GetLow(), 1200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());

		pWeekLine2 = dataChinaWeekLine.GetData(1);
		EXPECT_EQ(pWeekLine2->GetDate(), 20200101);
		EXPECT_EQ(pWeekLine2->GetHigh(), 1000);
		EXPECT_EQ(pWeekLine2->GetLow(), 200);
		EXPECT_TRUE(dataChinaWeekLine.IsDataLoaded());
	}

	TEST_F(CStockDataChinaWeekLineTest, TestSaveLoadCurrentWeekLine) {
		CSetCurrentWeekLine setCurrentWeekLine, setCurrentWeekLine2;
		CWeekLine weekLine;
		CContainerChinaWeekLine dataChinaWeekLine, weekLineContainer2;

		weekLine.SetStockSymbol("600000.SS");
		weekLine.SetDate(GetCurrentMonday(20200101)); // 此日期为星期三，20191230为星期一。
		dataChinaWeekLine.Add(weekLine);
		gl_pChinaMarket->DeleteCurrentWeekWeekLine();
		dataChinaWeekLine.SaveCurrentWeekLine();

		weekLineContainer2.LoadCurrentWeekLine();
		auto pWeekLine = weekLineContainer2.GetData(0);
		EXPECT_EQ(pWeekLine->GetStockSymbol(), "600000.SS");
		EXPECT_EQ(pWeekLine->GetDate(), 20191230) << "20200101之前的星期一";

		// 恢复原态
		gl_pChinaMarket->DeleteCurrentWeekWeekLine();
	}

	TEST_F(CStockDataChinaWeekLineTest, TestSaveDB) {
		vector<CWeekLine> vWeekLine;
		CContainerChinaWeekLine dataChinaWeekLine;

		CWeekLine weekLine;
		weekLine.SetDate(19901224); // 测试数据库中000003.SZ最早的日期为19901231，故此数据位于最前面
		weekLine.SetStockSymbol("000003.SZ");
		weekLine.SetClose(100);
		vWeekLine.push_back(weekLine);
		dataChinaWeekLine.UpdateData(vWeekLine);

		dataChinaWeekLine.SaveDB("000003.SZ");

		dataChinaWeekLine.LoadDB("000003.SZ");
		EXPECT_EQ(dataChinaWeekLine.GetData(0)->GetDate(), 19901224) << "新存储数据的日期";

		// 恢复原状
		CSetWeekLineInfo setChinaStockWeekLineBasic;
		setChinaStockWeekLineBasic.m_strFilter = "[Symbol] = '000003.SZ'";
		setChinaStockWeekLineBasic.m_strSort = "[Date]";
		setChinaStockWeekLineBasic.Open();
		setChinaStockWeekLineBasic.m_pDatabase->BeginTrans();
		EXPECT_EQ(setChinaStockWeekLineBasic.m_Date, 19901224) << "新存储数据的日期";
		setChinaStockWeekLineBasic.Delete();
		setChinaStockWeekLineBasic.m_pDatabase->CommitTrans();
		setChinaStockWeekLineBasic.Close();
	}
}
