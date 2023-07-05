#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockTengxunRTDataSourcePtr m_pMockTengxunRTDataSource; // ��Ѷʵʱ���ݲɼ�
	class CMockTengxunRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
			m_pMockTengxunRTDataSource = make_shared<CMockTengxunRTDataSource>();

			// ��ֹ����ʵ������������ȡ���񣨷���ᵼ�·���ʵ���������룩
			m_pMockTengxunRTDataSource->DisableDataSourceImp();
			// ��ֹ����ʵ������������ȡ���񣨷���ᵼ�·���ʵ���������룩
			TengxunRTDataSource.DisableDataSourceImp();
		}

		void TearDown() override {
			// clearUp
			EXPECT_FALSE(m_pMockTengxunRTDataSource->HaveInquiry());
			m_pMockTengxunRTDataSource = nullptr;
			gl_pChinaMarket->SetSystemReady(true);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTengxunRTDataSource TengxunRTDataSource;
	};

	TEST_F(CMockTengxunRTDataSourceTest, TestGenerateInquiryMessage2) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_systemStatus.IsWebBusy());

		EXPECT_CALL(*m_pMockTengxunRTDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(1 + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(121100)) << "�����ȴ�";
		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(121600)) << "��������";
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestGenerateInquiryMessage3) {
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		EXPECT_FALSE(gl_systemStatus.IsWebBusy());

		EXPECT_CALL(*m_pMockTengxunRTDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(0))
		.WillOnce(Return(40000))
		.WillOnce(Return(500000));

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120000));
		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120100));
		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120600)) << "��������";
	}
}
