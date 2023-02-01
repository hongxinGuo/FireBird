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
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CNeteaseDayLineDataSourceTest, TestUpdateStatus) {
		EXPECT_TRUE(gl_pNeteaseDayLineDataSource->UpdateStatus());
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestPrepareNextInquiringStr) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		CString str;
		gl_pChinaMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (gl_pNeteaseDayLineDataSource->PrepareNextInquiringString()) {
				str = gl_pNeteaseDayLineDataSource->GetInquiringString();
				EXPECT_STREQ(str.Left(54), _T("http://quotes.money.163.com/service/chddata.html?code="));
			}
			else
				EXPECT_EQ(str.GetLength(), 0);
		}
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_LT(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		// 目前将索引移入函数内，作为静态变量存在，故而无法知道确切位置了。
		//EXPECT_FALSE(gl_pChinaMarket->GetStock(0)->IsDayLineNeedUpdate());

		// 恢复原态
		for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
			gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
		}
	}

	TEST_F(CNeteaseDayLineDataSourceTest, TestInquire11) {
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(120000);
		gl_pNeteaseDayLineDataSource->SetInquiring(false);

		EXPECT_TRUE(gl_pNeteaseDayLineDataSource->Inquire(0));

		EXPECT_TRUE(gl_pNeteaseDayLineDataSource->IsInquiring());
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->GetInquiryQueueSize(), 1);
		auto pProduct = gl_pNeteaseDayLineDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductNeteaseDayLine"));

		gl_pNeteaseDayLineDataSource->SetInquiring(false);
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
