#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunDayLineDataSource.h"
#include "ProductTengxunDayLine.h"

using namespace testing;

namespace FireBirdTest {
	CMockTengxunDayLineDataSourcePtr m_pMockTengxunDayLineDataSource; // ����������ʷ����

	class CMockTengxunDayLineDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
		}

		void SetUp(void) override {
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			m_pMockTengxunDayLineDataSource = make_shared<CMockTengxunDayLineDataSource>();
			gl_pChinaMarket->CalculateTime();
			m_pMockTengxunDayLineDataSource->ResetDownLoadingStockCode();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		void TearDown(void) override {
			// clearUp
			m_pMockTengxunDayLineDataSource = nullptr;
			GeneralCheck();

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

	protected:
	};

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGetWebData) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		m_pMockTengxunDayLineDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, StartReadingThread)
		.Times(1);
		m_pMockTengxunDayLineDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockTengxunDayLineDataSource->IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage1) {
		gl_pSinaRTDataSource->SetErrorCode(12002);
		gl_pNeteaseDayLineDataSource->SetErrorCode(0);
		EXPECT_TRUE(gl_systemStatus.IsWebBusy());
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(0);

		EXPECT_FALSE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "WebBusyʱ������������";
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage2) {
		gl_pSinaRTDataSource->SetErrorCode(0);
		gl_pNeteaseDayLineDataSource->SetErrorCode(12002);
		EXPECT_TRUE(gl_systemStatus.IsWebBusy());
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(0);

		EXPECT_FALSE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "WebBusyʱ������������";
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage3) {
		gl_pSinaRTDataSource->SetErrorCode(0);
		gl_pNeteaseDayLineDataSource->SetErrorCode(0);
		EXPECT_FALSE(gl_systemStatus.IsWebBusy());
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedUpdate());
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000); // dummy time and > 114500
		m_pMockTengxunDayLineDataSource->SetInquiring(true);
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(0);

		EXPECT_FALSE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "�в�ѯ����ʱ������������";
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage4) {
		gl_pSinaRTDataSource->SetErrorCode(0);
		gl_pNeteaseDayLineDataSource->SetErrorCode(0);
		EXPECT_FALSE(gl_systemStatus.IsWebBusy());
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_pChinaMarket->IsDayLineNeedUpdate());
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000); // dummy time and > 114500
		m_pMockTengxunDayLineDataSource->SetInquiring(false);
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(1);

		EXPECT_TRUE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "û�в�ѯ����ʱ����������";
	}
}
