#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include"TiingoDataSource.h"

#include"ProductTiingoStockProfile.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoStockProfileTest : public Test {
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
		CProductTiingoStockProfile stockSymbol;
	};

	TEST_F(CProductTiingoStockProfileTest, TestInitialize) {
		EXPECT_EQ(stockSymbol.GetIndex(), 0);
		EXPECT_EQ(stockSymbol.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/fundamentals/meta?"));
	}

	TEST_F(CProductTiingoStockProfileTest, TestCreatMessage) {
		EXPECT_TRUE(stockSymbol.CreateMessage() == stockSymbol.GetInquiryFunction());
	}

	TEST_F(CProductTiingoStockProfileTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	TEST_F(CProductTiingoStockProfileTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateStockSymbol());

		stockSymbol.UpdateSystemStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), _T("Tiingo stock symbol updated"));
		EXPECT_EQ(gl_systemConfiguration.GetTiingoFundamentalsMetaUpdateDate(), gl_pWorldMarket->GetMarketDate());
		EXPECT_TRUE(gl_systemConfiguration.IsUpdateDB());

		// 恢复原状
		gl_pTiingoDataSource->SetUpdateStockSymbol(true);
		gl_systemConfiguration.SetTiingoFundamentalsMetaUpdateDate(19800101);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	// 下列数据的permaTicker项故意修改了，这样可以激活gl_dataContainerFinnhubStock的UpdateProfileDB().
	// 格式不对，缺乏'{'
	Test_TiingoWebData tiingoStockWebData1(1, _T(""),_T("[\"permaTicker\":\"US000000000500\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\",\"dataProviderPermaTicker\":\"123456\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\",\"dataProviderPermaTicker\":\"123456\"}]"));
	// 第一个数据缺项
	Test_TiingoWebData tiingoStockWebData2(2, _T(""),_T("[{\"Missing\":\"US000000000500\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\",\"dataProviderPermaTicker\":\"123456\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\",\"dataProviderPermaTicker\":\"123456\"}]"));
	// 第二个数据缺项
	Test_TiingoWebData tiingoStockWebData3(3, _T(""),_T("[{\"permaTicker\":\"US000000000500\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\",\"dataProviderPermaTicker\":\"123456\"},{\"Missing\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\",\"dataProviderPermaTicker\":\"123456\"}]"));
	// 正确的数据
	Test_TiingoWebData tiingoStockWebData4(4, _T(""), _T("[{\"permaTicker\":\"US000000000191\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"sector have data\",\"industry\":null,\"sicCode\":1234,\"sicSector\":\"sicSector have data\",\"sicIndustry\":\"sicIndustry have data\",\"reportingCurrency\":\"usd\",\"location\":\"location have data\",\"companyWebsite\":\"companyWebsite have data\",\"secFilingWebsite\":\"secFilingWebsite have data\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\",\"dataProviderPermaTicker\":\"123456\"}]"));
	// 正确的数据
	Test_TiingoWebData tiingoStockWebData10(10, _T(""),_T("[{\"permaTicker\":\"US000000001247\",\"ticker\":\"new symbol\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"\",\"industry\":\"free\",\"sicCode\":\"Field\",\"sicSector\":\"not\",\"sicIndustry\":\"for\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"free\",\"secFilingWebsite\":\"Field\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\",\"dataProviderPermaTicker\":\"123456\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"New Name\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\",\"dataProviderPermaTicker\":\"123456\"}]"));

	class ParseTiingoStockTest : public TestWithParam<Test_TiingoWebData*> {
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
		CTiingoStocksPtr m_pvStock;
		CProductTiingoStockProfile m_tiingoStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoStockProfile,
	                         ParseTiingoStockTest,
	                         testing::Values(&tiingoStockWebData1, &tiingoStockWebData2,
		                         &tiingoStockWebData3, &tiingoStockWebData4, &tiingoStockWebData10));

	TEST_P(ParseTiingoStockTest, TestParseStockProfile) {
		m_pvStock = m_tiingoStockSymbolProduct.ParseTiingoStockSymbol(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 3: // 缺乏address项
			EXPECT_EQ(m_pvStock->size(), 1);
			break;
		case 4:
			EXPECT_EQ(m_pvStock->size(), 1);
			EXPECT_EQ(m_pvStock->at(0)->GetTiingoPermaTicker(), _T("US000000000191"));
			EXPECT_EQ(m_pvStock->at(0)->GetSymbol(), _T("AA"));
			EXPECT_EQ(m_pvStock->at(0)->GetName(), _T("Alcoa Corp"));
			EXPECT_TRUE(m_pvStock->at(0)->IsActive());
			EXPECT_FALSE(m_pvStock->at(0)->IsADR());
			EXPECT_EQ(m_pvStock->at(0)->GetTiingoIndustry(), _T("")) << "内容为null时返回空串_T("")";
			EXPECT_EQ(m_pvStock->at(0)->GetTiingoSector(), _T("sector have data"));
			EXPECT_EQ(m_pvStock->at(0)->GetSicIndustry(), _T("sicIndustry have data"));
			EXPECT_EQ(m_pvStock->at(0)->GetSicSector(), _T("sicSector have data"));
			EXPECT_EQ(m_pvStock->at(0)->GetReportingCurrency(), _T("usd"));
			EXPECT_EQ(m_pvStock->at(0)->GetLocation(), _T("location have data"));
			EXPECT_EQ(m_pvStock->at(0)->GetCompanyWebSite(), _T("companyWebsite have data"));
			EXPECT_EQ(m_pvStock->at(0)->GetSECFilingWebSite(), _T("secFilingWebsite have data"));
			EXPECT_EQ(m_pvStock->at(0)->GetStatementLastUpdatedDate(), 20210302);
			EXPECT_EQ(m_pvStock->at(0)->GetSicCode(), 1234);
			EXPECT_EQ(m_pvStock->at(0)->GetDataProviderPermaTicker(), _T("123456"));
			break;
		case 10:
			EXPECT_EQ(m_pvStock->size(), 2);
			EXPECT_EQ(m_pvStock->at(0)->GetTiingoPermaTicker(), _T("US000000001247"));
			EXPECT_EQ(m_pvStock->at(0)->GetSymbol(), _T("NEW SYMBOL"));
			EXPECT_EQ(m_pvStock->at(0)->GetName(), _T("Agilent Technologies Inc"));
			EXPECT_TRUE(m_pvStock->at(0)->IsActive());
			EXPECT_FALSE(m_pvStock->at(0)->IsADR());
			EXPECT_EQ(m_pvStock->at(0)->GetTiingoIndustry(), _T("free"));
			EXPECT_EQ(m_pvStock->at(0)->GetTiingoSector(), _T(""));
			EXPECT_EQ(m_pvStock->at(0)->GetSicCode(), 0);
			EXPECT_EQ(m_pvStock->at(0)->GetSicIndustry(), _T("for"));
			EXPECT_EQ(m_pvStock->at(0)->GetSicSector(), _T("not"));
			EXPECT_EQ(m_pvStock->at(0)->GetReportingCurrency(), _T("usd"));
			EXPECT_EQ(m_pvStock->at(0)->GetLocation(), _T(""));
			EXPECT_EQ(m_pvStock->at(0)->GetCompanyWebSite(), _T("free"));
			EXPECT_EQ(m_pvStock->at(0)->GetSECFilingWebSite(), _T("Field"));
			EXPECT_EQ(m_pvStock->at(0)->GetStatementLastUpdatedDate(), 20210305);
			EXPECT_EQ(m_pvStock->at(1)->GetTiingoPermaTicker(), _T("US000000000091"));
			EXPECT_EQ(m_pvStock->at(1)->GetSymbol(), _T("AA"));
			EXPECT_EQ(m_pvStock->at(1)->GetName(), _T("New Name"));
			EXPECT_TRUE(m_pvStock->at(1)->IsActive());
			EXPECT_FALSE(m_pvStock->at(1)->IsADR());
			EXPECT_EQ(m_pvStock->at(1)->GetTiingoIndustry(), _T("")) << "当字符串为Field not available for free/evaluation时，返回空串";
			EXPECT_EQ(m_pvStock->at(1)->GetTiingoSector(), _T(""));
			EXPECT_EQ(m_pvStock->at(1)->GetSicCode(), 0);
			EXPECT_EQ(m_pvStock->at(1)->GetSicIndustry(), _T(""));
			EXPECT_EQ(m_pvStock->at(1)->GetSicSector(), _T(""));
			EXPECT_EQ(m_pvStock->at(1)->GetReportingCurrency(), _T("usd"));
			EXPECT_EQ(m_pvStock->at(1)->GetLocation(), _T(""));
			EXPECT_EQ(m_pvStock->at(1)->GetCompanyWebSite(), _T(""));
			EXPECT_EQ(m_pvStock->at(1)->GetSECFilingWebSite(), _T(""));
			EXPECT_EQ(m_pvStock->at(1)->GetStatementLastUpdatedDate(), 20210302);
			EXPECT_EQ(m_pvStock->at(1)->GetDataProviderPermaTicker(), _T("123456"));
			break;
		default:
			break;
		}
	}

	class ProcessTiingoStockProfileTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			m_tiingoStockProduct.SetMarket(gl_pWorldMarket);
			m_tiingoStockProduct.SetIndex(0);
			gl_pTiingoDataSource->SetUpdateStockSymbol(true);
		}

		void TearDown() override {
			// clearUp
			gl_systemConfiguration.SetUpdateDB(false);
			gl_dataContainerTiingoStock.GetStock(_T("A"))->SetUpdateProfileDB(false);
			gl_dataContainerTiingoStock.GetStock(_T("AA"))->SetUpdateProfileDB(false);
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoStockProfile m_tiingoStockProduct;
		long long m_llTiingoBandWidthLeft;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockProfile,
	                         ProcessTiingoStockProfileTest,
	                         testing::Values(&tiingoStockWebData1, &tiingoStockWebData2,
		                         &tiingoStockWebData3, &tiingoStockWebData4, &tiingoStockWebData10));

	//todo 完善之
	TEST_P(ProcessTiingoStockProfileTest, TestProcessStockProfile) {
		CTiingoStockPtr pTiingoStock = nullptr;
		m_tiingoStockProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			break;
		case 2: // 格式不对
			break;
		case 3: // 第二个数据缺乏address项,返回一个成功
			break;
		case 4:
			EXPECT_FALSE(gl_dataContainerTiingoStock.IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_FALSE(gl_dataContainerTiingoStock.IsSymbol(_T("NEW SYMBOL"))) << "抛弃退市股票";
			break;
		default:
			break;
		}
	}
}
