#include"pch.h"

#include"GeneralCheck.h"

#include"NeteaseDayLineDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductNeteaseDayLine.h"

#include"MockNeteaseDayLineWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	static CMockNeteaseDayLineWebInquiryPtr s_pMockNeteaseDayLineWebInquiry;

	class CNeteaseDayLineDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pNeteaseDayLineWebInquiry, NotNull());
			s_pMockNeteaseDayLineWebInquiry = static_pointer_cast<CMockNeteaseDayLineWebInquiry>(gl_pNeteaseDayLineWebInquiry);
			gl_pNeteaseDayLineDataSource->SetWebInquiringPtr(s_pMockNeteaseDayLineWebInquiry.get());
		}

		static void TearDownTestSuite(void) {
			s_pMockNeteaseDayLineWebInquiry = nullptr;
			GeneralCheck();
		}

		void SetUp(void) override {
			s_pMockNeteaseDayLineWebInquiry->SetReadingWebData(false);
		}

		void TearDown(void) override {
			// clearUp
			s_pMockNeteaseDayLineWebInquiry->SetReadingWebData(false);

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
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->GetInquiryQueueSize(), 0);
	}
}
