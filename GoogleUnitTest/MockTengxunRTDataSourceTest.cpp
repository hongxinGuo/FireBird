#include"pch.h"

//#include"gtest/gtest.h"

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
			gl_dataContainerChinaStock.SetTengxunRTDataInquiringIndex(0);
			m_pMockTengxunRTDataSource = make_shared<CMockTengxunRTDataSource>();
		}

		void TearDown() override {
			// clearUp
			m_pMockTengxunRTDataSource = nullptr;
			gl_pChinaMarket->SetSystemReady(true);
			gl_dataContainerChinaStock.SetTengxunRTDataInquiringIndex(0);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTengxunRTDataSource TengxunRTDataSource;
	};

	TEST_F(CMockTengxunRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(TengxunRTDataSource.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(TengxunRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(TengxunRTDataSource.GetInquiringNumber(), 900) << _T("��ѶĬ��ֵ");
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestGenerateInquiryMessage) {
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false); // ��֤������������

		m_pMockTengxunRTDataSource->SetWebError(true);
		EXPECT_CALL(*m_pMockTengxunRTDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(0))
		.WillOnce(Return(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(1 + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120000));

		EXPECT_FALSE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120100)) << "�����ȴ�";
		EXPECT_FALSE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_FALSE(m_pMockTengxunRTDataSource->HaveInquiry());

		EXPECT_TRUE(m_pMockTengxunRTDataSource->GenerateInquiryMessage(120600)) << "��������";

		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockTengxunRTDataSource->HaveInquiry());

		// �ָ�ԭ״
		gl_pChinaMarket->SetSystemReady(true);
	}
}
