#include"pch.h"

#include"GeneralCheck.h"

#include"TengxunRTDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductTengxunRT.h"

#include"MockTengxunRTWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

		virtual void SetUp(void) override {
			s_pMockTengxunRTWebInquiry->SetReadingWebData(false);

			gl_pTengxunRTDataSource->SetWebInquiringPtr(s_pMockTengxunRTWebInquiry.get());
		}

		virtual void TearDown(void) override {
			// clearUp
			s_pMockTengxunRTWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST(RTDataContainerTest, TestGetTengxunRTDataDuqueSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pChinaMarket->PushTengxunRT(pRTData);
		CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pChinaMarket->PushTengxunRT(pRTData2);
		CWebRTDataPtr pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pChinaMarket->PushTengxunRT(pRTData3);
		CWebRTDataPtr pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pChinaMarket->PushTengxunRT(pRTData4);
		CWebRTDataPtr pRTData5 = make_shared<CWebRTData>();
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