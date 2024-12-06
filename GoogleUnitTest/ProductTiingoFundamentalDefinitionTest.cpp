#include"pch.h"

////#include"gtest/gtest.h"

#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include"TiingoDataSource.h"

#include"ProductTiingoFundamentalDefinition.h"

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
		CProductTiingoFundamentalDefinition marketNews;
	};

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestInitialize) {
		EXPECT_EQ(marketNews.GetIndex(), 0);
		EXPECT_STREQ(marketNews.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/fundamentals/definitions?"));
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestCreatMessage) {
		EXPECT_STREQ(marketNews.CreateMessage(), marketNews.GetInquiryFunction());
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestUpdateDataSourceStatus1) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());

		marketNews.UpdateDataSourceStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());

		gl_pTiingoDataSource->SetUpdateFundamentalDefinition(true);
	}

	TEST_F(CProductTiingoFundamentalDefinitionTest, TestUpdateDataSourceStatus2) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());

		marketNews.UpdateDataSourceStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateFundamentalDefinition());

		gl_pTiingoDataSource->SetUpdateFundamentalDefinition(true);
	}

	// 正确的数据
	Test_TiingoWebData tiingoFundamentalDefinition1(1, _T(""), _T("[{\"dataCode\":\"liabilitiesCurrent\",\"name\":\"Current Liabilities\",\"description\":\"Debt or liabilities that are due within a year\",\"statementType\":\"balanceSheet\",\"units\":\"$\"},{\"dataCode\":\"rps\",\"name\":\"Revenue Per Share\",\"description\":\"Revenue per share\",\"statementType\":\"overview\",\"units\":\"$\"}]"));
	// 第一个数据缺项
	Test_TiingoWebData tiingoFundamentalDefinition2(2, _T(""), _T("[{\"dataCode\":\"liabilitiesCurrent\",\"name\":\"Current Liabilities\",\"description\":\"Debt or liabilities that are due within a year\",\"statementType\":\"balanceSheet\",\"units\":\"$\"},{\"dataCode\":\"rps\",\"name\":\"Revenue Per Share\",\"description\":\"Revenue per share\",\"statementType\":\"overview\",\"units\":\"$\"}]"));
	// 第二个数据缺项
	Test_TiingoWebData tiingoFundamentalDefinition3(3, _T("aapl"), _T("[{\"dataCode\":\"liabilitiesCurrent\",\"name\":\"Current Liabilities\",\"description\":\"Debt or liabilities that are due within a year\",\"statementType\":\"balanceSheet\",\"units\":\"$\"},{\"dataCode\":\"rps\",\"name\":\"Revenue Per Share\",\"description\":\"Revenue per share\",\"statementType\":\"overview\",\"units\":\"$\"}]"));
	// 正确的数据
	Test_TiingoWebData tiingoFundamentalDefinition4(4, _T(""), _T("[{\"dataCode\":\"liabilitiesCurrent\",\"name\":\"Current Liabilities\",\"description\":\"Debt or liabilities that are due within a year\",\"statementType\":\"balanceSheet\",\"units\":\"$\"},{\"dataCode\":\"rps\",\"name\":\"Revenue Per Share\",\"description\":\"Revenue per share\",\"statementType\":\"overview\",\"units\":\"$\"}]"));
	// 正确的数据
	Test_TiingoWebData tiingoFundamentalDefinition10(10, _T(""), _T("[{\"dataCode\":\"liabilitiesCurrent\",\"name\":\"Current Liabilities\",\"description\":\"Debt or liabilities that are due within a year\",\"statementType\":\"balanceSheet\",\"units\":\"$\"},{\"dataCode\":\"rps\",\"name\":\"Revenue Per Share\",\"description\":\"Revenue per share\",\"statementType\":\"overview\",\"units\":\"$\"}]"));

	class ParseTiingoFundamentalDefinitionTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoFundamentalDefinitionsPtr m_pvFundamentalDefinition;
		CProductTiingoFundamentalDefinition m_tiingoFundamentalDefinitionProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoFundamentalDefinition,
	                         ParseTiingoFundamentalDefinitionTest,
	                         testing::Values(&tiingoFundamentalDefinition1, &tiingoFundamentalDefinition2,
		                         &tiingoFundamentalDefinition3, &tiingoFundamentalDefinition4, &tiingoFundamentalDefinition10));

	TEST_P(ParseTiingoFundamentalDefinitionTest, TestParseTiingoFundamentalDefinition) {
		m_pvFundamentalDefinition = m_tiingoFundamentalDefinitionProduct.ParseTiingoFundamentalDefinition(m_pWebData);
		switch (m_lIndex) {
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
			EXPECT_STREQ(m_pvFundamentalDefinition->at(0)->m_strDataCode, _T("liabilitiesCurrent"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(0)->m_strName, _T("Current Liabilities"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(0)->m_strDescription, _T("Debt or liabilities that are due within a year"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(0)->m_strStatementType, _T("balanceSheet"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(0)->m_strUnits, _T("$"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(1)->m_strDataCode, _T("rps"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(1)->m_strName, _T("Revenue Per Share"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(1)->m_strDescription, _T("Revenue per share"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(1)->m_strStatementType, _T("overview"));
			EXPECT_STREQ(m_pvFundamentalDefinition->at(1)->m_strUnits, _T("$"));
			break;
		default:
			break;
		}
	}
	/*
	class ProcessTiingoFundamentalDefinitionTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			m_tiingoFundamentalDefinitionProduct.SetMarket(gl_pWorldMarket);
			m_tiingoFundamentalDefinitionProduct.SetIndex(0);
			gl_pTiingoDataSource->SetUpdateFundamentalDefinition(true);
			EXPECT_FALSE(gl_dataContainerFinnhubmarketNews.IsUpdateProfileDB());
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoFundamentalDefinition m_tiingoFundamentalDefinitionProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingomarketNews,
													 ProcessTiingoFundamentalDefinitionTest,
													 testing::Values(&tiingoFundamentalDefinition1, &tiingoFundamentalDefinition2,
														 &tiingoFundamentalDefinition3, &tiingoFundamentalDefinition4, &tiingoFundamentalDefinition10));

	TEST_P(ProcessTiingoFundamentalDefinitionTest, TestProcessmarketNews) {
		CTiingoFundamentalDefinitionPtr pTiingoFundamentalDefinition = nullptr;
		CFinnhubStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
		m_tiingoFundamentalDefinitionProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 2: // 格式不对
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 3: // 第二个数据缺乏address项,返回一个成功
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1) << "第一个数据是正确的";
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB()) << "第一个数据是正确的";

		//恢复原状
			pStock = gl_dataContainerFinnhubStock.GetStock(_T("A"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			pStock->SetUpdateProfileDB(false);
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());

		//恢复原状
			pStock = gl_dataContainerFinnhubStock.GetStock(_T("AA"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			pStock->SetUpdateProfileDB(false);
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(_T("NEW SYMBOL")));
			EXPECT_TRUE((pTiingoStock = gl_dataContainerTiingoStock.GetStock(_T("NEW SYMBOL"))) != nullptr);
			pStock = gl_dataContainerFinnhubStock.GetStock(_T("AA"));
			EXPECT_STREQ(pStock->GetName(), _T("New Name")) << "更改为此新名字";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());

		// 恢复原状
			gl_dataContainerTiingoStock.Delete(pTiingoStock);
			pStock->SetUpdateProfileDB(false);
			pStock->SetName(_T("Alcoa Corp"));
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		default:
			break;
		}
	}
	*/
}
