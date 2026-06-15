#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"
#include"ContainerFinnhubStockDayLine.h"
#include "dataBaseConnector.h"

using namespace testing;

namespace FireBirdTest {
	class CDataFinnhubStockDayLineTest : public ::testing::Test {
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
		CContainerFinnhubStockDayLine m_dataFinnhubStockDayLine;
	};

	TEST_F(CDataFinnhubStockDayLineTest, TestSaveDB) {
		CDayLinesPtr vDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20220101); // 测试库中的数据最新日期为20210330，此日期位于其后
		dayLine.SetStockSymbol("A");
		dayLine.SetExchange("Test"); // 日线表中没有这个字段，用于删除遗留的测试数据。
		dayLine.SetClose(100);
		vDayLine->push_back(dayLine);
		m_dataFinnhubStockDayLine.UpdateData(vDayLine);

		m_dataFinnhubStockDayLine.SaveDB("A");

		m_dataFinnhubStockDayLine.LoadDB("A");
		EXPECT_EQ(m_dataFinnhubStockDayLine.GetData(m_dataFinnhubStockDayLine.Size() - 1)->GetDate(),
		          toLocalDays(20220101)) << "新存储数据位于最后";

		// 恢复原状
		using namespace StockMarket;
		const auto& t = FinnhubStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.Date == 20220101));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1);

		db(remove_from(t).where(t.Date == 20220101));
		tx.commit();
	}
}
