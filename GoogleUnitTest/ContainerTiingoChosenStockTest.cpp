#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerTiingoChosenStock.h"
#include <sqlpp11/remove.h>

using namespace testing;

namespace FireBirdTest {
	class CContainerTiingoChosenStockTest : public Test {
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
		CContainerTiingoChosenStock m_dataTiingoChosenStock;
	};

	TEST_F(CContainerTiingoChosenStockTest, TestInitialize) {
		EXPECT_EQ(m_dataTiingoChosenStock.Size(), 0);
	}

	TEST_F(CContainerTiingoChosenStockTest, TestLoad) {
		EXPECT_EQ(m_dataTiingoChosenStock.Size(), 0) << "初始未装载IEX代码";

		m_dataTiingoChosenStock.LoadDB();
		EXPECT_EQ(m_dataTiingoChosenStock.Size(), 4) << "默认状态下装载4个代码";

		const CTiingoStockPtr pIEX = dynamic_pointer_cast<CTiingoStock>(m_dataTiingoChosenStock.Get(2));
		EXPECT_EQ(pIEX->GetSymbol(), "AAL") << "装载时没有排序，使用的是原始位置";
	}

	TEST_F(CContainerTiingoChosenStockTest, LoadDB_LoadsValidAndRemovesInvalid) {
		// Need at least one Tiingo stock in global container
		ASSERT_GE(gl_dataContainerTiingoStock.Size(), 1u);

		// Pick an existing Tiingo stock symbol
		auto pValid = gl_dataContainerTiingoStock.GetStock("RIG");
		ASSERT_NE(pValid, nullptr);
		const std::string validSymbol = pValid->GetSymbol();

		const std::string invalidSymbol = "UNIT_TEST_INVALID_SYM_12345";

		using namespace StockMarket;
		const auto& t = WorldChoiceStock{};

		// Insert one valid and one invalid row
		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			db(sqlpp::insert_into(t).set(t.Symbol = validSymbol));
			db(sqlpp::insert_into(t).set(t.Symbol = invalidSymbol));
			tx.commit();
		}

		// Call LoadDB()
		CContainerTiingoChosenStock chosen;
		ASSERT_TRUE(chosen.LoadDB());

		// After LoadDB, the container should contain only the valid symbol
		EXPECT_EQ(chosen.Size(), 5u);
		EXPECT_TRUE(chosen.IsSymbol(validSymbol));
		EXPECT_FALSE(chosen.IsSymbol(invalidSymbol));

		// Verify DB only contains the valid symbol (invalid should be removed)
		{
			auto db = gl_dbStockMarket.get();
			auto result = db(select(all_of(t)).from(t).unconditionally());
			std::vector<std::string> dbSymbols;
			for (const auto& row : result) {
				if (row.Symbol != "") dbSymbols.push_back(row.Symbol);
			}
			std::vector<std::string> expected = { "A", "AA", "AAL", "AAPL", validSymbol };
			EXPECT_THAT(dbSymbols, UnorderedElementsAreArray(expected));
		}

		// Cleanup
		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			db(sqlpp::remove_from(t).where(t.Symbol == "RIG"));
			tx.commit();
		}
	}

	TEST_F(CContainerTiingoChosenStockTest, UpdateDB_InsertsMissingSymbols) {
		// Ensure there are at least two Tiingo stocks available in global container
		ASSERT_GE(gl_dataContainerTiingoStock.Size(), 2u);

		// Pick two existing Tiingo stocks
		auto pStock1 = gl_dataContainerTiingoStock.GetStock("RIG");
		auto pStock2 = gl_dataContainerTiingoStock.GetStock("BAX");
		ASSERT_NE(pStock1, nullptr);
		ASSERT_NE(pStock2, nullptr);

		using namespace StockMarket;
		const auto& t = WorldChoiceStock{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			auto result = db(select(all_of(t)).from(t).unconditionally());
			size_t rows = result.size();
			EXPECT_EQ(rows, 4) << "初始时有四个代码"; // A, AA, AAL, AAPL
			tx.commit();
		}

		// Create chosen container and add selected stocks
		CContainerTiingoChosenStock chosen;
		chosen.Add(pStock1);
		chosen.Add(pStock2);

		// Call method under test
		chosen.UpdateDB();

		// Read back rows from DB and verify symbols were inserted
		{
			auto db = gl_dbStockMarket.get();
			auto result = db(select(all_of(t)).from(t).unconditionally());
			std::vector<std::string> dbSymbols;
			for (const auto& row : result) {
				if (row.Symbol != "") dbSymbols.push_back(static_cast<std::string>(row.Symbol));
			}
			std::vector<std::string> expected = { "A", "AA", "AAL", "AAPL", pStock1->GetSymbol(), pStock2->GetSymbol() };
			EXPECT_THAT(dbSymbols, UnorderedElementsAreArray(expected));
		}

		// Cleanup inserted rows to avoid side effects on other tests
		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			db(sqlpp::remove_from(t).where((t.Symbol == "RIG") || (t.Symbol == "BAX")));
			tx.commit();
		}
	}
}
