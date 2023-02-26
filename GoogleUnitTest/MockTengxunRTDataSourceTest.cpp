#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockTengxunRTDataSourcePtr m_pMockTengxunRTDataSource; // 腾讯实时数据采集
	class CMockTengxunRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
			m_pMockTengxunRTDataSource = make_shared<CMockTengxunRTDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			EXPECT_FALSE(m_pMockTengxunRTDataSource->HaveInquiry());
			m_pMockTengxunRTDataSource = nullptr;
			gl_pChinaMarket->SetSystemReady(true);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);

			GeneralCheck();
		}

	protected:
		CTengxunRTDataSource TengxunRTDataSource;
	};

	TEST_F(CMockTengxunRTDataSourceTest, TestGenerateInquiryMessage1) {
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());

		gl_pSinaRTDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pMockTengxunRTDataSource, GetTickCount()).Times(1)
		.WillOnce(Return(0));

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120000)) << "Web Busy";

		// 恢复原状
		gl_pSinaRTDataSource->SetErrorCode(0);
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestGenerateInquiryMessage2) {
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_systemStatus.IsWebBusy());

		m_pMockTengxunRTDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pMockTengxunRTDataSource, GetTickCount()).Times(5)
		.WillOnce(Return(0))
		.WillOnce(Return(10000))
		.WillOnce(Return(30000 + 1 + 5 * gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(30000 + 1 + 10 * gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(400000));

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120000));
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120100)) << "Web Error, postponed 10 seconds";
		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiring());
		m_pMockTengxunRTDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());
		while (m_pMockTengxunRTDataSource->HaveInquiry()) m_pMockTengxunRTDataSource->GetCurrentProduct();
		m_pMockTengxunRTDataSource->SetErrorCode(0);

		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120600)) << "已过10秒且网络正常，申请数据";
		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());
		m_pMockTengxunRTDataSource->SetInquiring(false);
		while (m_pMockTengxunRTDataSource->HaveInquiry()) m_pMockTengxunRTDataSource->GetCurrentProduct();

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(121100)) << "继续等待";
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(121600)) << "申请数据";

		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiring());

		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());
		while (m_pMockTengxunRTDataSource->HaveInquiry()) m_pMockTengxunRTDataSource->GetCurrentProduct();
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestGenerateInquiryMessage3) {
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_systemStatus.IsWebBusy());

		EXPECT_CALL(*m_pMockTengxunRTDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(0))
		.WillOnce(Return(40000))
		.WillOnce(Return(500000));

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120000));
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120100));
		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiring());
		m_pMockTengxunRTDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());
		while (m_pMockTengxunRTDataSource->HaveInquiry()) m_pMockTengxunRTDataSource->GetCurrentProduct();

		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120600)) << "申请数据";

		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiring());

		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());
		while (m_pMockTengxunRTDataSource->HaveInquiry()) m_pMockTengxunRTDataSource->GetCurrentProduct();
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestProcessInquiryMessage) {
		m_pMockTengxunRTDataSource->SetInquiring(true);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockTengxunRTDataSource, StartReadingThread)
		.Times(1);

		m_pMockTengxunRTDataSource->ProcessInquiryMessage();
	}
}
