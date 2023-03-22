#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include "ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"

#include"MockTiingoDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockTiingoDataSourcePtr m_pTiingoDataSource;

	class CMockTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
			m_pTiingoDataSource = make_shared<CMockTiingoDataSource>();
		}

		void TearDown() override {
			// clearUp
			m_pTiingoDataSource = nullptr;
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockTiingoDataSourceTest, TestGenerateInquiryMessage1) {
		EXPECT_FALSE(m_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		m_pTiingoDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pTiingoDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(1 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + 100000));
		EXPECT_CALL(*m_pTiingoDataSource, InquireCompanySymbol()).Times(2)
		.WillRepeatedly(DoAll(Invoke([]() { m_pTiingoDataSource->SetInquiring(true); }), Return(true)));
		EXPECT_CALL(*m_pTiingoDataSource, InquireCryptoSymbol()).Times(2);
		EXPECT_CALL(*m_pTiingoDataSource, InquireDayLine()).Times(2);

		EXPECT_TRUE(m_pTiingoDataSource->GenerateInquiryMessage(120000)) << "网络报错，延后五分钟";
		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		m_pTiingoDataSource->SetInquiring(false);
		m_pTiingoDataSource->SetErrorCode(0);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "未过五分钟，继续等待";
		EXPECT_TRUE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "已过五分钟，申请数据";

		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
	}
}
