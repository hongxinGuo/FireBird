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
		CContainerFinnhubStock m_containerFinnhubStock;
	};

	TEST_F(CContainerFinnhubStockTest, TestResetBasicFinancial) {
		for (size_t l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetItem(l);
			EXPECT_TRUE(pStock->IsUpdateBasicFinancial());
			EXPECT_EQ(pStock->GetBasicFinancialUpdateDate(), 19800101);
			EXPECT_FALSE(pStock->IsUpdateProfileDB());

			pStock->SetUpdateBasicFinancial(false);
			pStock->SetUpdateProfileDB(false);
			pStock->SetBasicFinancialUpdateDate(20000101);
		}

		gl_dataContainerFinnhubStock.ResetBasicFinancial();

		for (size_t l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
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

	TEST_F(CContainerFinnhubStockTest, TestLoadProfileDB) {
		using namespace StockMarket;
		const auto& t = FinnhubStockProfile{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			db(sqlpp::insert_into(t).set(t.Symbol = "BVDRF", t.Exchange = "Test")); // 重复代码，用于测试，此时代码总数是5702
			tx.commit();
		}
		m_containerFinnhubStock.LoadProfileDB();
		EXPECT_EQ(m_containerFinnhubStock.Size(), 4847); // 2024-06-01Tiingo股票总共有7183只股票。如果数据库中有重复的股票代码，则会被删除，所以最终装载的股票数应该是5701。
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			auto result = db(sqlpp::select(all_of(t)).from(t).where(t.Symbol == "BVDRF"));
			tx.commit();
			size_t rows = result.size();
			EXPECT_EQ(rows, 1) << "数据库中应该只有一条BVDRF的记录";
			auto& row = result.front();
			EXPECT_EQ(row.Symbol.value(), "BVDRF");
			EXPECT_EQ(row.ID.value(), 32525);
		}
		m_containerFinnhubStock.Reset();
	}
}
