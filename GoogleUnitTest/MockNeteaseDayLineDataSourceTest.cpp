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
			gl_pMockNeteaseDayLineDataSource = make_shared<CMockNeteaseDayLineDataSource>();
			gl_pChinaMarket->CalculateTime();
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
		}

		void TearDown() override {
			// clearUp

			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			gl_pChinaMarket->SetSystemReady(true);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			for (int i = 0; i < gl_dataContainerChinaStock.Size(); i++) {
				gl_dataContainerChinaStock.GetStock(i)->SetUpdateDayLine(true);
			}
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			gl_pMockNeteaseDayLineDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};
}
