#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"DataSource.h"
#include"WebInquirer.h"

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

#include"MockFinnhubWebInquiry.h"
#include"MockQuandlWebInquiry.h"
#include"MockTiingoWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CMockFinnhubWebInquiryPtr s_pMockFinnhubWebInquiry;
	static CMockQuandlWebInquiryPtr s_pMockQuandlWebInquiry;
	static CMockTiingoWebInquiryPtr s_pMockTiingoWebInquiry;

	class CDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
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
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);
			s_pMockTiingoWebInquiry->SetReadingWebData(false);
			s_pMockQuandlWebInquiry->SetReadingWebData(false);

			FinnhubDataSource.SetWebInquiringPtr(s_pMockFinnhubWebInquiry);
			TiingoDataSource.SetWebInquiringPtr(s_pMockTiingoWebInquiry);
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockFinnhubWebInquiry->SetReadingWebData(false);
			s_pMockTiingoWebInquiry->SetReadingWebData(false);
			s_pMockQuandlWebInquiry->SetReadingWebData(false);

			s_pMockTiingoWebInquiry->SetInquiryingStringMiddle(_T(""));

			GeneralCheck();
		}

	protected:
		CDataSource dataSource;
		CDataSource FinnhubDataSource;
		CDataSource TiingoDataSource;
	};

	TEST_F(CDataSourceTest, Test_IsInquiry) {
		ASSERT_FALSE(dataSource.IsInquiring());
		dataSource.SetInquiring(true);
		ASSERT_TRUE(dataSource.IsInquiring());
		dataSource.SetInquiring(false);
		ASSERT_FALSE(dataSource.IsInquiring());
	}

	TEST_F(CDataSourceTest, Test_IsDataReceived) {
		ASSERT_TRUE(dataSource.IsDataReceived());
		dataSource.SetDataReceived(false);
		ASSERT_FALSE(dataSource.IsDataReceived());
		dataSource.SetDataReceived(true);
		ASSERT_TRUE(dataSource.IsDataReceived());
	}

	TEST_F(CDataSourceTest, Test_SetInquiry) {
		CProductWebSourceDataPtr p = std::make_shared<CProductWebSourceData>();
		p->SetIndex(10000);

		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
		dataSource.StoreInquiry(p);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 1);
		CProductWebSourceDataPtr p2 = dataSource.GetInquiry();
		EXPECT_EQ(p2->GetIndex(), 10000);
		EXPECT_EQ(dataSource.GetInquiryQueueSize(), 0);
	}

	TEST_F(CDataSourceTest, TestProcessInquiringMessage01) {
		while (FinnhubDataSource.GetInquiryQueueSize() > 0) FinnhubDataSource.GetInquiry();
		EXPECT_FALSE(FinnhubDataSource.ProcessInquiringMessage());
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage02) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(false);
		FinnhubDataSource.SetInquiring(true);
		EXPECT_FALSE(FinnhubDataSource.ProcessInquiringMessage()) << "Finnhub web data尚未接受到";

		// 恢复原状
		FinnhubDataSource.GetInquiry();
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false); // 无论是否使用mock，被操作的都是gl_pWorldMarket
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsProfileUpdated());
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfile)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__COMPANY_PROFILE_CONCISE__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsProfileUpdated());
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyProfileConcise)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_SYMBOLS__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);
		s_pMockFinnhubWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStockExchange(0)->m_strCode);
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockSymbol)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__PEERS__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsPeerUpdated());
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCompanyPeer)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetPeerUpdated(false);
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_EPS_SURPRISE__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockEstimatesEPSSurprise)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_QUOTE__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockPriceQuote)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__STOCK_CANDLES__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubStockDayLine)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__FOREX_SYMBOLS__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetForexExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexSymbol)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__FOREX_CANDLES__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);
		gl_pWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubForexDayLine)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__CRYPTO_SYMBOLS__) {
		CProductWebSourceDataPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetCryptoExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_TRUE(FinnhubDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductFinnhubCryptoSymbol)));
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubInquiringMessage__CRYPTO_CANDLES__) {
		CFinnhubCryptoDayLinePtr p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		FinnhubDataSource.StoreInquiry(p);
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(FinnhubDataSource.GetInquiryQueueSize(), 1);
		FinnhubDataSource.SetDataReceived(true);
		FinnhubDataSource.SetInquiring(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*s_pMockFinnhubWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(FinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockFinnhubWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr()
			+ gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_FALSE(FinnhubDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockFinnhubWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		FinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubWebDataReceived01) {
		FinnhubDataSource.SetDataReceived(false);
		EXPECT_FALSE(FinnhubDataSource.ProcessWebDataReceived());
	}

	TEST_F(CDataSourceTest, TestProcessFinnhubWebDataReceived02) {
		FinnhubDataSource.SetDataReceived(true);
		while (FinnhubDataSource.GetInquiryQueueSize() > 0) FinnhubDataSource.GetInquiry();

		EXPECT_FALSE(FinnhubDataSource.ProcessWebDataReceived());
	}

	TEST_F(CDataSourceTest, TestProcessTiingoInquiringMessage01) {
		while (TiingoDataSource.GetInquiryQueueSize() > 0) TiingoDataSource.GetInquiry();
		EXPECT_FALSE(TiingoDataSource.ProcessInquiringMessage());
	}

	TEST_F(CDataSourceTest, TestProcessTiingoInquiringMessage02) {
		CProductWebSourceDataPtr p = make_shared<CProductTinngoStockSymbol>();
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetDataReceived(false);
		TiingoDataSource.SetInquiring(true);
		EXPECT_FALSE(TiingoDataSource.ProcessInquiringMessage()) << "Tiingo web data尚未接受到";
		EXPECT_TRUE(TiingoDataSource.IsInquiring()) << "没有处理，故此标识没有重置";

		// 恢复原状
		TiingoDataSource.GetInquiry();
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestParseTiingoInquiringMessage__STOCK_SYMBOLS__) {
		CProductWebSourceDataPtr p = make_shared<CProductTinngoStockSymbol>();
		gl_pWorldMarket->GetStock(0)->SetProfileUpdated(false);
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetDataReceived(true);
		TiingoDataSource.SetInquiring(true);
		s_pMockTiingoWebInquiry->SetReadingWebData(false);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(TiingoDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringMiddle(), _T(""));

		// 顺便测试一下
		EXPECT_TRUE(TiingoDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTinngoStockSymbol)));
		EXPECT_FALSE(TiingoDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestParseTiingoInquiringMessage__STOCK_CANDLES__) {
		CProductWebSourceDataPtr p = make_shared<CProductTiingoStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		TiingoDataSource.StoreInquiry(p);
		EXPECT_EQ(TiingoDataSource.GetInquiryQueueSize(), 1);
		TiingoDataSource.SetDataReceived(true);
		TiingoDataSource.SetInquiring(true);

		EXPECT_CALL(*s_pMockTiingoWebInquiry, StartReadingThread())
			.Times(1);
		EXPECT_TRUE(TiingoDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(s_pMockTiingoWebInquiry->GetInquiringStringPrefix(),
			p->GetInquiringStr() + gl_pWorldMarket->GetStock(0)->GetTiingoDayLineInquiryString(gl_pWorldMarket->GetMarketDate()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate());
		// 顺便测试一下
		EXPECT_TRUE(TiingoDataSource.GetCurrentInquiry()->IsKindOf(RUNTIME_CLASS(CProductTiingoStockDayLine)));
		EXPECT_FALSE(TiingoDataSource.IsDataReceived());
		EXPECT_TRUE(s_pMockTiingoWebInquiry->IsReadingWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		TiingoDataSource.SetInquiring(false);
	}

	TEST_F(CDataSourceTest, TestProcessTiingoWebDataReceived01) {
		CProductWebSourceDataPtr p = make_shared<CProductTiingoStockDayLine>();
		TiingoDataSource.StoreInquiry(p);

		TiingoDataSource.SetDataReceived(false);

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived());
	}

	TEST_F(CDataSourceTest, TestProcessTiingoWebDataReceived02) {
		CProductWebSourceDataPtr p = make_shared<CProductTiingoStockDayLine>();
		TiingoDataSource.StoreInquiry(p);
		TiingoDataSource.SetDataReceived(true);
		while (TiingoDataSource.GetInquiryQueueSize() > 0) TiingoDataSource.GetInquiry();

		EXPECT_FALSE(TiingoDataSource.ProcessWebDataReceived());
	}
}