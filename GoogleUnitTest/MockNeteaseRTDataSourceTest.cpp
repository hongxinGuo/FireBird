#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockNeteaseRTDataSourcePtr m_pMockNeteaseRTDataSource; // ����ʵʱ���ݲɼ�
	class CMockNeteaseRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			m_pMockNeteaseRTDataSource = make_shared<CMockNeteaseRTDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pMockNeteaseRTDataSource = nullptr;
			gl_pChinaMarket->SetSystemReady(false);
			GeneralCheck();
		}
	};

	TEST_F(CMockNeteaseRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_pMockNeteaseRTDataSource->GetInquiryFunction(), _T("http://api.money.126.net/data/feed/"));
		EXPECT_STREQ(m_pMockNeteaseRTDataSource->GetInquiryToken(), _T(""));
		EXPECT_EQ(m_pMockNeteaseRTDataSource->GetInquiringNumber(), 900) << _T("DEBUGģʽ������Ĭ��ֵ");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->UpdateStatus()) << "����ʵʱ�����������״̬��ֱ�ӷ���";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->ParseData(pData)) << "����ʵʱ�����������";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestInquireRTData) {
		m_pMockNeteaseRTDataSource->SetInquiring(true);
		EXPECT_FALSE(m_pMockNeteaseRTDataSource->InquireRTData(1010));

		m_pMockNeteaseRTDataSource->SetInquiring(false);
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->InquireRTData(1010));
		EXPECT_EQ(m_pMockNeteaseRTDataSource->GetInquiryQueueSize(), 1);
		auto pProduct = m_pMockNeteaseRTDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseRT"));
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestGetWebData) {
		m_pMockNeteaseRTDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockNeteaseRTDataSource, StartReadingThread)
		.Times(1);
		m_pMockNeteaseRTDataSource->GetWebData();
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_pMockNeteaseRTDataSource->PrepareNextInquiringString());
		const CString str = m_pMockNeteaseRTDataSource->GetInquiringString();
		EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
	}
}
