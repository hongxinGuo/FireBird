#include"pch.h"

#include"GeneralCheck.h"

#include"tengxunRTDataSource.h"
#include"ChinaMarket.h"

#include"TestWebData.h"

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
		CTengxunRTDataSource tengxunRTDataSource;
	};

	TEST_F(CTengxunRTDataSourceTest, TestInitialize2) {
		EXPECT_STREQ(tengxunRTDataSource.GetInquiryFunction().c_str(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(tengxunRTDataSource.GetInquiryToken().c_str(), _T(""));
		EXPECT_EQ(tengxunRTDataSource.GetInquiringNumber(), 900) << _T("ÌÚÑ¶Ä¬ÈÏÖµ");
	}

	TEST_F(CTengxunRTDataSourceTest, TestGenerateInquiryMessage1) {
		tengxunRTDataSource.SetInquiring(true);

		tengxunRTDataSource.GenerateInquiryMessage(0);

		EXPECT_FALSE(tengxunRTDataSource.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestGenerateInquiryMessage2) {
		tengxunRTDataSource.SetInquiring(false);

		tengxunRTDataSource.GenerateInquiryMessage(0);

		EXPECT_TRUE(tengxunRTDataSource.IsInquiring());
		EXPECT_TRUE(tengxunRTDataSource.HaveInquiry());
		EXPECT_EQ(tengxunRTDataSource.InquiryQueueSize(), 1);
		tengxunRTDataSource.GetCurrentProduct();
		const auto pProduct = tengxunRTDataSource.GetCurrentInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), _T("class CProductTengxunRT"));
		EXPECT_FALSE(tengxunRTDataSource.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestIsTengxunRTDataInValid) {
		const CWebDataPtr pWebDataReceived = make_shared<CWebData>();
		string str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->Resize(str.length());
		pWebDataReceived->SetData(str.c_str(), str.length());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(tengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->Resize(str.length());
		pWebDataReceived->SetData(str.c_str(), str.length());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(tengxunRTDataSource.IsInvalidTengxunRTData(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}
}
