///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"Thread.h"

#include"WebInquirer.h"

#include"MockWorldMarket.h"
#include"MockFinnhubWebInquiry.h"
#include"MockQuandlWebInquiry.h"
#include"MockTiingoWebInquiry.h"

#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
#include"ProductFinnhubCompanyInsiderTransaction.h"

#include"ProductFinnhubStockEstimatesEPSSurprise.h"

#include"ProductFinnhubCryptoExchange.h"
#include"ProductFinnhubForexExchange.h"

#include"ProductFinnhubStockPriceQuote.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"ProductFinnhubStockDayLine.h"

#include"PRoductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubStockSymbol.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"

using namespace std;
using namespace testing;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	extern CMockWorldMarketPtr gl_pMockWorldMarket;

	static CMockFinnhubWebInquiryPtr s_pMockFinnhubWebInquiry;
	static CMockQuandlWebInquiryPtr s_pMockQuandlWebInquiry;
	static CMockTiingoWebInquiryPtr s_pMockTiingoWebInquiry;

	class CMockWorldMarketTest : public ::testing::Test
	{
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

		virtual void SetUp(void) override {
			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
			gl_fExitingSystem = false;
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);
			s_pMockTiingoWebInquiry->SetReadingWebData(false);
			s_pMockQuandlWebInquiry->SetReadingWebData(false);
			gl_pDataSourceFinnhub->SetStockProfileUpdated(false);
			EXPECT_EQ(gl_pDataSourceFinnhub->GetInquiryQueueSize(), 0);
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 0);

			s_pMockFinnhubWebInquiry->SetReadingWebData(false);
			s_pMockTiingoWebInquiry->SetReadingWebData(false);
			s_pMockQuandlWebInquiry->SetReadingWebData(false);

			s_pMockTiingoWebInquiry->SetInquiryingStringMiddle(_T(""));
			gl_pMockWorldMarket->SetSystemReady(true);

			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringSuffix(), _T("&token=bv985d748v6ujthqfke0"));
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringSuffix(), _T("&token=c897a00b7cfc2adffc630d23befd5316a4683156"));
		EXPECT_STREQ(s_pMockQuandlWebInquiry->GetInquiringStringSuffix(), _T("&api_key=zBMXMyoTyiy_N3pMb3ex"));

		s_pMockFinnhubWebInquiry->SetInquiryingStringSuffix(_T(""));
		s_pMockTiingoWebInquiry->SetInquiryingStringSuffix(_T(""));
		s_pMockQuandlWebInquiry->SetInquiryingStringSuffix(_T(""));
		gl_pMockWorldMarket->UpdateToken();
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringSuffix(), _T("&token=bv985d748v6ujthqfke0"));
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringSuffix(), _T("&token=c897a00b7cfc2adffc630d23befd5316a4683156"));
		EXPECT_STREQ(s_pMockQuandlWebInquiry->GetInquiringStringSuffix(), _T("&api_key=zBMXMyoTyiy_N3pMb3ex"));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCountryListDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateCountryListDB)
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_EQ(ThreadUpdateCountryListDB(gl_pMockWorldMarket.get()), (UINT)40);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockProfileDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockProfileDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateStockProfileDB(gl_pMockWorldMarket.get()), (UINT)37);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockDayLineDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockDayLineDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineDB(gl_pMockWorldMarket.get()), (UINT)42);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexSymbolDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateForexSymbolDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateForexSymbolDB(gl_pMockWorldMarket.get()), (UINT)39);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexExchangedDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateForexExchangeDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateForexExchangeDB(gl_pMockWorldMarket.get()), (UINT)49);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateFinnhubCryptoSymbolDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateFinnhubCryptoSymbolDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateFinnhubCryptoSymbolDB(gl_pMockWorldMarket.get()), (UINT)52);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCryptoExchangedDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateCryptoExchangeDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateCryptoExchangeDB(gl_pMockWorldMarket.get()), (UINT)51);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderTransactionDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateInsiderTransactionDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateInsiderTransactionDB(gl_pMockWorldMarket.get()), (UINT)48);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoStockDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateTiingoStockDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoStockDB(gl_pMockWorldMarket.get()), (UINT)44);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoIndustry) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateTiingoIndustry)
			.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoIndustry(gl_pMockWorldMarket.get()), (UINT)45);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateSICIndustry) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateSICIndustry)
			.Times(1);
		EXPECT_EQ(ThreadUpdateSICIndustry(gl_pMockWorldMarket.get()), (UINT)46);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateNaicsIndustry) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateNaicsIndustry)
			.Times(1);
		EXPECT_EQ(ThreadUpdateNaicsIndustry(gl_pMockWorldMarket.get()), (UINT)47);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateDayLineStartEndDate) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateStockDayLineStartEndDate)
			.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineStartEndDate(gl_pMockWorldMarket.get()), (UINT)43);
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateEconmicCalendarDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateEconomicCalendarDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateEconomicCalendarDB(gl_pMockWorldMarket.get()), (UINT)50);
	}
}