#include"pch.h"

#include"GeneralCheck.h"

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
}
