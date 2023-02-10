#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunDayLineDataSource.h"
#include "ProductTengxunDayLine.h"

using namespace testing;

namespace FireBirdTest {
	CMockTengxunDayLineDataSourcePtr m_pMockTengxunDayLineDataSource; // 网易日线历史数据

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
		EXPECT_TRUE(m_pMockTengxunDayLineDataSource->IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}
}
