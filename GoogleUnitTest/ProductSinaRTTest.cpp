#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductSinaRT.h"

using namespace testing;

namespace FireBirdTest {
	class CProductSinaRTTest : public ::testing::Test {
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
		CProductSinaRT sinaRT;
	};

	TEST_F(CProductSinaRTTest, TestInitialize) {
		EXPECT_STREQ(sinaRT.GetInquiryFunction(), _T("https://hq.sinajs.cn/list="));
	}

	TEST_F(CProductSinaRTTest, TestCreateMessage) {
		EXPECT_STREQ(sinaRT.CreateMessage(), _T("https://hq.sinajs.cn/list=")) << "新浪实时数据的申请字符串由CSinaRTWebInquiry类完成，本Product无需动作";
	}

	TEST_F(CProductSinaRTTest, TestParseSinaRT) {
		vector<CWebRTDataPtr> vWebRTData;
		const auto pWebData = make_shared<CWebData>();
		EXPECT_FALSE(sinaRT.ParseSinaRT(vWebRTData, pWebData)) << "新浪实时数据非json制式，无需解析";
	}
}
