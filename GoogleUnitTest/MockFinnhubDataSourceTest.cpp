#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"
#include"MockFinnhubDataSource.h"

#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"ProductFinnhubStockDayLine.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include"ProductFinnhubStockPriceQuote.h"

#include"ProductTiingoStockSymbol.h"
#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	CMockFinnhubDataSourcePtr m_pMockFinnhubDataSource;

	class CMockFinnhubDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubDataSource, NotNull());
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
			m_pMockFinnhubDataSource = make_shared<CMockFinnhubDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pMockFinnhubDataSource = nullptr;
			gl_pWorldMarket->SetSystemReady(true);
			GeneralCheck();
		}

	protected:
	};

	/*
	TEST_F(CMockFinnhubDataSourceTest, TestGenerateInquiryMessage1) {
		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		m_pMockFinnhubDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(0))
		.WillOnce(Return(300000 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + 10000));
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEconomicCalendar()).Times(1)
		.WillOnce(DoAll(Invoke([]() { m_pMockFinnhubDataSource->SetInquiring(true); }), Return(true)));
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanySymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexSymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoSymbol()).Times(1);

		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyProfileConcise()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyNews()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyBasicFinancial()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquirePeer()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireInsiderTransaction()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireInsiderSentiment()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireStockDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEPSSurprise()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireRTQuote()).Times(0);

		EXPECT_FALSE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120000)) << "网络报错，延后五分钟";
		m_pMockFinnhubDataSource->SetErrorCode(0);
		EXPECT_FALSE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120500)) << "未过五分钟，继续等待";
		EXPECT_TRUE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120500)) << "已过五分钟，申请数据";

		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiring());
	}
	*/
	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub1) {
		m_pMockFinnhubDataSource->SetInquiring(false);
		EXPECT_FALSE(m_pMockFinnhubDataSource->InquireFinnhub(165700));
		EXPECT_FALSE(m_pMockFinnhubDataSource->InquireFinnhub(170000));
		EXPECT_FALSE(m_pMockFinnhubDataSource->InquireFinnhub(170500));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub2) {
		m_pMockFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(false);
		InSequence Seq;
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEconomicCalendar()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanySymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexSymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoSymbol()).Times(1);

		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyProfileConcise()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyNews()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyBasicFinancial()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquirePeer()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireInsiderTransaction()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireInsiderSentiment()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoDayLine()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireStockDayLine()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexDayLine()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEPSSurprise()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireRTQuote()).Times(0);

		EXPECT_TRUE(m_pMockFinnhubDataSource->InquireFinnhub(120000));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub3) {
		m_pMockFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(true);
		InSequence Seq;
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEconomicCalendar()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanySymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexSymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoSymbol()).Times(1);

		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyProfileConcise()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyNews()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCompanyBasicFinancial()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquirePeer()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireInsiderTransaction()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireInsiderSentiment()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireStockDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEPSSurprise()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireRTQuote()).Times(0);

		EXPECT_TRUE(m_pMockFinnhubDataSource->InquireFinnhub(120000));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfile>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true); // 无论是否使用mock，被操作的都是gl_pWorldMarket
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfile"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_COMPANY_PROFILE_CONCISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyProfileConcise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());

		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdateCompanyProfile()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyProfileConcise"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdateCompanyProfile(true);
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetStockExchange(0)->m_strCode);
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockSymbol"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_PEERS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCompanyPeer>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsUpdatePeer()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCompanyPeer"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetUpdatePeer(true);
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_EPS_SURPRISE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockEstimatesEPSSurprise"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_QUOTE_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockPriceQuote>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetSymbol());
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockPriceQuote"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_STOCK_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubStockDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetStock(0)->GetFinnhubDayLineInquiryParam(GetUTCTime()));
		EXPECT_TRUE(gl_pWorldMarket->GetStock(0)->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubStockDayLine"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		gl_pWorldMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetForexExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubForexSymbol"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_FOREX_CANDLES_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubForexDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);
		gl_pWorldMarket->GetForexSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetForexSymbol(p->GetIndex())->GetFinnhubDayLineInquiryParam(GetUTCTime()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubForexDayLine"));
		EXPECT_TRUE(gl_pWorldMarket->GetForexSymbol(p->GetIndex())->IsDayLineNeedUpdate()) << "接收到的数据处理后方设置此标识";
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_SYMBOLS_) {
		CVirtualProductWebDataPtr p = make_shared<CProductFinnhubCryptoSymbol>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetCryptoExchange(p->GetIndex()));
		// 顺便测试一下
		EXPECT_STREQ(typeid(*m_pMockFinnhubDataSource->GetCurrentInquiry()).name(), _T("class CProductFinnhubCryptoSymbol"));
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubInquiringMessage_CRYPTO_CANDLES_) {
		auto p = make_shared<CProductFinnhubCryptoDayLine>();
		p->SetIndex(0);
		p->SetMarket(gl_pWorldMarket.get());
		m_pMockFinnhubDataSource->StoreInquiry(p);
		EXPECT_TRUE(gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->IsDayLineNeedUpdate());
		EXPECT_EQ(m_pMockFinnhubDataSource->GetInquiryQueueSize(), 1);
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		m_pMockFinnhubDataSource->SetInquiring(true);
		gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->SetDayLineNeedUpdate(true);

		EXPECT_CALL(*m_pMockFinnhubDataSource, StartReadingThread()).Times(1);
		EXPECT_TRUE(m_pMockFinnhubDataSource->GetWebData());
		EXPECT_STREQ(m_pMockFinnhubDataSource->GetInquiryFunction(), p->GetInquiryFunction() + gl_pWorldMarket->GetFinnhubCryptoSymbol(p->GetIndex())->GetFinnhubDayLineInquiryParam(GetUTCTime()));
		// 顺便测试一下
		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiringWebData()) << "由于使用了Mock方式，结果此标识没有重置。需要在TearDown中手工重置之";

		// 恢复原状
		m_pMockFinnhubDataSource->SetInquiring(false);
	}

	//todo 以下测试尚未完成
	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived01) {
		m_pMockFinnhubDataSource->SetInquiringWebData(true);
		EXPECT_FALSE(m_pMockFinnhubDataSource->ProcessWebDataReceived());
	}

	TEST_F(CMockFinnhubDataSourceTest, TestProcessFinnhubWebDataReceived02) {
		m_pMockFinnhubDataSource->SetInquiringWebData(false);
		while (m_pMockFinnhubDataSource->GetInquiryQueueSize() > 0) m_pMockFinnhubDataSource->GetCurrentProduct();

		EXPECT_FALSE(m_pMockFinnhubDataSource->ProcessWebDataReceived());
	}
}
