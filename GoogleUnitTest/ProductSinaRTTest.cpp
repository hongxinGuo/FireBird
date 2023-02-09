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
		EXPECT_STREQ(sinaRT.CreateMessage(), _T("https://hq.sinajs.cn/list=")) << "����ʵʱ���ݵ������ַ�����CSinaRTWebInquiry����ɣ���Product���趯��";
	}

	TEST_F(CProductSinaRTTest, TestParseSinaRT) {
		vector<CWebRTDataPtr> vWebRTData;
		const auto pWebData = make_shared<CWebData>();
		EXPECT_FALSE(sinaRT.ParseSinaRT(vWebRTData, pWebData)) << "����ʵʱ���ݷ�json��ʽ���������";
	}
}
