#include"pch.h"

#include"WorldMarket.h"

#include"MockQuandlDataSource.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	static bool m_fSystemStatus;

	class CMockQuandlDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			m_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Quandl�������ȴ�ϵͳ��ʼ����ſ�ʹ�á�
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			gl_pWorldMarket->SetSystemReady(m_fSystemStatus);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			gl_pWorldMarket->CalculateTime();
		}

		void TearDown(void) override {
			// clearUp
			gl_pWorldMarket->SetResetMarket(true);
			GeneralCheck();
		}

		CMockQuandlDataSource m_MockQuandlDataSource; // ����������ʷ����
	};

	TEST_F(CMockQuandlDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_MockQuandlDataSource.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(m_MockQuandlDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CMockQuandlDataSourceTest, TestGetWebData) {
		m_MockQuandlDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockQuandlDataSource.GetWebData());
		m_MockQuandlDataSource.SetInquiringWebData(false);
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockQuandlDataSource, StartReadingThread)
			.Times(1);
		m_MockQuandlDataSource.GetWebData();
		EXPECT_TRUE(m_MockQuandlDataSource.IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	TEST_F(CMockQuandlDataSourceTest, TestPrepareNextInquiringStr) {
		CString str;
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (m_MockQuandlDataSource.PrepareNextInquiringString()) {
				str = m_MockQuandlDataSource.GetInquiringString();
				EXPECT_STREQ(str, _T("&api_key=")) << "Ĭ��״̬�£����ַ�����Ϊ�գ��ʶ�ֻʣ��&api_key=";
			}
			else
				EXPECT_EQ(str.GetLength(), 0);
		}
		gl_pWorldMarket->SetSystemReady(false);
	}
}
