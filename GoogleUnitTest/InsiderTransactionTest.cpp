#include"pch.h"

#include"GeneralCheck.h"

#include"InsiderTransaction.h"

namespace FireBirdTest {
	class CInsiderTransactionTest : public ::testing::Test {
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

	TEST_F(CInsiderTransactionTest, TestInitialize) {
		const CInsiderTransaction insiderTransaction;
		EXPECT_EQ(insiderTransaction.m_strSymbol, " ");
		EXPECT_EQ(insiderTransaction.m_strPersonName, " ");
		EXPECT_EQ(insiderTransaction.m_lShare, 0);
		EXPECT_EQ(insiderTransaction.m_lShareChange, 0);
		EXPECT_EQ(insiderTransaction.m_lFilingDate, 19800101);
		EXPECT_EQ(insiderTransaction.m_lTransactionDate, 19800101);
		EXPECT_DOUBLE_EQ(insiderTransaction.m_dTransactionPrice, 0.0);
	}
}
