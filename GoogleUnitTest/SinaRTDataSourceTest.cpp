#include"pch.h"

#include"GeneralCheck.h"

#include"SinaRTDataSource.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductSinaRT.h"

using namespace testing;

namespace FireBirdTest {
	class CSinaRTDataSourceTest : public ::testing::Test {
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
			gl_pSinaRTDataSource->SetInquiring(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CSinaRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_TRUE(gl_pSinaRTDataSource->ParseData(pData));
	}

	TEST_F(CSinaRTDataSourceTest, TestInquireRTData1) {
		gl_pSinaRTDataSource->SetInquiring(true);
		EXPECT_FALSE(gl_pSinaRTDataSource->InquireRTData(0)) << "其他FinnhubInquiry正在进行";
	}

	TEST_F(CSinaRTDataSourceTest, TestInquireRTData2) {
		gl_pSinaRTDataSource->SetInquiring(false);
		EXPECT_TRUE(gl_pSinaRTDataSource->InquireRTData(0));

		EXPECT_TRUE(gl_pSinaRTDataSource->IsInquiring());
		EXPECT_EQ(gl_pSinaRTDataSource->GetInquiryQueueSize(), 1);
		auto pProduct = gl_pSinaRTDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductSinaRT"));
	}

	// 测试有优先级的队列存储临时实时数据。
	TEST_F(CSinaRTDataSourceTest, TestGetDataSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
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
