#include"pch.h"

#include"GeneralCheck.h"

#include"NeteaseRTDataSource.h"
#include"WorldMarket.h"

#include "ProductNeteaseRT.h"
#include"NeteaseRTDataSource.h"

#include"MockNeteaseRTWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
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

		virtual void SetUp(void) override {
			s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);

			gl_pNeteaseRTDataSource->SetWebInquiringPtr(s_pMockNeteaseRTWebInquiry.get());
		}

		virtual void TearDown(void) override {
			// clearu
			s_pMockNeteaseRTWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST(RTDataContainerTest, TestGetNeteaseRTDataDuqueSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pNeteaseRTDataSource->DataSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pNeteaseRTDataSource->PushData(pRTData);
		CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pNeteaseRTDataSource->PushData(pRTData2);
		CWebRTDataPtr pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pNeteaseRTDataSource->PushData(pRTData3);
		CWebRTDataPtr pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pNeteaseRTDataSource->PushData(pRTData4);
		CWebRTDataPtr pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_pNeteaseRTDataSource->PushData(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_pNeteaseRTDataSource->DataSize(), 5);
		CWebRTDataPtr p2 = gl_pNeteaseRTDataSource->PopData();
		EXPECT_EQ(gl_pNeteaseRTDataSource->DataSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_pNeteaseRTDataSource->PopData();
		EXPECT_EQ(gl_pNeteaseRTDataSource->DataSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_pNeteaseRTDataSource->PopData();
		EXPECT_EQ(gl_pNeteaseRTDataSource->DataSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_pNeteaseRTDataSource->PopData();
		EXPECT_EQ(gl_pNeteaseRTDataSource->DataSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_pNeteaseRTDataSource->PopData();
		EXPECT_EQ(gl_pNeteaseRTDataSource->DataSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}