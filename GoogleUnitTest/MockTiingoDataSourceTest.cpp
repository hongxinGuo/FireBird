#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include"MockTiingoDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockTiingoDataSourcePtr m_pTiingoDataSource;

	class CMockTiingoDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			m_pTiingoDataSource = make_shared<CMockTiingoDataSource>();
		}

		void TearDown() override {
			// clearUp
			m_pTiingoDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockTiingoDataSourceTest, TestGenerateInquiryMessage1) {
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(m_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		m_pTiingoDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pTiingoDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(1 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(300000 + 100000));
		EXPECT_CALL(*m_pTiingoDataSource, InquireCompanySymbol()).Times(1)
		.WillRepeatedly(DoAll(Invoke([p]() {
			m_pTiingoDataSource->SetInquiring(true);
			m_pTiingoDataSource->StoreInquiry(p);
		}), Return(true)));
		EXPECT_CALL(*m_pTiingoDataSource, InquireCryptoSymbol()).Times(1);
		EXPECT_CALL(*m_pTiingoDataSource, InquireDayLine()).Times(1);

		EXPECT_FALSE(m_pTiingoDataSource->GenerateInquiryMessage(120000)) << "网络报错，延后五分钟";
		EXPECT_FALSE(m_pTiingoDataSource->IsInquiring());
		EXPECT_FALSE(m_pTiingoDataSource->HaveInquiry());
		m_pTiingoDataSource->SetErrorCode(0);
		EXPECT_FALSE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "未过五分钟，继续等待";
		EXPECT_FALSE(m_pTiingoDataSource->IsInquiring());
		EXPECT_FALSE(m_pTiingoDataSource->HaveInquiry());
		EXPECT_TRUE(m_pTiingoDataSource->GenerateInquiryMessage(120500)) << "已过五分钟，申请数据";

		EXPECT_TRUE(m_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(m_pTiingoDataSource->HaveInquiry());
	}
}
