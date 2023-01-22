///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"MockWorldMarket.h"
#include"MockFinnhubWebInquiry.h"
#include"MockQuandlWebInquiry.h"
#include"MockTiingoWebInquiry.h"

#include"ProductFinnhubCompanyInsiderTransaction.h"

#include"PRoductFinnhubForexSymbol.h"
#include"FinnhubDataSource.h"

using namespace testing;
#include<memory>

namespace FireBirdTest {
	extern CMockWorldMarketPtr gl_pMockWorldMarket;

	static CMockFinnhubWebInquiryPtr s_pMockFinnhubWebInquiry;
	static CMockQuandlWebInquiryPtr s_pMockQuandlWebInquiry;
	static CMockTiingoWebInquiryPtr s_pMockTiingoWebInquiry;

	class CMockWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			EXPECT_TRUE(gl_pMockWorldMarket != nullptr) << "此Mock变量在EnvironmentSetUp.h中生成";

			ASSERT_THAT(gl_pFinnhubWebInquiry, NotNull());
			s_pMockFinnhubWebInquiry = static_pointer_cast<CMockFinnhubWebInquiry>(gl_pFinnhubWebInquiry);
			ASSERT_THAT(gl_pQuandlWebInquiry, NotNull());
			s_pMockQuandlWebInquiry = static_pointer_cast<CMockQuandlWebInquiry>(gl_pQuandlWebInquiry);
			ASSERT_THAT(gl_pTiingoWebInquiry, NotNull());
			s_pMockTiingoWebInquiry = static_pointer_cast<CMockTiingoWebInquiry>(gl_pTiingoWebInquiry);
		}

		static void TearDownTestSuite(void) {
			s_pMockFinnhubWebInquiry = nullptr;
			s_pMockQuandlWebInquiry = nullptr;
			s_pMockTiingoWebInquiry = nullptr;
			GeneralCheck();
		}

		void SetUp(void) override {
			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
			gl_systemStatus.SetExitingSystem(false);
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);
			s_pMockTiingoWebInquiry->SetReadingWebData(false);
			s_pMockQuandlWebInquiry->SetReadingWebData(false);
			gl_pFinnhubDataSource->SetUpdateStockProfile(true);
			EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
		}

		void TearDown(void) override {
			// clearUp
			EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);

			s_pMockFinnhubWebInquiry->SetReadingWebData(false);
			s_pMockTiingoWebInquiry->SetReadingWebData(false);
			s_pMockQuandlWebInquiry->SetReadingWebData(false);

			gl_pMockWorldMarket->SetSystemReady(true);

			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryToken(), _T("bv985d748v6ujthqfke0"));
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiryToken(), _T("c897a00b7cfc2adffc630d23befd5316a4683156"));
		EXPECT_STREQ(s_pMockQuandlWebInquiry->GetInquiryToken(), _T("zBMXMyoTyiy_N3pMb3ex"));

		s_pMockFinnhubWebInquiry->SetInquiryToken(_T(""));
		s_pMockTiingoWebInquiry->SetInquiryToken(_T(""));
		s_pMockQuandlWebInquiry->SetInquiryToken(_T(""));
		gl_pMockWorldMarket->UpdateToken();
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiryToken(), _T("bv985d748v6ujthqfke0"));
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiryToken(), _T("c897a00b7cfc2adffc630d23befd5316a4683156"));
		EXPECT_STREQ(s_pMockQuandlWebInquiry->GetInquiryToken(), _T("zBMXMyoTyiy_N3pMb3ex"));
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
