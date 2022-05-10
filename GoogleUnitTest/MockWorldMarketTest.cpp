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
			gl_pMockWorldMarket->SetFinnhubStockProfileUpdated(false);
			EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 0);
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

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage01) {
		while (gl_pMockWorldMarket->GetFinnhubInquiryQueueSize() > 0) gl_pMockWorldMarket->GetFinnhubInquiry();
		EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage02) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(false);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);
		EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage()) << "Finnhub web data尚未接受到";

		// 恢复原状
		gl_pMockWorldMarket->GetFinnhubInquiry();
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false); // 无论是否使用mock，被操作的都是gl_pWorldMarket
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pMockWorldMarket->GetStock(0)->IsProfileUpdated());
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfile)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE_CONCISE__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pMockWorldMarket->GetStock(0)->IsProfileUpdated());
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_SYMBOLS__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);
		s_pMockFinnhubWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStockExchange(0)->m_strCode);
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockSymbol)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__PEERS__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->GetStock(0)->SetPeerUpdated(false);
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pMockWorldMarket->GetStock(0)->IsPeerUpdated());
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->GetStock(0)->SetPeerUpdated(false);
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_EPS_SURPRISE__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_QUOTE__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__STOCK_CANDLES__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryString(gl_pMockWorldMarket->GetUTCTime()));
		EXPECT_FALSE(gl_pMockWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__FOREX_SYMBOLS__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetForexExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__FOREX_CANDLES__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);
		gl_pMockWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pMockWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_FALSE(gl_pMockWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__CRYPTO_SYMBOLS__) {
		CWebSourceDataProductPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetCryptoExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentFinnhubInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoSymbol)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubInquiringMessage__CRYPTO_CANDLES__) {
		CFinnhubCryptoDayLinePtr p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->PushFinnhubInquiry(p);
		EXPECT_TRUE(gl_pMockWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(gl_pMockWorldMarket->GetFinnhubInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		gl_pMockWorldMarket->SetFinnhubInquiring(true);
		gl_pMockWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessFinnhubInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr()
			+ gl_pMockWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pMockWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_FALSE(gl_pMockWorldMarket->IsFinnhubDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetFinnhubInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived01) {
		gl_pMockWorldMarket->SetFinnhubDataReceived(false);
		EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
	}

	TEST_F(CMockWorldMarketTest, TestProcessFinnhubWebDataReceived02) {
		gl_pMockWorldMarket->SetFinnhubDataReceived(true);
		while (gl_WebInquirer.GetFinnhubDataSize() > 0) gl_WebInquirer.PopFinnhubData();

		EXPECT_FALSE(gl_pMockWorldMarket->ProcessFinnhubWebDataReceived());
	}

	TEST_F(CMockWorldMarketTest, TestProcessTiingoInquiringMessage01) {
		while (gl_pMockWorldMarket->GetTiingoInquiryQueueSize() > 0) gl_pMockWorldMarket->GetTiingoInquiry();
		EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage());
	}

	TEST_F(CMockWorldMarketTest, TestProcessTiingoInquiringMessage02) {
		CWebSourceDataProductPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pMockWorldMarket->PushTiingoInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetTiingoInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetTiingoDataReceived(false);
		gl_pMockWorldMarket->SetTiingoInquiring(true);
		EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage()) << "Tiingo web data尚未接受到";
		EXPECT_TRUE(gl_pMockWorldMarket->IsTiingoInquiring()) << "没有处理，故此标识没有重置";

		// 恢复原状
		gl_pMockWorldMarket->GetTiingoInquiry();
		gl_pMockWorldMarket->SetTiingoInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestParseTiingoInquiringMessage__STOCK_SYMBOLS__) {
		CWebSourceDataProductPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pMockWorldMarket->GetStock(0)->SetProfileUpdated(false);
		gl_pMockWorldMarket->PushTiingoInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetTiingoInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetTiingoDataReceived(true);
		gl_pMockWorldMarket->SetTiingoInquiring(true);
		s_pMockTiingoWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringMiddle(), _T(""));

		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentTiingoInquiry()->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsTiingoDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->SetTiingoInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestParseTiingoInquiringMessage__STOCK_CANDLES__) {
		CWebSourceDataProductPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pMockWorldMarket.get());
		gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pMockWorldMarket->PushTiingoInquiry(p);
		EXPECT_EQ(gl_pMockWorldMarket->GetTiingoInquiryQueueSize(), 1);
		gl_pMockWorldMarket->SetTiingoDataReceived(true);
		gl_pMockWorldMarket->SetTiingoInquiring(true);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(gl_pMockWorldMarket->ProcessTiingoInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pMockWorldMarket->GetStock(0)->GetTiingoDayLineInquiryString(gl_pMockWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pMockWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// 顺便测试一下
		EXPECT_TRUE(gl_pMockWorldMarket->GetCurrentTiingoInquiry()->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_FALSE(gl_pMockWorldMarket->IsTiingoDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pMockWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pMockWorldMarket->SetTiingoInquiring(false);
	}

	TEST_F(CMockWorldMarketTest, TestProcessTiingoWebDataReceived01) {
		CWebSourceDataProductPtr p = make_shared<CProductTiingoStockDayLine>();
		gl_pMockWorldMarket->SetCurrentTiingoInquiry(p);

		gl_pMockWorldMarket->SetTiingoDataReceived(false);

		EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoWebDataReceived());
	}

	TEST_F(CMockWorldMarketTest, TestProcessTiingoWebDataReceived02) {
		CWebSourceDataProductPtr p = make_shared<CProductTiingoStockDayLine>();
		gl_pMockWorldMarket->SetCurrentTiingoInquiry(p);
		gl_pMockWorldMarket->SetTiingoDataReceived(true);
		while (gl_WebInquirer.GetTiingoDataSize() > 0) gl_WebInquirer.PopTiingoData();

		EXPECT_FALSE(gl_pMockWorldMarket->ProcessTiingoWebDataReceived());
	}

	TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub1) {
		EXPECT_FALSE(gl_pMockWorldMarket->TaskInquiryFinnhub(165700));
		EXPECT_FALSE(gl_pMockWorldMarket->TaskInquiryFinnhub(170200));
	}

	TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub2) {
		gl_pMockWorldMarket->SetSystemReady(false);
		InSequence seq;
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCountryList).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexExchange).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoExchange).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanySymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexSymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoSymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEconomicCalendar).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyProfileConcise).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyBasicFinancial).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubPeer).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubInsiderTransaction).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEPSSurprise).Times(0);
		//EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexDayLine).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoDayLine).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubStockDayLine).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubRTQuote).Times(0);

		EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryFinnhub(170501));
	}

	TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub3) {
		gl_pMockWorldMarket->SetSystemReady(true);
		gl_pMockWorldMarket->SetFinnhubDayLineUpdated(false);
		InSequence seq;
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCountryList).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexExchange).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoExchange).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanySymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexSymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoSymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEconomicCalendar).Times(0); //"目前未使用"
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyProfileConcise).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyBasicFinancial).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubPeer).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubInsiderTransaction).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEPSSurprise).Times(0); //"目前未使用"
		//EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexDayLine).Times(1); // Finnhub Forex DayLine为Premium数据了
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoDayLine).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubStockDayLine).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubRTQuote).Times(0);

		EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryFinnhub(165659));
	}

	TEST_F(CMockWorldMarketTest, TestTaskInquiryFinnhub4) {
		gl_pMockWorldMarket->SetSystemReady(true);
		gl_pMockWorldMarket->SetFinnhubDayLineUpdated(true);
		InSequence seq;
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCountryList).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexExchange).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoExchange).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanySymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexSymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoSymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEconomicCalendar).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyProfileConcise).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCompanyBasicFinancial).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubPeer).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubInsiderTransaction).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubEPSSurprise).Times(0);
		//EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubForexDayLine).Times(1);// Finnhub Forex DayLine为Premium数据了
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubCryptoDayLine).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubStockDayLine).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryFinnhubRTQuote).Times(0);

		EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryFinnhub(165659));
	}

	TEST_F(CMockWorldMarketTest, TestTaskInquiryTiingo1) {
		gl_pMockWorldMarket->SetSystemReady(false);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoCompanySymbol).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoCryptoSymbol).Times(0);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoDayLine).Times(0);

		EXPECT_FALSE(gl_pMockWorldMarket->TaskInquiryTiingo());
	}
	TEST_F(CMockWorldMarketTest, TestTaskInquiryTiingo2) {
		gl_pMockWorldMarket->SetSystemReady(true);
		InSequence seq;
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoCompanySymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoCryptoSymbol).Times(1);
		EXPECT_CALL(*gl_pMockWorldMarket, TaskInquiryTiingoDayLine).Times(1);

		EXPECT_TRUE(gl_pMockWorldMarket->TaskInquiryTiingo());
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateEconmicCalendarDB) {
		EXPECT_CALL(*gl_pMockWorldMarket, UpdateEconomicCalendarDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateEconomicCalendarDB(gl_pMockWorldMarket.get()), (UINT)50);
	}
}