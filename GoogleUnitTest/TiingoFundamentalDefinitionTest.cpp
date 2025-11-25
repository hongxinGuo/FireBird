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

	TEST_F(CTiingoFundamentalDefinitionTest, TestAppend) {
		CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition, setTiingoFundamentalDefinition2;
		CTiingoFundamentalDefinition fundamentalDefinitionToBeSaved;

		fundamentalDefinitionToBeSaved.m_strDataCode = "aaa";
		fundamentalDefinitionToBeSaved.m_strStatementType = "abdc";
		fundamentalDefinitionToBeSaved.m_strName = "don't reply";
		fundamentalDefinitionToBeSaved.m_strDescription = "no description";
		fundamentalDefinitionToBeSaved.m_strUnits = "$";

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setTiingoFundamentalDefinition.Open();
		setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
		fundamentalDefinitionToBeSaved.Append(setTiingoFundamentalDefinition);
		setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
		setTiingoFundamentalDefinition.Close();

		setTiingoFundamentalDefinition2.m_strFilter = "[dataCode] = 'aaa'";
		setTiingoFundamentalDefinition2.Open();
		setTiingoFundamentalDefinition2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setTiingoFundamentalDefinition2.IsEOF()) << "此时已经存入了AA";
		fundamentalDefinition.Load(setTiingoFundamentalDefinition2);
		EXPECT_EQ(fundamentalDefinition.m_strDataCode, "aaa");
		EXPECT_EQ(fundamentalDefinition.m_strName, "don't reply");
		EXPECT_EQ(fundamentalDefinition.m_strDescription, "no description");
		EXPECT_EQ(fundamentalDefinition.m_strStatementType, "abdc");
		EXPECT_EQ(fundamentalDefinition.m_strUnits, "$");
		setTiingoFundamentalDefinition2.Delete();
		setTiingoFundamentalDefinition2.m_pDatabase->CommitTrans();
		setTiingoFundamentalDefinition2.Close();
	}
}
