#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"

#include"ContainerAlpacaStockSymbol.h"
#include"AlpacaStock.h"

#include <string>

#include<sqlpp23/sqlpp23.h>
#include "dataBaseConnector.h"
#include "StockMarketSQLTable.h"

using namespace testing;
using std::make_shared;

namespace FireBirdTest {
	class CContainerAlpacaStockSymbolTest : public Test {
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
			CAlpacaStockPtr pStock = nullptr;
			/*
			for (int i = 0; i < 100; i++) {
				pStock = gl_dataContainerAlpacaStockSymbol.GetItem(i);
				m_containerAlpacaStockSymbol.Add(pStock);
			}*/
		}

		void TearDown() override {
			//EXPECT_EQ(m_containerAlpacaStockSymbol.Size(), 100);
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CContainerAlpacaStockSymbol m_containerAlpacaStockSymbol;
	};

	using namespace StockMarket;

	TEST_F(CContainerAlpacaStockSymbolTest, UpdateProfileDB_InsertsNewStock_WhenDbConfigured) {
		using namespace StockMarket;
		auto db = gl_dbStockMarket.get();

		// Use a unique test symbol to avoid collisions
		const std::string testSymbol = "UT_TEST_SYMBOL_12345";

		// Ensure any prior leftover rows are removed
		{
			const auto& t = AlpacaStockSymbol{};
			db(sqlpp::delete_from(t).where(t.Symbol == testSymbol));
		}

		// Prepare container and a new stock
		CContainerAlpacaStockSymbol container;
		container.Reset();
		container.SetUpdateDB(true); // enable updates (checked by UpdateProfileDB)

		auto pStock = std::make_shared<CAlpacaStock>();
		pStock->SetSymbol(testSymbol);
		string s = "NYSE";
		pStock->SetExchange(s);
		s = "UnitTest Company";
		pStock->SetDisplaySymbol(s);
		pStock->SetNewStock(true);
		pStock->SetUpdateProfileDB(true);

		container.Add(pStock);

		// Call UpdateProfileDB with a non-requested stop token
		std::stop_source ss;
		container.UpdateProfileDB(ss.get_token());

		// Verify insertion happened
		const auto& t = AlpacaStockSymbol{};
		auto result = db(select(all_of(t)).from(t).where(t.Symbol == testSymbol));
		ASSERT_EQ(result.size(), 1u) << "Expected exactly one inserted row for test symbol.";

		// Cleanup: delete test row
		db(sqlpp::delete_from(t).where(t.Symbol == testSymbol));
	}
}
