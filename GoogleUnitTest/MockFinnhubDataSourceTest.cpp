#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"
#include"MockFinnhubDataSource.h"

#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
//#include"ProductFinnhubForexExchange.h"
//#include"ProductFinnhubEconomicCountryList.h"
//#include"ProductFinnhubEconomicCalendar.h"
//#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
//#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"ProductFinnhubStockDayLine.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include"ProductFinnhubStockPriceQuote.h"

#include"ProductTiingoStockSymbol.h"
#include"ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CMockFinnhubDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubDataSource, NotNull());
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			gl_pWorldMarket->SetSystemReady(true);
			GeneralCheck();
		}

	protected:
		CMockFinnhubDataSource m_MockFinnhubDataSource;
	};

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub1) {
		m_MockFinnhubDataSource.SetInquiring(false);
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireFinnhub(165700));
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireFinnhub(170000));
		EXPECT_FALSE(m_MockFinnhubDataSource.InquireFinnhub(170500));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub2) {
		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(false);
		InSequence Seq;
		EXPECT_CALL(m_MockFinnhubDataSource, InquireEconomicCalendar()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanySymbol()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireForexSymbol()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCryptoSymbol()).Times(1);

		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanyProfileConcise()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanyNews()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanyBasicFinancial()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquirePeer()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCryptoDayLine()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireStockDayLine()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireForexDayLine()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireEPSSurprise()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireRTQuote()).Times(0);

		EXPECT_TRUE(m_MockFinnhubDataSource.InquireFinnhub(120000));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub3) {
		m_MockFinnhubDataSource.SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(true);
		InSequence Seq;
		EXPECT_CALL(m_MockFinnhubDataSource, InquireEconomicCalendar()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanySymbol()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireForexSymbol()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCryptoSymbol()).Times(1);

		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanyProfileConcise()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanyNews()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCompanyBasicFinancial()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquirePeer()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireCryptoDayLine()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireStockDayLine()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireForexDayLine()).Times(1);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireEPSSurprise()).Times(0);
		EXPECT_CALL(m_MockFinnhubDataSource, InquireRTQuote()).Times(0);

		EXPECT_TRUE(m_MockFinnhubDataSource.InquireFinnhub(120000));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true); // 无论是否使用mock，被操作的都是gl_pWorldMarket
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfile"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_CONCISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);
		m_MockFinnhubDataSource.SetInquiringWebData(false);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetStockExchange(0)->m_strCode);
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_PEERS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdatePeer()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_EPS_SURPRISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_QUOTE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockPriceQuote"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryParam(gl_pWorldMarket->GetUTCTime()));
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetForexExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);
		gl_pWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryParam(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetCryptoExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_MockFinnhubDataSource.GetCurrentInquiry()).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_CANDLES_) {
		auto p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_MockFinnhubDataSource.StoreInquiry(p);
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(m_MockFinnhubDataSource.GetInquiryQueueSize(), 1);
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		m_MockFinnhubDataSource.SetInquiring(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(m_MockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_MockFinnhubDataSource.ProcessInquiringMessage());
		EXPECT_STREQ(m_MockFinnhubDataSource.GetInquiryFunction(), p->GetInquiry() + gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryParam(gl_pWorldMarket->GetUTCTime()));
		// 顺便测试一下
		EXPECT_TRUE(m_MockFinnhubDataSource.IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_MockFinnhubDataSource.SetInquiring(false);
	}

	//todo 以下测试尚未完成
	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived01) {
		m_MockFinnhubDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockFinnhubDataSource.ProcessWebDataReceived());
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived02) {
		m_MockFinnhubDataSource.SetInquiringWebData(false);
		while (m_MockFinnhubDataSource.GetInquiryQueueSize() > 0) m_MockFinnhubDataSource.GetInquiry();

		EXPECT_FALSE(m_MockFinnhubDataSource.ProcessWebDataReceived());
	}
}
