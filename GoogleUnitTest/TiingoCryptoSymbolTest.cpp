#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoCrypto.h"

namespace FireBirdTest {
	class CTiingoCryptoSymbolTest : public ::testing::Test {
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

	TEST_F(CTiingoCryptoSymbolTest, TestInitialize) {
		const CTiingoCrypto tiingo;

		EXPECT_EQ(tiingo.m_strBaseCurrency, "");
		EXPECT_EQ(tiingo.GetSymbol(), "");
		EXPECT_EQ(tiingo.GetDescription(), "");
		EXPECT_EQ(tiingo.m_strQuoteCurrency, "");
		EXPECT_EQ(tiingo.m_strName, "");

		EXPECT_EQ(tiingo.GetRatio(), 100000);
	}
}
