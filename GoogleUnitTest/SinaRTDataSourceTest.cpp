#include"pch.h"

#include"GeneralCheck.h"

#include"SinaRTDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductSinaRT.h"

#include"MockSinaRTWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	static CMockSinaRTWebInquiryPtr s_pMockSinaRTWebInquiry;

	class CSinaRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pSinaRTWebInquiry, NotNull());
			s_pMockSinaRTWebInquiry = static_pointer_cast<CMockSinaRTWebInquiry>(gl_pSinaRTWebInquiry);
		}

		static void TearDownTestSuite(void) {
			s_pMockSinaRTWebInquiry = nullptr;
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			s_pMockSinaRTWebInquiry->SetReadingWebData(false);

			gl_pSinaRTDataSource->SetWebInquiringPtr(s_pMockSinaRTWebInquiry.get());
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockSinaRTWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	// 测试有优先级的队列存储临时实时数据。
	TEST(CSinaRTDataSourceTest, TestGetDataSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pChinaMarket->PushSinaRT(pRTData);
		CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pChinaMarket->PushSinaRT(pRTData2);
		CWebRTDataPtr pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pChinaMarket->PushSinaRT(pRTData3);
		CWebRTDataPtr pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pChinaMarket->PushSinaRT(pRTData4);
		CWebRTDataPtr pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_pChinaMarket->PushSinaRT(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 5);
		CWebRTDataPtr p2 = gl_pChinaMarket->PopSinaRT();
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_pChinaMarket->PopSinaRT();
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_pChinaMarket->PopSinaRT();
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_pChinaMarket->PopSinaRT();
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_pChinaMarket->PopSinaRT();
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}