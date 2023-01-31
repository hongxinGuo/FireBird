#include"pch.h"

#include"GeneralCheck.h"

#include"NeteaseDayLineDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CNeteaseDayLineDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry, NotNull());
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(gl_pNeteaseDayLineDataSource->UpdateStatus());
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestInquire1) {
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000);
		gl_pNeteaseDayLineDataSource->SetInquiring(false);

		EXPECT_TRUE(gl_pNeteaseDayLineDataSource->Inquire(0));

		EXPECT_TRUE(gl_pNeteaseDayLineDataSource->IsInquiring());
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->GetInquiryQueueSize(), 1);
		auto pProduct = gl_pNeteaseDayLineDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseDayLine"));
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestInquire2) {
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000);
		gl_pNeteaseDayLineDataSource->SetInquiring(true);

		EXPECT_TRUE(gl_pNeteaseDayLineDataSource->Inquire(0));
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->GetInquiryQueueSize(), 0) << "没有申请数据";
	}
}
