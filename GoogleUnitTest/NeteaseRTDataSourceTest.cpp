#include"pch.h"

#include"GeneralCheck.h"

#include"NeteaseRTDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductNeteaseRT.h"

#include"MockNeteaseRTWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	static CMockNeteaseRTWebInquiryPtr s_pMockNeteaseRTWebInquiry;

	class CNeteaseRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pNeteaseRTWebInquiry, NotNull());
			s_pMockNeteaseRTWebInquiry = static_pointer_cast<CMockNeteaseRTWebInquiry>(gl_pNeteaseRTWebInquiry);
		}

		static void TearDownTestSuite(void) {
			s_pMockNeteaseRTWebInquiry = nullptr;
			GeneralCheck();
		}

		void SetUp(void) override {
			s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);

			gl_pNeteaseRTDataSource->SetWebInquiringPtr(s_pMockNeteaseRTWebInquiry.get());
		}

		void TearDown(void) override {
			// clearUp
			s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}

	protected:
	};

	TEST(RTDataContainerTest, TestGetNeteaseRTDataDuqueSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 0);
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pChinaMarket->PushNeteaseRT(pRTData);
		auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pChinaMarket->PushNeteaseRT(pRTData2);
		auto pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pChinaMarket->PushNeteaseRT(pRTData3);
		auto pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pChinaMarket->PushNeteaseRT(pRTData4);
		auto pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_pChinaMarket->PushNeteaseRT(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 5);
		CWebRTDataPtr p2 = gl_pChinaMarket->PopNeteaseRT();
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_pChinaMarket->PopNeteaseRT();
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_pChinaMarket->PopNeteaseRT();
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_pChinaMarket->PopNeteaseRT();
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_pChinaMarket->PopNeteaseRT();
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}
