#include"pch.h"

#include"GeneralCheck.h"

#include"tengxunRTDataSourceImp.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include "ProductTengxunRT.h"

using namespace testing;

namespace FireBirdTest {
	class CTengxunRTDataSourceTest : public ::testing::Test {
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
		CTengxunRTDataSourceImp tengxunRTDataSource;
	};

	TEST_F(CTengxunRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(tengxunRTDataSource.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(tengxunRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(tengxunRTDataSource.GetInquiringNumber(), 900) << _T("腾讯默认值");
	}

	TEST_F(CTengxunRTDataSourceTest, TestInquireRTData1) {
		tengxunRTDataSource.SetInquiring(true);

		EXPECT_FALSE(tengxunRTDataSource.InquireRTData(0));

		EXPECT_FALSE(tengxunRTDataSource.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestInquireRTData2) {
		tengxunRTDataSource.SetInquiring(false);

		EXPECT_TRUE(tengxunRTDataSource.InquireRTData(0));

		EXPECT_TRUE(tengxunRTDataSource.IsInquiring());
		EXPECT_TRUE(tengxunRTDataSource.HaveInquiry());
		EXPECT_EQ(tengxunRTDataSource.GetInquiryQueueSize(), 1);
		tengxunRTDataSource.GetCurrentProduct();
		const auto pProduct = tengxunRTDataSource.GetCurrentInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductTengxunRT"));
		EXPECT_FALSE(tengxunRTDataSource.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestParseData) {
		const auto pData = make_shared<CWebData>();
		EXPECT_FALSE(tengxunRTDataSource.ParseData(pData)) << "腾讯实时数据无需解析";
	}

	TEST_F(CTengxunRTDataSourceTest, TestIsTengxunRTDataInValid) {
		const CWebDataPtr pWebDataReceived = make_shared<CWebData>();
		CString str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength());
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(tengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength());
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(tengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}

	TEST_F(CTengxunRTDataSourceTest, TestGetTengxunRTDataDuqueSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 0);
		const auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_pChinaMarket->PushTengxunRT(pRTData);
		const auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_pChinaMarket->PushTengxunRT(pRTData2);
		const auto pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_pChinaMarket->PushTengxunRT(pRTData3);
		const auto pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_pChinaMarket->PushTengxunRT(pRTData4);
		const auto pRTData5 = make_shared<CWebRTData>();
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
