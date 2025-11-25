#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerFinnhubStock.h"

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
}
