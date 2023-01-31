///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"MockWorldMarket.h"

#include"ProductFinnhubCompanyInsiderTransaction.h"

#include"PRoductFinnhubForexSymbol.h"
#include"FinnhubDataSource.h"

using namespace testing;
#include<memory>

namespace FireBirdTest {
	extern CMockWorldMarketPtr gl_pMockWorldMarket;

	class CMockWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			EXPECT_TRUE(gl_pMockWorldMarket != nullptr) << "此Mock变量在EnvironmentSetUp.h中生成";
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
			gl_systemStatus.SetExitingSystem(false);
			gl_pFinnhubDataSource->SetUpdateStockProfile(true);
			EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
		}

		void TearDown(void) override {
			// clearUp
			EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);

			gl_pMockWorldMarket->SetSystemReady(true);

			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		gl_pMockWorldMarket->UpdateToken();
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCountryListDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateCountryListDB)
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadUpdateCountryListDB(gl_pMockWorldMarket.get()), static_cast<UINT>(40));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockProfileDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockProfileDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateWorldMarketStockProfileDB(gl_pMockWorldMarket.get()), static_cast<UINT>(37));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockDayLineDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockDayLineDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineDB(gl_pMockWorldMarket.get()), static_cast<UINT>(42));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexSymbolDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateForexSymbolDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateForexSymbolDB(gl_pMockWorldMarket.get()), static_cast<UINT>(39));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexExchangedDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateForexExchangeDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateForexExchangeDB(gl_pMockWorldMarket.get()), static_cast<UINT>(49));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateFinnhubCryptoSymbolDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateFinnhubCryptoSymbolDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateFinnhubCryptoSymbolDB(gl_pMockWorldMarket.get()), static_cast<UINT>(52));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCryptoExchangedDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateCryptoExchangeDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateCryptoExchangeDB(gl_pMockWorldMarket.get()), static_cast<UINT>(51));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderTransactionDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateInsiderTransactionDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateInsiderTransactionDB(gl_pMockWorldMarket.get()), static_cast<UINT>(48));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderSentimentDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateInsiderSentimentDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateInsiderSentimentDB(gl_pMockWorldMarket.get()), static_cast<UINT>(58));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoStockDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateTiingoStockDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoStockDB(gl_pMockWorldMarket.get()), static_cast<UINT>(44));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoIndustry) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateTiingoIndustry)
			.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoIndustry(gl_pMockWorldMarket.get()), static_cast<UINT>(45));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateSICIndustry) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateSICIndustry)
			.Times(1);
		EXPECT_EQ(ThreadUpdateSICIndustry(gl_pMockWorldMarket.get()), static_cast<UINT>(46));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateNaicsIndustry) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateNaicsIndustry)
			.Times(1);
		EXPECT_EQ(ThreadUpdateNaicsIndustry(gl_pMockWorldMarket.get()), static_cast<UINT>(47));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateDayLineStartEndDate) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockDayLineStartEndDate)
			.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineStartEndDate(gl_pMockWorldMarket.get()), static_cast<UINT>(43));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateEconmicCalendarDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateEconomicCalendarDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateEconomicCalendarDB(gl_pMockWorldMarket.get()), static_cast<UINT>(50));
	}
}
