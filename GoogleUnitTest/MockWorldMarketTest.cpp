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
	CMockWorldMarketPtr s_pMockWorldMarket;

	class CMockWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			s_pMockWorldMarket = make_shared<CMockWorldMarket>(); // 在此生成，
			s_pMockWorldMarket->ResetMarket();
			EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
			gl_systemMessage.PopInformationMessage();
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
		}

		static void TearDownTestSuite(void) {
			s_pMockWorldMarket = nullptr;

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

			s_pMockWorldMarket->SetSystemReady(true);

			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		s_pMockWorldMarket->UpdateToken();
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCountryListDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateCountryListDB)
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_EQ(ThreadUpdateCountryListDB(s_pMockWorldMarket.get()), static_cast<UINT>(40));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockProfileDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateStockProfileDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateWorldMarketStockProfileDB(s_pMockWorldMarket.get()), static_cast<UINT>(37));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockDayLineDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateStockDayLineDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineDB(s_pMockWorldMarket.get()), static_cast<UINT>(42));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexSymbolDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateForexSymbolDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateForexSymbolDB(s_pMockWorldMarket.get()), static_cast<UINT>(39));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexExchangedDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateForexExchangeDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateForexExchangeDB(s_pMockWorldMarket.get()), static_cast<UINT>(49));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateFinnhubCryptoSymbolDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateFinnhubCryptoSymbolDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateFinnhubCryptoSymbolDB(s_pMockWorldMarket.get()), static_cast<UINT>(52));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCryptoExchangedDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateCryptoExchangeDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateCryptoExchangeDB(s_pMockWorldMarket.get()), static_cast<UINT>(51));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderTransactionDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateInsiderTransactionDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateInsiderTransactionDB(s_pMockWorldMarket.get()), static_cast<UINT>(48));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderSentimentDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateInsiderSentimentDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateInsiderSentimentDB(s_pMockWorldMarket.get()), static_cast<UINT>(58));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoStockDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateTiingoStockDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoStockDB(s_pMockWorldMarket.get()), static_cast<UINT>(44));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateTiingoIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoIndustry(s_pMockWorldMarket.get()), static_cast<UINT>(45));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateSICIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateSICIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateSICIndustry(s_pMockWorldMarket.get()), static_cast<UINT>(46));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateNaicsIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateNaicsIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateNaicsIndustry(s_pMockWorldMarket.get()), static_cast<UINT>(47));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateDayLineStartEndDate) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateStockDayLineStartEndDate)
		.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineStartEndDate(s_pMockWorldMarket.get()), static_cast<UINT>(43));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateEconmicCalendarDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateEconomicCalendarDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateEconomicCalendarDB(s_pMockWorldMarket.get()), static_cast<UINT>(50));
	}
}
