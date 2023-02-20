#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseDayLineDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockNeteaseDayLineDataSourcePtr gl_pMockNeteaseDayLineDataSource;

	class CMockNeteaseDayLineDataSourceTest : public ::testing::Test {
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
			gl_pMockNeteaseDayLineDataSource = make_shared<CMockNeteaseDayLineDataSource>();
			gl_pChinaMarket->CalculateTime();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		void TearDown(void) override {
			// clearUp

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(true);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			gl_pMockNeteaseDayLineDataSource = nullptr;
			GeneralCheck();
		}
	};

	TEST_F(CMockNeteaseDayLineDataSourceTest, TestGetWebData) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		gl_pMockNeteaseDayLineDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*gl_pMockNeteaseDayLineDataSource, StartReadingThread)
		.Times(1);
		gl_pMockNeteaseDayLineDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(gl_pMockNeteaseDayLineDataSource->IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}
}
