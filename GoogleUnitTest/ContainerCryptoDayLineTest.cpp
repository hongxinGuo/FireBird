#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"ContainerCryptoDayLine.h"
#include "dataBaseConnector.h"

using namespace testing;

namespace FireBirdTest {
	class CDataCryptoDayLineTest : public ::testing::Test {
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
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerCryptoDayLine m_dataCryptoDayLine;
	};

	TEST_F(CDataCryptoDayLineTest, TestSaveDB) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200101); // 测试数据库中最早的日期为20200817，故此数据位于最前面
		dayLine.SetExchange("Test"); // 用于删除测试数据
		dayLine.SetStockSymbol("BINANCE:USDTUAH");
		dayLine.SetClose(100);
		pvDayLine->push_back(dayLine);
		m_dataCryptoDayLine.UpdateData(pvDayLine);

		m_dataCryptoDayLine.SaveDB("BINANCE:USDTUAH");

		m_dataCryptoDayLine.LoadDB("BINANCE:USDTUAH");
		EXPECT_EQ(m_dataCryptoDayLine.GetData(0)->GetDate(), 20200101) << "新存储数据的日期";

		// 恢复原状
		using namespace StockMarket;
		const auto& t = FinnhubCryptoDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.Symbol == "BINANCE:USDTUAH" && t.Date == 20200101).order_by(t.Date.asc()));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1) << "新存储数据的行数";
		auto& row = result.front();
		EXPECT_EQ(row.Date.value(), 20200101) << "新存储数据的日期";

		db(sqlpp::remove_from(t).where(t.Symbol == "BINANCE:USDTUAH"));
		tx.commit();
	}
}
