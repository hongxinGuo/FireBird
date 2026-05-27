#include"pch.h"

#include"TimeConvert.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"ContainerChinaWeekLine.h"
#include "dataBaseConnector.h"

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
		weekLine.SetStockSymbol("000001.SZ");
		weekLine.SetClose(100);
		vWeekLine.push_back(weekLine);
		dataChinaWeekLine.UpdateData(vWeekLine);

		dataChinaWeekLine.SaveDB("000001.SZ");

		dataChinaWeekLine.LoadDB("000001.SZ");
		EXPECT_EQ(dataChinaWeekLine.GetData(0)->GetDate(), 19901224) << "新存储数据的日期";

		// 恢复原状
		using namespace StockMarket;
		const auto& t = ChinaStockWeekline{};
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		auto result = db(select(all_of(t)).from(t).order_by(t.Date.asc()).where(t.Symbol == "000001.SZ"));
		size_t rows = result.size();
		EXPECT_GT(rows, 1);
		auto& row = result.front();
		EXPECT_EQ(row.Date.value(), 19901224) << "新存储数据的日期";

		db(sqlpp::remove_from(t).where(t.Symbol == "000001.SZ" && t.Date == 19901224));
		tx.commit();
	}

	TEST_F(CStockDataChinaWeekLineTest, SaveDB_InsertsMissingRows) {
		using namespace StockMarket;
		const string testSymbol = "TEST";

		// Ensure no existing rows for the test symbol
		{
			const auto& t = ChinaStockWeekline{};
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);
			db(remove_from(t).where(t.Symbol == testSymbol));
			tx.commit();
		}

		// Build container with one weekline entry
		CContainerChinaWeekLine container;
		const int ratio = container.GetRatio();
		CWeekLine wk;
		wk.SetStockSymbol(testSymbol);
		wk.SetDate(20250101);
		wk.SetExchange("Test");
		// set prices scaled by ratio so DB stores normalized values (value/ratio)
		wk.SetLastClose(500LL * ratio);
		wk.SetOpen(600LL * ratio);
		wk.SetHigh(700LL * ratio);
		wk.SetLow(400LL * ratio);
		wk.SetClose(650LL * ratio);
		wk.SetVolume(123456LL);
		wk.SetAmount(789012LL);

		vector<CWeekLine> v{ wk };
		container.StoreVectorData(v);

		// Call SaveDB and expect it to insert rows
		EXPECT_TRUE(container.SaveDB(testSymbol));

		// Verify DB now contains exactly one row for the symbol with expected (normalized) values
		{
			const auto& t = ChinaStockWeekline{};
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);
			auto result = db(select(all_of(t)).from(t).where(t.Symbol == testSymbol));
			tx.commit();

			ASSERT_EQ(result.size(), 1u);
			const auto& row = result.front();
			EXPECT_EQ(row.Date.value(), 20250101);
			EXPECT_EQ(row.Exchange, "Test");
			// In SaveDB we insert price = candle.GetOpen() / m_ratio etc.
			EXPECT_DOUBLE_EQ(row.LastClose.value(), static_cast<double>(wk.GetLastClose()) / ratio);
			EXPECT_DOUBLE_EQ(row.Open.value(), static_cast<double>(wk.GetOpen()) / ratio);
			EXPECT_DOUBLE_EQ(row.High.value(), static_cast<double>(wk.GetHigh()) / ratio);
			EXPECT_DOUBLE_EQ(row.Low.value(), static_cast<double>(wk.GetLow()) / ratio);
			EXPECT_DOUBLE_EQ(row.Close.value(), static_cast<double>(wk.GetClose()) / ratio);
			EXPECT_EQ(row.Volume.value(), wk.GetVolume());
			EXPECT_EQ(row.Amount.value(), wk.GetAmount());
		}

		// Cleanup
		{
			const auto& t = ChinaStockWeekline{};
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);
			db(remove_from(t).where(t.Symbol == testSymbol));
			tx.commit();
		}
	}
}
