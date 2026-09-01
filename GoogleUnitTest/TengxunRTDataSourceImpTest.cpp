#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"

#include"VirtualWebProduct.h"
#include"tengxunRTDataSource.h"
#include"ChinaMarket.h"
#include"TimeConvert.h"

using namespace testing;
using std::make_shared;

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
		EXPECT_EQ(tengxunRTDataSource.GetInquiryFunction(), "http://qt.gtimg.cn/q=");
		EXPECT_EQ(tengxunRTDataSource.GetToken(), "");
		EXPECT_EQ(tengxunRTDataSource.GetInquiringNumber(), 900) << "腾讯默认值";
	}

	TEST_F(CTengxunRTDataSourceTest, TestGenerateInquiryMessage1) {
		tengxunRTDataSource.SetInquiring(true);

		tengxunRTDataSource.GenerateInquiryMessage(toLocalTime(0));

		EXPECT_FALSE(tengxunRTDataSource.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestGenerateInquiryMessage2) {
		tengxunRTDataSource.SetInquiring(false);

		tengxunRTDataSource.GenerateInquiryMessage(toLocalTime(0));

		EXPECT_TRUE(tengxunRTDataSource.HaveInquiry());
		EXPECT_EQ(tengxunRTDataSource.InquiryQueueSize(), 1);
		tengxunRTDataSource.GetCurrentProduct();
		const auto pProduct = tengxunRTDataSource.GetCurrentInquiry();
		EXPECT_STREQ(typeid(*pProduct).name(), "class CProductTengxunRT");
		EXPECT_FALSE(tengxunRTDataSource.HaveInquiry());
	}

	TEST_F(CTengxunRTDataSourceTest, TestIsTengxunRTDataInValid) {
		string str = "v_pv_none_match=\"1\";\n";

		EXPECT_TRUE(tengxunRTDataSource.IsInvalidTengxunRTData(str));

		str = "v_pv_none_mtch=\"1\";\n";

		EXPECT_FALSE(tengxunRTDataSource.IsInvalidTengxunRTData(str));
	}
}
