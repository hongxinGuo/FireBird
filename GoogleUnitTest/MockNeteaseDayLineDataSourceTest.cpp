#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseDayLineDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockNeteaseDayLineDataSourcePtr gl_pMockNeteaseDayLineDataSource;

	class CMockNeteaseDayLineDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
			gl_pMockNeteaseDayLineDataSource = make_shared<CMockNeteaseDayLineDataSource>();
			gl_pChinaMarket->CalculateTime();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		void TearDown() override {
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
			SCOPED_TRACE(""); GeneralCheck();
		}
	};
}
