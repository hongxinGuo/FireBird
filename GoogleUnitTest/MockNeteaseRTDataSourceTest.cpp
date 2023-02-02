#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
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
		}

		void TearDown(void) override {
			// clearUp
			gl_pChinaMarket->SetSystemReady(false);
			GeneralCheck();
		}

		CMockNeteaseRTDataSource m_MockNeteaseRTDataSource; // ����ʵʱ���ݲɼ�
	};

	TEST_F(CMockNeteaseRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_MockNeteaseRTDataSource.GetInquiryFunction(), _T("http://api.money.126.net/data/feed/"));
		EXPECT_STREQ(m_MockNeteaseRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(m_MockNeteaseRTDataSource.GetInquiringNumber(), 900) << _T("DEBUGģʽ������Ĭ��ֵ");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(m_MockNeteaseRTDataSource.UpdateStatus()) << "����ʵʱ�����������״̬��ֱ�ӷ���";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_TRUE(m_MockNeteaseRTDataSource.ParseData(pData)) << "����ʵʱ�����������";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestInquireRTData) {
		m_MockNeteaseRTDataSource.SetInquiring(true);
		EXPECT_FALSE(m_MockNeteaseRTDataSource.InquireRTData(1010));

		m_MockNeteaseRTDataSource.SetInquiring(false);
		EXPECT_TRUE(m_MockNeteaseRTDataSource.InquireRTData(1010));
		EXPECT_EQ(m_MockNeteaseRTDataSource.GetInquiryQueueSize(), 1);
		auto pProduct = m_MockNeteaseRTDataSource.GetInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseRT"));
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestGetWebData) {
		m_MockNeteaseRTDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockNeteaseRTDataSource.GetWebData());
		m_MockNeteaseRTDataSource.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockNeteaseRTDataSource, StartReadingThread)
			.Times(1);
		m_MockNeteaseRTDataSource.GetWebData();
		EXPECT_TRUE(m_MockNeteaseRTDataSource.IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_MockNeteaseRTDataSource.PrepareNextInquiringString());
		const CString str = m_MockNeteaseRTDataSource.GetInquiringString();
		EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
	}
}
