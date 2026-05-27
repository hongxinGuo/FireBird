#include"pch.h"

#include"GeneralCheck.h"

#include"DayLine.h"

#include"ContainerForexDayLine.h"
#include "dataBaseConnector.h"

using namespace testing;

namespace FireBirdTest {
	class CDataForexDayLineTest : public ::testing::Test {
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
		CContainerForexDayLine m_dataForexDayLine;
	};

	TEST_F(CDataForexDayLineTest, TestSaveDB) {
		CDayLinesPtr pvDayLine = make_shared<vector<CDayLine>>();

		CDayLine dayLine;
		dayLine.SetDate(20200411); // 此日为星期六，新数据
		dayLine.SetStockSymbol("OANDA:AUD_SGD");
		dayLine.SetExchange("Test");
		dayLine.SetClose(100);
		pvDayLine->push_back(dayLine);
		m_dataForexDayLine.UpdateData(pvDayLine);

		m_dataForexDayLine.SaveDB("OANDA:AUD_SGD");

		m_dataForexDayLine.LoadDB("OANDA:AUD_SGD");
		EXPECT_EQ(m_dataForexDayLine.GetData(5)->GetDate(), 20200411) << "新存储数据位于第六位";

		// 恢复原状
		using namespace StockMarket;
		const auto& t = FinnhubForexDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		auto result = db(select(all_of(t)).from(t).where(t.Date == 20200411));
		size_t rows = result.size();
		EXPECT_EQ(rows, 1) << "刚存储的数据";

		db(sqlpp::remove_from(t).where(t.Date == 20200411));
		tx.commit();
	}
}
