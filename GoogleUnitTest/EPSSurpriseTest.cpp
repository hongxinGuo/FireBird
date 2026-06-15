#include"pch.h"

#include"GeneralCheck.h"

#include"EPSSurprise.h"

namespace FireBirdTest {
	class CEPSSurpriseTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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
	};

	TEST_F(CEPSSurpriseTest, TestInitialize) {
		const CEPSSurprise EPSSurprise;
		EXPECT_EQ(EPSSurprise.m_strSymbol, " ");
		EXPECT_EQ(EPSSurprise.m_lDate, chrono::local_days(chrono::days(0)));
		EXPECT_DOUBLE_EQ(EPSSurprise.m_dActual, 0.0);
		EXPECT_DOUBLE_EQ(EPSSurprise.m_dEstimate, 0.0);
	}
}
