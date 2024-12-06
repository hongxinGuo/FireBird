#include"pch.h"

////#include"gtest/gtest.h"

#include"SystemConfiguration.h"

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
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strDataCode, _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strName, _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strDescription, _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strStatementType, _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strUnits, _T(""));
	}

	TEST_F(CTiingoFundamentalDefinitionTest, TestAppend) {
		CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition, setTiingoFundamentalDefinition2;
		CTiingoFundamentalDefinition fundamentalDefinitionToBeSaved;

		fundamentalDefinitionToBeSaved.m_strDataCode = _T("aaa");
		fundamentalDefinitionToBeSaved.m_strStatementType = _T("abdc");
		fundamentalDefinitionToBeSaved.m_strName = _T("don't reply");
		fundamentalDefinitionToBeSaved.m_strDescription = _T("no description");
		fundamentalDefinitionToBeSaved.m_strUnits = _T("$");

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setTiingoFundamentalDefinition.Open();
		setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
		fundamentalDefinitionToBeSaved.Append(setTiingoFundamentalDefinition);
		setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
		setTiingoFundamentalDefinition.Close();

		setTiingoFundamentalDefinition2.m_strFilter = _T("[dataCode] = 'aaa'");
		setTiingoFundamentalDefinition2.Open();
		setTiingoFundamentalDefinition2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setTiingoFundamentalDefinition2.IsEOF()) << "此时已经存入了AA";
		fundamentalDefinition.Load(setTiingoFundamentalDefinition2);
		EXPECT_STREQ(fundamentalDefinition.m_strDataCode, _T("aaa"));
		EXPECT_STREQ(fundamentalDefinition.m_strName, _T("don't reply"));
		EXPECT_STREQ(fundamentalDefinition.m_strDescription, _T("no description"));
		EXPECT_STREQ(fundamentalDefinition.m_strStatementType, _T("abdc"));
		EXPECT_STREQ(fundamentalDefinition.m_strUnits, _T("$"));
		setTiingoFundamentalDefinition2.Delete();
		setTiingoFundamentalDefinition2.m_pDatabase->CommitTrans();
		setTiingoFundamentalDefinition2.Close();
	}
}
