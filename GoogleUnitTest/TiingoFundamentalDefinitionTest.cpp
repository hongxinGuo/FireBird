#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoFundamentalDefinition.h"

namespace FireBirdTest {
	class CTiingoFundamentalDefinitionTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // ��������ĳ�ʼ������
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
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strDataCode.c_str(), _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strName.c_str(), _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strDescription.c_str(), _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strStatementType.c_str(), _T(""));
		EXPECT_STREQ(TiingoFundamentalDefinition.m_strUnits.c_str(), _T(""));
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
		EXPECT_TRUE(!setTiingoFundamentalDefinition2.IsEOF()) << "��ʱ�Ѿ�������AA";
		fundamentalDefinition.Load(setTiingoFundamentalDefinition2);
		EXPECT_STREQ(fundamentalDefinition.m_strDataCode.c_str(), _T("aaa"));
		EXPECT_STREQ(fundamentalDefinition.m_strName.c_str(), _T("don't reply"));
		EXPECT_STREQ(fundamentalDefinition.m_strDescription.c_str(), _T("no description"));
		EXPECT_STREQ(fundamentalDefinition.m_strStatementType.c_str(), _T("abdc"));
		EXPECT_STREQ(fundamentalDefinition.m_strUnits.c_str(), _T("$"));
		setTiingoFundamentalDefinition2.Delete();
		setTiingoFundamentalDefinition2.m_pDatabase->CommitTrans();
		setTiingoFundamentalDefinition2.Close();
	}
}
