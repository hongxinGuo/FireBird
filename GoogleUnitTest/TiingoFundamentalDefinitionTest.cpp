#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoFundamentalDefinition.h"

namespace FireBirdTest {
	class CTiingoFundamentalDefinitionTest : public ::testing::Test {
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

		CTiingoFundamentalDefinition fundamentalDefinition;
	};

	TEST_F(CTiingoFundamentalDefinitionTest, TestInitialize) {
		const CTiingoFundamentalDefinition TiingoFundamentalDefinition;
		EXPECT_EQ(TiingoFundamentalDefinition.m_strDataCode, "");
		EXPECT_EQ(TiingoFundamentalDefinition.m_strName, "");
		EXPECT_EQ(TiingoFundamentalDefinition.m_strDescription, "");
		EXPECT_EQ(TiingoFundamentalDefinition.m_strStatementType, "");
		EXPECT_EQ(TiingoFundamentalDefinition.m_strUnits, "");
	}
}
