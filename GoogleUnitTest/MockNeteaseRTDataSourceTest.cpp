#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseRTDataSource.h"
#include "ProductNeteaseRT.h"

using namespace testing;

namespace FireBirdTest {
	CMockNeteaseRTDataSourcePtr m_pMockNeteaseRTDataSource; // 网易实时数据采集
	class CMockNeteaseRTDataSourceTest : public ::testing::Test {
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
			m_pMockNeteaseRTDataSource = make_shared<CMockNeteaseRTDataSource>();
		}

		void TearDown() override {
			// clearUp
			m_pMockNeteaseRTDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockNeteaseRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_pMockNeteaseRTDataSource->GetInquiryFunction(), _T("http://api.money.126.net/data/feed/"));
		EXPECT_STREQ(m_pMockNeteaseRTDataSource->GetInquiryToken(), _T(""));
		EXPECT_EQ(m_pMockNeteaseRTDataSource->GetInquiringNumber(), 900) << _T("DEBUG模式下网易默认值");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestGenerateInquiryMessage) {
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(m_pMockNeteaseRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false); // 保证快速申请数据

		EXPECT_CALL(*m_pMockNeteaseRTDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(0))
		.WillOnce(Return(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(1 + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120000));
		EXPECT_FALSE(m_pMockNeteaseRTDataSource->IsInquiring());

		EXPECT_FALSE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120100)) << "时间未到，继续等待";
		EXPECT_FALSE(m_pMockNeteaseRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->GenerateInquiryMessage(120600)) << "申请数据";

		EXPECT_TRUE(m_pMockNeteaseRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->HaveInquiry());

		// 恢复原状
		gl_pChinaMarket->SetSystemReady(true);
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestInquireRTData) {
		m_pMockNeteaseRTDataSource->SetInquiring(false);

		m_pMockNeteaseRTDataSource->Inquire(1010);

		EXPECT_EQ(m_pMockNeteaseRTDataSource->InquiryQueueSize(), 1);
		const auto pProduct = m_pMockNeteaseRTDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseRT"));
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		const auto p = make_shared<CProductNeteaseRT>();
		p->SetInquiryFunction(_T("http://api.money.126.net/data/feed/"));
		m_pMockNeteaseRTDataSource->SetCurrentInquiry(p);
		m_pMockNeteaseRTDataSource->GenerateCurrentInquiryMessage();
		const CString str = m_pMockNeteaseRTDataSource->GetInquiringString();
		EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
	}
}
