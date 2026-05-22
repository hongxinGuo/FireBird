#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerFinnhubStock.h"
#include "dataBaseConnector.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerFinnhubStockTest : public Test {
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
	};

	TEST_F(CContainerFinnhubStockTest, TestResetBasicFinancial) {
		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
			EXPECT_TRUE(pStock->IsUpdateBasicFinancial());
			EXPECT_EQ(pStock->GetBasicFinancialUpdateDate(), 19800101);
			EXPECT_FALSE(pStock->IsUpdateProfileDB());

			pStock->SetUpdateBasicFinancial(false);
			pStock->SetUpdateProfileDB(false);
			pStock->SetBasicFinancialUpdateDate(20000101);
		}

		gl_dataContainerFinnhubStock.ResetBasicFinancial();

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
			EXPECT_TRUE(pStock->IsUpdateBasicFinancial());
			EXPECT_EQ(pStock->GetBasicFinancialUpdateDate(), 19800101);
			EXPECT_TRUE(pStock->IsUpdateProfileDB());

			// 恢复原状
			//pStock->SetUpdateBasicFinancial(false);
			pStock->SetUpdateProfileDB(false);
			//pStock->SetBasicFinancialUpdateDate(20000101);
		}
	}

	TEST_F(CContainerFinnhubStockTest, TestClearUpdateBasicFinancialFlag) {
		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
			EXPECT_FALSE(pStock->IsUpdateBasicFinancialDB());
			pStock->SetUpdateBasicFinancialDB(true);
		}
		vector<CFinnhubStockPtr> vStock;
		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
			vStock.push_back(pStock);
		}

		gl_dataContainerFinnhubStock.ClearUpdateBasicFinancialFlag(vStock);

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
			EXPECT_FALSE(pStock->IsUpdateBasicFinancialDB());
		}

		// 恢复原状
	}

	TEST_F(CContainerFinnhubStockTest, TestValidateStockSymbol1) {
		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("AAPL");
		pStock->SetExchangeCode("US");

		EXPECT_TRUE(gl_dataContainerFinnhubStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerFinnhubStockTest, TestValidateStockSymbol2) {
		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("600601.SS");
		pStock->SetExchangeCode("SS");

		EXPECT_TRUE(gl_dataContainerFinnhubStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerFinnhubStockTest, TestValidateStockSymbol3) {
		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol("600601SS");
		pStock->SetExchangeCode("SS");

		EXPECT_FALSE(gl_dataContainerFinnhubStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerFinnhubStockTest, TestIsNeedSaveDayLine) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateDayLineDB());
		gl_dataContainerFinnhubStock.Get(0)->SetUpdateDayLineDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateDayLineDB());

		// 恢复原状
		gl_dataContainerFinnhubStock.Get(0)->SetUpdateDayLineDB(false);
	}

	TEST_F(CContainerFinnhubStockTest, TestIsUpdateInsiderTransactionDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB());
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateInsiderTransactionDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB());

		// 恢复原状
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateInsiderTransactionDB(false);
	}

	TEST_F(CContainerFinnhubStockTest, TestIsSaveInsiderSentiment) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB());
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateInsiderSentimentDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB());

		// 恢复原状
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateInsiderSentimentDB(false);
	}

	TEST_F(CContainerFinnhubStockTest, TestIsUpdateBasicFinancialDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB());
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateBasicFinancialDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB());

		// 恢复原状
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateBasicFinancialDB(false);
	}

	TEST_F(CContainerFinnhubStockTest, TestIsUpdateCompanyNewsDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB());
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateCompanyNewsDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB());

		// 恢复原状
		gl_dataContainerFinnhubStock.GetItem(0)->SetUpdateCompanyNewsDB(false);
	}

	TEST_F(CContainerFinnhubStockTest, TestLoadProfileDB_LoadsSymbols) {
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847) << "测试数据库中的代码数量为4847";
		gl_dataContainerFinnhubStock.Reset();
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 0) << "重置后代码数量应为0";

		// Load from test database
		EXPECT_TRUE(gl_dataContainerFinnhubStock.LoadProfileDB());

		// Basic expectations
		EXPECT_EQ(gl_dataContainerFinnhubStock.Size(), 4847) << "测试数据库中的代码数量为4847";
		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol("AAPL")) << "Test DB must contain AAPL";

		// Verify a loaded item's symbol
		auto p = gl_dataContainerFinnhubStock.GetItem("AAPL");
		ASSERT_NE(p, nullptr);
		EXPECT_EQ(p->GetSymbol(), "AAPL");
	}

	TEST_F(CContainerFinnhubStockTest, TestDeleteDuplicatedSymbolDB) {
		using namespace StockMarket;
		const auto& t = FinnhubStockProfile{};
		// Ensure no leftover test symbols
		auto db = GetStockMarketDB();
		db(remove_from(t).where(t.Symbol == std::string("DUPLICATE")));

		// Insert duplicate rows for the same Symbol
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.Name = std::string("TEST_DUP1")));
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.Name = std::string("TEST_DUP2")));
		db(insert_into(t).set(t.Symbol = std::string("DUPLICATE"), t.Name = std::string("TEST_DUP3")));

		// Ensure inserts are committed and visible to other connections
		db.execute("COMMIT");

		// Verify duplicates were inserted
		db = GetStockMarketDB(); // 执行完插入后，重新获取数据库连接，以确保看到最新的数据
		auto resBefore = db(select(all_of(t)).from(t).where(t.Symbol == std::string("DUPLICATE")));
		EXPECT_TRUE(resBefore.size() >= 3);
		// Call the function under test
		gl_dataContainerFinnhubStock.DeleteDuplicatedSymbolFromDB();
		// Verify only one row remains for that symbol
		db = GetStockMarketDB();
		auto resAfter = db(select(all_of(t)).from(t).where(t.Symbol == std::string("DUPLICATE")));
		EXPECT_EQ(resAfter.size(), 1);

		// Clean up test data
		db = GetStockMarketDB(); // 执行完删除重复代码任务后，重新获取数据库连接，以确保看到最新的数据
		auto tx2 = start_transaction(db);
		db(remove_from(t).where(t.Symbol == std::string("DUPLICATE")));
		tx2.commit();
	}
}
