#include"pch.h"

#include"GeneralCheck.h"

#include"TengxunRTDataSource.h"
#include"WorldMarket.h"

#include "ProductTengxunRT.h"
#include"TengxunRTDataSource.h"

#include"MockTengxunRTWebInquiry.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
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
			// clearu
			s_pMockTengxunRTWebInquiry->SetReadingWebData(false);

			GeneralCheck();
		}
	protected:
	};

	TEST(RTDataContainerTest, TestGetTengxunRTDataDuqueSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pTengxunRTDataSource->DataSize(), 0);
		CWebRTDataPtr pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pTengxunRTDataSource->PushData(pRTData);
		CWebRTDataPtr pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pTengxunRTDataSource->PushData(pRTData2);
		CWebRTDataPtr pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pTengxunRTDataSource->PushData(pRTData3);
		CWebRTDataPtr pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pTengxunRTDataSource->PushData(pRTData4);
		CWebRTDataPtr pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_pTengxunRTDataSource->PushData(pRTData5);  // �����pRTData2��ʱ����ͬ��Ӧ��λ��pRTData2֮��
		EXPECT_EQ(gl_pTengxunRTDataSource->DataSize(), 5);
		CWebRTDataPtr p2 = gl_pTengxunRTDataSource->PopData();
		EXPECT_EQ(gl_pTengxunRTDataSource->DataSize(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_pTengxunRTDataSource->PopData();
		EXPECT_EQ(gl_pTengxunRTDataSource->DataSize(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_pTengxunRTDataSource->PopData();
		EXPECT_EQ(gl_pTengxunRTDataSource->DataSize(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_pTengxunRTDataSource->PopData();
		EXPECT_EQ(gl_pTengxunRTDataSource->DataSize(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_pTengxunRTDataSource->PopData();
		EXPECT_EQ(gl_pTengxunRTDataSource->DataSize(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // ��������ͬʱ�������Ӧ��λ�ں���
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}