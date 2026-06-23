#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunDayLineDataSource.h"
#include "SinaRTDataSource.h"

using namespace testing;

namespace {
	CMockTengxunDayLineDataSourcePtr s_pMockTengxunDayLineDataSource; // 网易日线历史数据
}

namespace FireBirdTest {
	class CMockTengxunDayLineDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			s_pMockTengxunDayLineDataSource = make_shared<CMockTengxunDayLineDataSource>();
			gl_pChinaMarket->CalculateTime();
			s_pMockTengxunDayLineDataSource->ResetDownLoadingStockCode();
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
		}

		void TearDown() override {
			// clearUp
			s_pMockTengxunDayLineDataSource = nullptr;

			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			gl_pChinaMarket->SetSystemReady(true);
			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				gl_dataContainerChinaStock.GetStock(i)->SetUpdateDayLine(true);
			}
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage1) {
		gl_pSinaRTDataSource->SetWebError(true);
		EXPECT_TRUE(gl_systemConfiguration.IsWebBusy());
		EXPECT_CALL(*s_pMockTengxunDayLineDataSource, Inquire).Times(0);
		EXPECT_CALL(*s_pMockTengxunDayLineDataSource, GetTickCount()).Times(1);

		EXPECT_FALSE(s_pMockTengxunDayLineDataSource->GenerateInquiryMessage(toLocalTime(120000))) << "WebBusy时，不申请数据";

		gl_pSinaRTDataSource->SetWebError(false);
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage2) {
		gl_pSinaRTDataSource->SetWebError(false);
		EXPECT_CALL(*s_pMockTengxunDayLineDataSource, Inquire).Times(0);
		EXPECT_CALL(*s_pMockTengxunDayLineDataSource, GetTickCount()).Times(1);

		EXPECT_FALSE(s_pMockTengxunDayLineDataSource->GenerateInquiryMessage(toLocalTime(120000))) << "WebBusy时，不申请数据";
	}

	TEST_F(CMockTengxunDayLineDataSourceTest, TestGenerateInquiryMessage3) {
		gl_pSinaRTDataSource->SetWebError(false);
		EXPECT_FALSE(gl_systemConfiguration.IsWebBusy());
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(gl_dataContainerChinaStock.IsUpdateDayLine());
		gl_pChinaMarket->TEST_SetMarketTime(toLocalTime(120000)); // dummy time and > 114500
		s_pMockTengxunDayLineDataSource->SetInquiring(true);
		EXPECT_CALL(*s_pMockTengxunDayLineDataSource, Inquire).Times(0);
		EXPECT_CALL(*s_pMockTengxunDayLineDataSource, GetTickCount()).Times(1);

		EXPECT_FALSE(s_pMockTengxunDayLineDataSource->GenerateInquiryMessage(toLocalTime(120000) )) << "有查询申请时，不申请数据";
	}
}
