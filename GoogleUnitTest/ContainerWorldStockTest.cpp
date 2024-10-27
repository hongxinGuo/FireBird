#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerFinnhubStock.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerWorldStockTest : public Test {
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

	TEST_F(CContainerWorldStockTest, TestResetBasicFinancial) {
		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			EXPECT_TRUE(pStock->IsUpdateBasicFinancial());
			EXPECT_EQ(pStock->GetBasicFinancialUpdateDate(), 19800101);
			EXPECT_FALSE(pStock->IsUpdateProfileDB());

			pStock->SetUpdateBasicFinancial(false);
			pStock->SetUpdateProfileDB(false);
			pStock->SetBasicFinancialUpdateDate(20000101);
		}

		gl_dataContainerFinnhubStock.ResetBasicFinancial();

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			EXPECT_TRUE(pStock->IsUpdateBasicFinancial());
			EXPECT_EQ(pStock->GetBasicFinancialUpdateDate(), 19800101);
			EXPECT_TRUE(pStock->IsUpdateProfileDB());

			// »Ö¸´Ô­×´
			//pStock->SetUpdateBasicFinancial(false);
			pStock->SetUpdateProfileDB(false);
			//pStock->SetBasicFinancialUpdateDate(20000101);
		}
	}

	TEST_F(CContainerWorldStockTest, TestClearUpdateBasicFinancialFlag) {
		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			EXPECT_FALSE(pStock->IsUpdateBasicFinancialDB());
			pStock->SetUpdateBasicFinancialDB(true);
		}
		vector<CFinnhubStockPtr> vStock;
		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			vStock.push_back(pStock);
		}

		gl_dataContainerFinnhubStock.ClearUpdateBasicFinancialFlag(vStock);

		for (long l = 0; l < gl_dataContainerFinnhubStock.Size(); l++) {
			const auto pStock = gl_dataContainerFinnhubStock.GetStock(l);
			EXPECT_FALSE(pStock->IsUpdateBasicFinancialDB());
		}

		// »Ö¸´Ô­×´
	}

	TEST_F(CContainerWorldStockTest, TestValidateStockSymbol1) {
		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol(_T("AAPL"));
		pStock->SetExchangeCode(_T("US"));

		EXPECT_TRUE(gl_dataContainerFinnhubStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerWorldStockTest, TestValidateStockSymbol2) {
		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol(_T("600601.SS"));
		pStock->SetExchangeCode(_T("SS"));

		EXPECT_TRUE(gl_dataContainerFinnhubStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerWorldStockTest, TestValidateStockSymbol3) {
		const auto pStock = make_shared<CFinnhubStock>();
		pStock->SetSymbol(_T("600601SS"));
		pStock->SetExchangeCode(_T("SS"));

		EXPECT_FALSE(gl_dataContainerFinnhubStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerWorldStockTest, TestIsNeedSaveDayLine) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateDayLineDB());
		gl_dataContainerFinnhubStock.Get(0)->SetUpdateDayLineDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateDayLineDB());

		// »Ö¸´Ô­×´
		gl_dataContainerFinnhubStock.Get(0)->SetUpdateDayLineDB(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsUpdateInsiderTransactionDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB());
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateInsiderTransactionDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateInsiderTransactionDB());

		// »Ö¸´Ô­×´
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateInsiderTransactionDB(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsSaveInsiderSentiment) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB());
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateInsiderSentimentDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateInsiderSentimentDB());

		// »Ö¸´Ô­×´
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateInsiderSentimentDB(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsUpdateBasicFinancialDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB());
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateBasicFinancialDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateBasicFinancialDB());

		// »Ö¸´Ô­×´
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateBasicFinancialDB(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsUpdateCompanyNewsDB) {
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB());
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateCompanyNewsDB(true);

		EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateCompanyNewsDB());

		// »Ö¸´Ô­×´
		gl_dataContainerFinnhubStock.GetStock(0)->SetUpdateCompanyNewsDB(false);
	}
}
