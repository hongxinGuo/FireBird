#include"pch.h"

#include"GeneralCheck.h"

#include"SinaRTDataSourceImp.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductSinaRT.h"

using namespace testing;

namespace FireBirdTest {
	class CSinaRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CSinaRTDataSourceImp sinaRTDataSourceImp;
	};

	TEST_F(CSinaRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_FALSE(sinaRTDataSourceImp.ParseData(pData)) << "�������";
	}

	TEST_F(CSinaRTDataSourceTest, TestInquireRTData2) {
		sinaRTDataSourceImp.SetInquiring(false);

		sinaRTDataSourceImp.InquireRTData(0);

		EXPECT_TRUE(sinaRTDataSourceImp.IsInquiring());
		EXPECT_EQ(sinaRTDataSourceImp.GetInquiryQueueSize(), 1);
		const auto pProduct = sinaRTDataSourceImp.GetCurrentProduct();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductSinaRT"));
	}

	// ���������ȼ��Ķ��д洢��ʱʵʱ���ݡ�
	TEST_F(CSinaRTDataSourceTest, TestGetDataSize) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);
		const auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pChinaMarket->PushSinaRT(pRTData);
		const auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pChinaMarket->PushSinaRT(pRTData2);
		const auto pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pChinaMarket->PushSinaRT(pRTData3);
		const auto pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pChinaMarket->PushSinaRT(pRTData4);
		const auto pRTData5 = make_shared<CWebRTData>();
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
