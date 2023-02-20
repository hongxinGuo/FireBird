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
			m_pMockTengxunRTDataSource = nullptr;
			gl_pChinaMarket->SetSystemReady(true);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}

	protected:
		CTengxunRTDataSource TengxunRTDataSource;
	};

	TEST_F(CMockTengxunRTDataSourceTest, TestGetWebData) {
		m_pMockTengxunRTDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockTengxunRTDataSource, StartReadingThread)
		.Times(1);
		m_pMockTengxunRTDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}
}
