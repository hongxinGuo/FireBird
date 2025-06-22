#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"MockFinnhubDataSource.h"

#include"ProductFinnhubStockSymbol.h"
#include "SinaRTDataSource.h"

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
		auto timePoint = chrono::time_point<chrono::steady_clock>();

		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		gl_pSinaRTDataSource->SetWebError(true);
		EXPECT_TRUE(gl_systemConfiguration.IsWebBusy());

		m_pMockFinnhubDataSource->SetWebError(true);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GetTickCount()).Times(1)
		.WillOnce(Return(timePoint + 1ms));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateEconomicCalendar()).Times(0);

		EXPECT_FALSE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120000)) << "网络报错，不申请数据";
		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());

		// 恢复原状
		gl_pSinaRTDataSource->SetWebError(false);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestGenerateInquiryMessage3) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();

		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_systemConfiguration.IsWebBusy());

		EXPECT_CALL(*m_pMockFinnhubDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime()))
		.WillOnce(Return(timePoint + 300000ms + 100000ms));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateEconomicCalendar()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCountryList()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexExchange()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoExchange()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateMarketStatus()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateMarketHoliday()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanySymbol()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexSymbol()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoSymbol()).Times(1)
		.WillOnce(Return(false));

		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyProfileConcise()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyNews()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyBasicFinancial()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GeneratePeer()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateInsiderTransaction()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateInsiderSentiment()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoDayLine()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateStockDayLine()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexDayLine()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateEPSSurprise()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateSECFilings()).Times(1)
		.WillRepeatedly(DoAll(Invoke([p]() {
			m_pMockFinnhubDataSource->SetInquiring(true);
			m_pMockFinnhubDataSource->StoreInquiry(p);
		}), Return(true)));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateRTQuote()).Times(0);

		EXPECT_FALSE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120500)) << "时间未到，继续等待";
		EXPECT_FALSE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_FALSE(m_pMockFinnhubDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockFinnhubDataSource->GenerateInquiryMessage(120500)) << "申请数据";

		EXPECT_TRUE(m_pMockFinnhubDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockFinnhubDataSource->HaveInquiry());
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub2) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();

		m_pMockFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(false);
		InSequence Seq;
		EXPECT_CALL(*m_pMockFinnhubDataSource, GetTickCount()).Times(1)
		.WillOnce(Return(timePoint + 300000ms + 100000ms));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateEconomicCalendar()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateMarketStatus()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateMarketHoliday()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanySymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexSymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoSymbol()).Times(1);

		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateSECFilings()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyProfileConcise()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyNews()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyBasicFinancial()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GeneratePeer()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateInsiderTransaction()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateInsiderSentiment()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoDayLine()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateStockDayLine()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexDayLine()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateEPSSurprise()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateSECFilings()).Times(0);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateRTQuote()).Times(0);

		m_pMockFinnhubDataSource->GenerateInquiryMessage(120000);
	}

	TEST_F(CMockFinnhubDataSourceTest, TestInquiryFinnhub3) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();

		m_pMockFinnhubDataSource->SetInquiring(false);
		gl_pWorldMarket->SetSystemReady(true);
		InSequence Seq;
		EXPECT_CALL(*m_pMockFinnhubDataSource, GetTickCount()).Times(1)
		.WillOnce(Return(timePoint + 300000ms + 100000ms));
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateEconomicCalendar()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCountryList()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoExchange()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateMarketStatus()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateMarketHoliday()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanySymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexSymbol()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoSymbol()).Times(1);

		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyProfileConcise()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyNews()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCompanyBasicFinancial()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GeneratePeer()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateInsiderTransaction()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateInsiderSentiment()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateCryptoDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateStockDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateForexDayLine()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateEPSSurprise()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateSECFilings()).Times(1);
		EXPECT_CALL(*m_pMockFinnhubDataSource, GenerateRTQuote()).Times(0);

		m_pMockFinnhubDataSource->GenerateInquiryMessage(120000);

		const string str = gl_systemMessage.PopInformationMessage();
		EXPECT_STREQ(str.c_str(), _T("finnhub data inquiry finished"));
	}
}
