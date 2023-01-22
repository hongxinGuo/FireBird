#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductNeteaseRT.h"

using namespace testing;

namespace FireBirdTest {
	class CProductNeteaseRTTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CProductNeteaseRT neteaseRT;
	};

	TEST_F(CProductNeteaseRTTest, TestInitialize) {
		EXPECT_STREQ(neteaseRT.GetInquiry(), _T("http://api.money.126.net/data/feed/"));
	}

	TEST_F(CProductNeteaseRTTest, TestCreateMessage) {
		EXPECT_STREQ(neteaseRT.CreateMessage(), _T("http://api.money.126.net/data/feed/")) << "����ʵʱ���ݵ������ַ�����CNeteaseRTWebInquiry����ɣ���Product���趯��";
	}

	TEST_F(CProductNeteaseRTTest, TestParseNeteaseRT) {
		vector<CWebRTDataPtr> vWebRTData;
		const auto pWebData = make_shared<CWebData>();
		EXPECT_FALSE(neteaseRT.ParseNeteaseRT(vWebRTData, pWebData)) << "����ʵʱ���ݷ�json��ʽ���������";
	}
}
