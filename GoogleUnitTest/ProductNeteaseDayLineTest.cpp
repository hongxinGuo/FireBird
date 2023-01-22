#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductNeteaseDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CProductNeteaseDayLineTest : public Test {
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
		CProductNeteaseDayLine neteaseDayLine;
	};

	TEST_F(CProductNeteaseDayLineTest, TestInitialize) {
		EXPECT_STREQ(neteaseDayLine.GetInquiry(), _T("http://quotes.money.163.com/service/chddata.html?code="));
	}

	TEST_F(CProductNeteaseDayLineTest, TestCreateMessage) {
		EXPECT_STREQ(neteaseDayLine.CreateMessage(), _T("http://quotes.money.163.com/service/chddata.html?code=")) << "网易日线数据的申请字符串由CNeteaseDayLineWebInquiry类完成，本Product无需动作";
	}
}
