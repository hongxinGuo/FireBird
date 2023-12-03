#include"pch.h"

#include"GeneralCheck.h"

#include"MockFinnhubDataSource.h"

#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"

#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	CMockFinnhubDataSourcePtr m_pMockFinnhubDataSource;

	class CMockFinnhubDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubDataSource, NotNull());
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			m_pMockFinnhubDataSource = make_shared<CMockFinnhubDataSource>();
		}

		void TearDown() override {
			// clearUp
			m_pMockFinnhubDataSource = nullptr;
			gl_pWorldMarket->SetSystemReady(true);
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockFinnhubDataSourceTest, TestGenerateInquiryMessage1) {
		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		gl_pSinaRTDataSource->SetErrorCode(12002);
		EXPECT_TRUE(gl_systemConfiguration.IsWebBusy());

		m_pMockFinnhubDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GetTickCount()).Times(1)
		.WillOnce(Return(1));
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEconomicCalendar()).Times(0);

		EXPECT_FALSE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120000)) << "网络报错，不申请数据";
		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());

		// 恢复原状
		gl_pSinaRTDataSource->SetErrorCode(0);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestGenerateInquiryMessage3) {
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_systemConfiguration.IsWebBusy());

		EXPECT_CALL(*m_pMockFinnhubDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime()))
		.WillOnce(Return(300000 + 100000));
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEconomicCalendar()).Times(1)
		.WillRepeatedly(DoAll(Invoke([p]() {
			m_pMockFinnhubDataSource->SetInquiring(true);
			m_pMockFinnhubDataSource->StoreInquiry(p);
		}), Return(true)));
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireMarketStatus()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireMarketHoliday()).Times(1);
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
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEPSSurprise()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireRTQuote()).Times(0);

		EXPECT_FALSE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120500)) << "时间未到，继续等待";
		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_FALSE(m_pMockFinnhubDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120500)) << "申请数据";

		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockFinnhubDataSource->HaveInquiry());
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub2) {
		m_pMockFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(false);
		InSequence Seq;
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEconomicCalendar()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireMarketStatus()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireMarketHoliday()).Times(1);
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

		m_pMockFinnhubDataSource->InquireFinnhub(120000);

		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("finnhub data inquiry finished"));
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub3) {
		m_pMockFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(true);
		InSequence Seq;
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEconomicCalendar()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireMarketStatus()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireMarketHoliday()).Times(1);
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
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireEPSSurprise()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, InquireRTQuote()).Times(0);

		m_pMockFinnhubDataSource->InquireFinnhub(120000);

		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str, _T("finnhub data inquiry finished"));
	}
}
