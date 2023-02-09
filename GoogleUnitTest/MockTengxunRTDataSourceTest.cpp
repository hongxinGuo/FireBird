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
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}

	protected:
		CTengxunRTDataSource TengxunRTDataSource;
	};

	TEST_F(CMockTengxunRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(TengxunRTDataSource.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(TengxunRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(TengxunRTDataSource.GetInquiringNumber(), 900) << _T("��ѶĬ��ֵ");
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(TengxunRTDataSource.UpdateStatus()) << "����ʵʱ�����������״̬��ֱ�ӷ���";
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_TRUE(TengxunRTDataSource.ParseData(pData)) << "����ʵʱ�����������";
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestGetWebData) {
		m_pMockTengxunRTDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockTengxunRTDataSource, StartReadingThread)
		.Times(1);
		m_pMockTengxunRTDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockTengxunRTDataSource->IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestIsTengxunRTDataInValid) {
		const CWebDataPtr pWebDataReceived = make_shared<CWebData>();
		CString str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(TengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(TengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}
}
