#include"pch.h"

#include"GeneralCheck.h"

#include"TengxunRTDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductTengxunRT.h"

#include"MockTengxunRTWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	static CMockTengxunRTWebInquiryPtr s_pMockTengxunRTWebInquiry;

	class CTengxunRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pTengxunRTWebInquiry, NotNull());
			s_pMockTengxunRTWebInquiry = static_pointer_cast<CMockTengxunRTWebInquiry>(gl_pTengxunRTWebInquiry);
		}

		static void TearDownTestSuite(void) {
			s_pMockTengxunRTWebInquiry = nullptr;
			GeneralCheck();
		}

		void SetUp(void) override {
			s_pMockTengxunRTWebInquiry->SetInquiringWebData(false);

			gl_pTengxunRTDataSource->SetWebInquiringPtr(s_pMockTengxunRTWebInquiry.get());
		}

		void TearDown(void) override {
			// clearUp
			s_pMockTengxunRTWebInquiry->SetInquiringWebData(false);
			gl_pTengxunRTDataSource->SetInquiring(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CTengxunRTDataSourceTest, TestInquireRTData1) {
		gl_pTengxunRTDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pTengxunRTDataSource->InquireRTData(0)) << "其他FinnhubInquiry正在进行";
	}

	TEST_F(CTengxunRTDataSourceTest, TestInquireRTData2) {
		gl_pTengxunRTDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pTengxunRTDataSource->InquireRTData(0));

		EXPECT_TRUE(gl_pTengxunRTDataSource->IsInquiring());
		EXPECT_EQ(gl_pTengxunRTDataSource->GetInquiryQueueSize(), 1);
		auto pProduct = gl_pTengxunRTDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductTengxunRT"));

	}

	TEST_F(CTengxunRTDataSourceTest, TestGetTengxunRTDataDuqueSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 0);
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pChinaMarket->PushTengxunRT(pRTData);
		auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pChinaMarket->PushTengxunRT(pRTData2);
		auto pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pChinaMarket->PushTengxunRT(pRTData3);
		auto pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pChinaMarket->PushTengxunRT(pRTData4);
		auto pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_pChinaMarket->PushTengxunRT(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 5);
		CWebRTDataPtr p2 = gl_pChinaMarket->PopTengxunRT();
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_pChinaMarket->PopTengxunRT();
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_pChinaMarket->PopTengxunRT();
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_pChinaMarket->PopTengxunRT();
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_pChinaMarket->PopTengxunRT();
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}
