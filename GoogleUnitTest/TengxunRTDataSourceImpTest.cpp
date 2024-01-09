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
		CTengxunRTDataSourceImp tengxunRTDataSourceImp;
	};

	TEST_F(CTengxunRTDataSourceTest, TestInitialize2) {
		EXPECT_STREQ(tengxunRTDataSourceImp.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(tengxunRTDataSourceImp.GetInquiryToken(), _T(""));
		EXPECT_EQ(tengxunRTDataSourceImp.GetInquiringNumber(), 900) << _T("腾讯默认值");
	}

	TEST_F(CTengxunRTDataSourceTest, TestGenerateInquiryMessage1) {
		tengxunRTDataSourceImp.SetInquiring(true);

		tengxunRTDataSourceImp.GenerateInquiryMessage(0);

		EXPECT_FALSE(tengxunRTDataSourceImp.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestGenerateInquiryMessage2) {
		tengxunRTDataSourceImp.SetInquiring(false);

		tengxunRTDataSourceImp.GenerateInquiryMessage(0);

		EXPECT_TRUE(tengxunRTDataSourceImp.IsInquiring());
		EXPECT_TRUE(tengxunRTDataSourceImp.HaveInquiry());
		EXPECT_EQ(tengxunRTDataSourceImp.InquiryQueueSize(), 1);
		tengxunRTDataSourceImp.GetCurrentProduct();
		const auto pProduct = tengxunRTDataSourceImp.GetCurrentInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductTengxunRT"));
		EXPECT_FALSE(tengxunRTDataSourceImp.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestIsTengxunRTDataInValid) {
		const CWebDataPtr pWebDataReceived = make_shared<CWebData>();
		CString str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->Resize(str.GetLength());
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(tengxunRTDataSourceImp.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->Resize(str.GetLength());
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(tengxunRTDataSourceImp.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}

	TEST_F(CTengxunRTDataSourceTest, TestGetTengxunRTDataDuqueSize) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_EQ(gl_qTengxunRT.Size(), 0);
		const auto pRTData = make_shared<CWebRTData>();
		pRTData->SetTransactionTime(100100100);
		gl_qTengxunRT.PushData(pRTData);
		const auto pRTData2 = make_shared<CWebRTData>();
		pRTData2->SetTransactionTime(200200200);
		pRTData2->SetBuy(1);
		gl_qTengxunRT.PushData(pRTData2);
		const auto pRTData3 = make_shared<CWebRTData>();
		pRTData3->SetTransactionTime(200200);
		gl_qTengxunRT.PushData(pRTData3);
		const auto pRTData4 = make_shared<CWebRTData>();
		pRTData4->SetTransactionTime(200);
		gl_qTengxunRT.PushData(pRTData4);
		const auto pRTData5 = make_shared<CWebRTData>();
		pRTData5->SetTransactionTime(200200200);
		pRTData5->SetBuy(2);
		gl_qTengxunRT.PushData(pRTData5);  // 这个与pRTData2的时间相同，应该位于pRTData2之后
		EXPECT_EQ(gl_qTengxunRT.Size(), 5);
		CWebRTDataPtr p2 = gl_qTengxunRT.PopData();
		EXPECT_EQ(gl_qTengxunRT.Size(), 4);
		EXPECT_EQ(p2->GetTransactionTime(), 200);
		p2 = gl_qTengxunRT.PopData();
		EXPECT_EQ(gl_qTengxunRT.Size(), 3);
		EXPECT_EQ(p2->GetTransactionTime(), 200200);
		p2 = gl_qTengxunRT.PopData();
		EXPECT_EQ(gl_qTengxunRT.Size(), 2);
		EXPECT_EQ(p2->GetTransactionTime(), 100100100);
		p2 = gl_qTengxunRT.PopData();
		EXPECT_EQ(gl_qTengxunRT.Size(), 1);
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
		EXPECT_EQ(p2->GetBuy(), 1);
		p2 = gl_qTengxunRT.PopData();
		EXPECT_EQ(gl_qTengxunRT.Size(), 0);
		EXPECT_EQ(p2->GetBuy(), 2); // 后放入的相同时间的数据应该位于后面
		EXPECT_EQ(p2->GetTransactionTime(), 200200200);
	}
}
