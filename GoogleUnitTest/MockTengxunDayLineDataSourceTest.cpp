#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunDayLineDataSource.h"
#include "NeteaseRTDataSource.h"
#include "SinaRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockTengxunDayLineDataSourcePtr m_pMockTengxunDayLineDataSource; // 网易日线历史数据

	class CMockTengxunDayLineDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(gl_containerChinaStock.GetDayLineNeedUpdateNumber(), gl_containerChinaStock.Size());
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_containerChinaStock.GetDayLineNeedUpdateNumber(), gl_containerChinaStock.Size());
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			m_pMockTengxunDayLineDataSource = make_shared<CMockTengxunDayLineDataSource>();
			gl_pChinaMarket->CalculateTime();
			m_pMockTengxunDayLineDataSource->ResetDownLoadingStockCode();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_containerChinaStock.GetDayLineNeedUpdateNumber(), gl_containerChinaStock.Size());
		}

		void TearDown() override {
			// clearUp
			m_pMockTengxunDayLineDataSource = nullptr;

			EXPECT_EQ(gl_containerChinaStock.GetDayLineNeedUpdateNumber(), gl_containerChinaStock.Size());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(true);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_containerChinaStock.Size(); i++) {
				gl_containerChinaStock.GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_containerChinaStock.GetDayLineNeedUpdateNumber(), gl_containerChinaStock.Size());

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage1) {
		gl_pSinaRTDataSource->SetErrorCode(12002);
		gl_pNeteaseRTDataSource->SetErrorCode(0);
		EXPECT_TRUE(gl_systemConfiguration.IsWebBusy());
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(0);

		EXPECT_FALSE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "WebBusy时，不申请数据";

		gl_pSinaRTDataSource->SetErrorCode(0);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage2) {
		gl_pSinaRTDataSource->SetErrorCode(0);
		gl_pNeteaseRTDataSource->SetErrorCode(12002);
		EXPECT_TRUE(gl_systemConfiguration.IsWebBusy());
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(0);

		EXPECT_FALSE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "WebBusy时，不申请数据";

		gl_pNeteaseRTDataSource->SetErrorCode(0);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage3) {
		gl_pSinaRTDataSource->SetErrorCode(0);
		gl_pNeteaseRTDataSource->SetErrorCode(0);
		EXPECT_FALSE(gl_systemConfiguration.IsWebBusy());
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_containerChinaStock.IsDayLineNeedUpdate());
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000); // dummy time and > 114500
		m_pMockTengxunDayLineDataSource->SetInquiring(true);
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(0);

		EXPECT_FALSE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "有查询申请时，不申请数据";
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage4) {
		gl_pSinaRTDataSource->SetErrorCode(0);
		gl_pNeteaseRTDataSource->SetErrorCode(0);
		EXPECT_FALSE(gl_systemConfiguration.IsWebBusy());
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_containerChinaStock.IsDayLineNeedUpdate());
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000); // dummy time and > 114500
		m_pMockTengxunDayLineDataSource->SetInquiring(false);
		EXPECT_CALL(*m_pMockTengxunDayLineDataSource, InquireDayLine).Times(1);

		EXPECT_TRUE(m_pMockTengxunDayLineDataSource->GenerateInquiryMessage(120000)) << "没有查询申请时，申请数据";
	}
}
