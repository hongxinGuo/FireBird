#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"

#include"SystemMessage.h"

#include"TiingoDataSource.h"
#include"ProductTiingoFundamentalDefinition.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoFundamentalDefinitionTest : public Test {
	protected:
		static void SetUpTestSuite() {
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

	protected:
		CProductTiingoFundamentalDefinition marketFundamentalDefinition;
	};

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestInitialize) {
		EXPECT_EQ(marketFundamentalDefinition.GetIndex(), 0);
		EXPECT_EQ(marketFundamentalDefinition.GetInquiryFunction(), "https://api.tiingo.com/tiingo/fundamentals/definitions?");
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestCreatMessage) {
		EXPECT_EQ(marketFundamentalDefinition.CreateMessage()->front(), marketFundamentalDefinition.GetInquiryFunction());
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestUpdateDataSourceStatus1) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());

		marketFundamentalDefinition.UpdateSystemStatus();

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), "Fundamental definition updated");

		gl_pTiingoDataSource->SetUpdateFundamentalDefinition(true);
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestUpdateDataSourceStatus2) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());

		marketFundamentalDefinition.UpdateSystemStatus();

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), "Fundamental definition updated");

		gl_pTiingoDataSource->SetUpdateFundamentalDefinition(true);
	}

	namespace {
		// 正确的数据
		Test_TiingoWebData tiingoFundamentalDefinition1(1, "", R"([{"dataCode":"liabilitiesCurrent","name":"Current Liabilities","description":"Debt or liabilities that are due within a year","statementType":"balanceSheet","units":"$"},{"dataCode":"rps","name":"Revenue Per Share","description":"Revenue per share","statementType":"overview","units":"$"}])");
		// 第一个数据缺项
		Test_TiingoWebData tiingoFundamentalDefinition2(2, "", R"([{"dataCode":"liabilitiesCurrent","name":"Current Liabilities","description":"Debt or liabilities that are due within a year","statementType":"balanceSheet","units":"$"},{"dataCode":"rps","name":"Revenue Per Share","description":"Revenue per share","statementType":"overview","units":"$"}])");
		// 第二个数据缺项
		Test_TiingoWebData tiingoFundamentalDefinition3(3, "aapl", R"([{"dataCode":"liabilitiesCurrent","name":"Current Liabilities","description":"Debt or liabilities that are due within a year","statementType":"balanceSheet","units":"$"},{"dataCode":"rps","name":"Revenue Per Share","description":"Revenue per share","statementType":"overview","units":"$"}])");
		// 正确的数据
		Test_TiingoWebData tiingoFundamentalDefinition4(4, "", R"([{"dataCode":"liabilitiesCurrent","name":"Current Liabilities","description":"Debt or liabilities that are due within a year","statementType":"balanceSheet","units":"$"},{"dataCode":"rps","name":"Revenue Per Share","description":"Revenue per share","statementType":"overview","units":"$"}])");
		// 正确的数据
		Test_TiingoWebData tiingoFundamentalDefinition10(10, "", R"([{"dataCode":"liabilitiesCurrent","name":"Current Liabilities","description":"Debt or liabilities that are due within a year","statementType":"balanceSheet","units":"$"},{"dataCode":"rps","name":"Revenue Per Share","description":"Revenue per share","statementType":"overview","units":"$"}])");
	}

	class ParseTiingoFundamentalDefinitionTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_index = pData->m_index;
			m_text = pData->m_data;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		string m_text;
		CTiingoFundamentalDefinitionsPtr m_pvFundamentalDefinition;
		CProductTiingoFundamentalDefinition m_tiingoFundamentalDefinitionProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoFundamentalDefinition,
	                         ParseTiingoFundamentalDefinitionTest,
	                         testing::Values(&tiingoFundamentalDefinition1, &tiingoFundamentalDefinition2,
		                         &tiingoFundamentalDefinition3, &tiingoFundamentalDefinition4, &tiingoFundamentalDefinition10));

	TEST_P(ParseTiingoFundamentalDefinitionTest, TestParseTiingoFundamentalDefinition) {
		m_pvFundamentalDefinition = m_tiingoFundamentalDefinitionProduct.Parse(m_text);
		switch (m_index) {
		case 1: // 格式不对
			EXPECT_EQ(m_pvFundamentalDefinition->size(), 2);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvFundamentalDefinition->size(), 2);
			break;
		case 3: // 缺乏address项
			EXPECT_EQ(m_pvFundamentalDefinition->size(), 2);
			break;
		case 4:
			EXPECT_EQ(m_pvFundamentalDefinition->size(), 2);
			break;
		case 10:
			EXPECT_EQ(m_pvFundamentalDefinition->size(), 2);
			EXPECT_EQ(m_pvFundamentalDefinition->at(0).m_strDataCode, "liabilitiesCurrent");
			EXPECT_EQ(m_pvFundamentalDefinition->at(0).m_strName, "Current Liabilities");
			EXPECT_EQ(m_pvFundamentalDefinition->at(0).m_strDescription, "Debt or liabilities that are due within a year");
			EXPECT_EQ(m_pvFundamentalDefinition->at(0).m_strStatementType, "balanceSheet");
			EXPECT_EQ(m_pvFundamentalDefinition->at(0).m_strUnits, "$");
			EXPECT_EQ(m_pvFundamentalDefinition->at(1).m_strDataCode, "rps");
			EXPECT_EQ(m_pvFundamentalDefinition->at(1).m_strName, "Revenue Per Share");
			EXPECT_EQ(m_pvFundamentalDefinition->at(1).m_strDescription, "Revenue per share");
			EXPECT_EQ(m_pvFundamentalDefinition->at(1).m_strStatementType, "overview");
			EXPECT_EQ(m_pvFundamentalDefinition->at(1).m_strUnits, "$");
			break;
		default:
			break;
		}
	}
}
