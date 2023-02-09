#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductTengxunRT.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTengxunRTTest : public ::testing::Test {
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
			GeneralCheck();
		}

	protected:
		CProductTengxunRT tengxunRT;
	};

	TEST_F(CProductTengxunRTTest, TestInitialize) {
		EXPECT_STREQ(tengxunRT.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
	}

	TEST_F(CProductTengxunRTTest, TestCreateMessage) {
		EXPECT_STREQ(tengxunRT.CreateMessage(), _T("http://qt.gtimg.cn/q=")) << "腾讯实时数据的申请字符串由CTengxunRTWebInquiry类完成，本Product无需动作";
	}

	TEST_F(CProductTengxunRTTest, TestParseTengxunRT) {
		vector<CWebRTDataPtr> vWebRTData;
		const auto pWebData = make_shared<CWebData>();
		EXPECT_FALSE(tengxunRT.ParseTengxunRT(vWebRTData, pWebData)) << "新浪实时数据非json制式，无需解析";
	}
}
