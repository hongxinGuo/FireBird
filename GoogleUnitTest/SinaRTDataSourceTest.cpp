#include"pch.h"

#include"GeneralCheck.h"

#include"SinaRTDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductSinaRT.h"

#include"MockSinaRTWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
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

		void SetUp(void) override {
			s_pMockSinaRTWebInquiry->SetReadingWebData(false);

			gl_pSinaRTDataSource->SetWebInquiringPtr(s_pMockSinaRTWebInquiry.get());
		}

		void TearDown(void) override {
			// clearUp
			s_pMockSinaRTWebInquiry->SetReadingWebData(false);
			gl_pSinaRTDataSource->SetInquiring(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CSinaRTDataSourceTest, TestInquireRTData1) {
		gl_pSinaRTDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pSinaRTDataSource->InquireRTData(0)) << "����FinnhubInquiry���ڽ���";
	}

	TEST_F(CSinaRTDataSourceTest, TestInquireRTData2) {
		gl_pSinaRTDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pSinaRTDataSource->InquireRTData(0));

		EXPECT_TRUE(gl_pSinaRTDataSource->IsInquiring());
		EXPECT_EQ(gl_pSinaRTDataSource->GetInquiryQueueSize(), 1);
		auto pProduct = gl_pSinaRTDataSource->GetInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductSinaRT"));

	}

	// ���������ȼ��Ķ��д洢��ʱʵʱ���ݡ�
	TEST_F(CSinaRTDataSourceTest, TestGetDataSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pChinaMarket->PushSinaRT(pRTData);
		auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pChinaMarket->PushSinaRT(pRTData2);
		auto pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pChinaMarket->PushSinaRT(pRTData3);
		auto pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pChinaMarket->PushSinaRT(pRTData4);
		auto pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_pChinaMarket->PushSinaRT(pRTData5);  // �����pRTData2��ʱ����ͬ��Ӧ��λ��pRTData2֮��
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
		EXPECT_EQ(p2->GetBuy(), 2); // ��������ͬʱ�������Ӧ��λ�ں���
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}
