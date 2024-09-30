#include"pch.h"

#include"GeneralCheck.h"

#include"WorldStock.h"
#include"WorldMarket.h"

#include"TiingoDataSource.h"

#include"ProductTiingoStock.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoStockTest : public Test {
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
		CProductTiingoStock stockSymbol;
	};

	TEST_F(CProductTiingoStockTest, TestInitialize) {
		EXPECT_EQ(stockSymbol.GetIndex(), -1);
		EXPECT_STREQ(stockSymbol.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/fundamentals/meta?"));
	}

	TEST_F(CProductTiingoStockTest, TestCreatMessage) {
		EXPECT_STREQ(stockSymbol.CreateMessage(), stockSymbol.GetInquiryFunction());
	}

	TEST_F(CProductTiingoStockTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	TEST_F(CProductTiingoStockTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateStockSymbol());

		stockSymbol.UpdateDataSourceStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage(), _T("Tiingo stock symbol已更新"));

		gl_pTiingoDataSource->SetUpdateStockSymbol(true);
	}

	// 下列数据的permaTicker项故意修改了，这样可以激活gl_dataContainerFinnhubStock的UpdateProfileDB().
	// 格式不对，缺乏'{'
	Test_TiingoWebData tiingoStockWebData1(1, _T(""),_T("[\"permaTicker\":\"US000000000500\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 第一个数据缺项
	Test_TiingoWebData tiingoStockWebData2(2, _T(""),_T("[{\"Missing\":\"US000000000500\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 第二个数据缺项
	Test_TiingoWebData tiingoStockWebData3(3, _T(""),_T("[{\"permaTicker\":\"US000000000500\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"Missing\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 正确的数据
	Test_TiingoWebData tiingoStockWebData4(4, _T(""), _T("[{\"permaTicker\":\"US000000000191\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"sector have data\",\"industry\":\"industry have new data\",\"sicCode\":1234,\"sicSector\":\"sicSector have data\",\"sicIndustry\":\"sicIndustry have data\",\"reportingCurrency\":\"usd\",\"location\":\"location have data\",\"companyWebsite\":\"companyWebsite have data\",\"secFilingWebsite\":\"secFilingWebsite have data\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 正确的数据
	Test_TiingoWebData tiingoStockWebData10(10, _T(""),_T("[{\"permaTicker\":\"US000000001247\",\"ticker\":\"new symbol\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"\",\"industry\":\"free\",\"sicCode\":\"Field\",\"sicSector\":\"not\",\"sicIndustry\":\"for\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"free\",\"secFilingWebsite\":\"Field\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"New Name\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));

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
		CProductTiingoStock m_tiingoStockSymbolProduct;
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
			EXPECT_STREQ(m_pvStock->at(0)->m_strTiingoPermaTicker, _T("US000000000191"));
			EXPECT_STREQ(m_pvStock->at(0)->GetSymbol(), _T("AA"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strName, _T("Alcoa Corp"));
			EXPECT_TRUE(m_pvStock->at(0)->m_fIsActive);
			EXPECT_FALSE(m_pvStock->at(0)->m_fIsADR);
			EXPECT_STREQ(m_pvStock->at(0)->m_strTiingoIndustry, _T("industry have new data"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strTiingoSector, _T("sector have data"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strSICIndustry, _T("sicIndustry have data"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strSICSector, _T("sicSector have data"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strReportingCurrency, _T("usd"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strLocation, _T("location have data"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strCompanyWebSite, _T("companyWebsite have data"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strSECFilingWebSite, _T("secFilingWebsite have data"));
			EXPECT_EQ(m_pvStock->at(0)->GetCompanyFinancialStatementUpdateDate(), 19800101) << "此项冲突，不存储";
			EXPECT_EQ(m_pvStock->at(0)->GetDailyDataUpdateDate(), 19800101) << "此项冲突，不存储";
			EXPECT_EQ(m_pvStock->at(0)->m_iSICCode, 1234);
			break;
		case 10:
			EXPECT_EQ(m_pvStock->size(), 2);
			EXPECT_STREQ(m_pvStock->at(0)->m_strTiingoPermaTicker, _T("US000000001247"));
			EXPECT_STREQ(m_pvStock->at(0)->GetSymbol(), _T("NEW SYMBOL"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strName, _T("Agilent Technologies Inc"));
			EXPECT_TRUE(m_pvStock->at(0)->m_fIsActive);
			EXPECT_FALSE(m_pvStock->at(0)->m_fIsADR);
			EXPECT_STREQ(m_pvStock->at(0)->m_strTiingoIndustry, _T("free"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strTiingoSector, _T(""));
			EXPECT_EQ(m_pvStock->at(0)->m_iSICCode, 0);
			EXPECT_STREQ(m_pvStock->at(0)->m_strSICIndustry, _T("for"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strSICSector, _T("not"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strReportingCurrency, _T("usd"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strLocation, _T(" "));
			EXPECT_STREQ(m_pvStock->at(0)->m_strCompanyWebSite, _T("free"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strSECFilingWebSite, _T("Field"));
			EXPECT_EQ(m_pvStock->at(0)->GetCompanyFinancialStatementUpdateDate(), 19800101) << "此项冲突，不存储";
			EXPECT_EQ(m_pvStock->at(0)->GetDailyDataUpdateDate(), 19800101) << "此项冲突，不存储";
			EXPECT_STREQ(m_pvStock->at(1)->m_strTiingoPermaTicker, _T("US000000000091"));
			EXPECT_STREQ(m_pvStock->at(1)->GetSymbol(), _T("AA"));
			EXPECT_STREQ(m_pvStock->at(1)->m_strName, _T("New Name"));
			EXPECT_TRUE(m_pvStock->at(1)->m_fIsActive);
			EXPECT_FALSE(m_pvStock->at(1)->m_fIsADR);
			EXPECT_STREQ(m_pvStock->at(1)->m_strTiingoIndustry, _T(" ")) << "当字符串为Field not available for free/evaluation时，返回空串(一个空格)";
			EXPECT_STREQ(m_pvStock->at(1)->m_strTiingoSector, _T(" "));
			EXPECT_EQ(m_pvStock->at(1)->m_iSICCode, 0);
			EXPECT_STREQ(m_pvStock->at(1)->m_strSICIndustry, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strSICSector, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strReportingCurrency, _T("usd"));
			EXPECT_STREQ(m_pvStock->at(1)->m_strLocation, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strCompanyWebSite, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strSECFilingWebSite, _T(" "));
			EXPECT_EQ(m_pvStock->at(1)->GetCompanyFinancialStatementUpdateDate(), 19800101) << "此项冲突，不存储";
			EXPECT_EQ(m_pvStock->at(1)->GetDailyDataUpdateDate(), 19800101) << "此项冲突，不存储";
			break;
		default:
			break;
		}
	}

	class ProcessTiingoStockTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			m_tiingoStockSymbolProduct.SetMarket(gl_pWorldMarket);
			m_tiingoStockSymbolProduct.SetIndex(0);
			gl_pTiingoDataSource->SetUpdateStockSymbol(true);
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
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
		CProductTiingoStock m_tiingoStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockProfile,
	                         ProcessTiingoStockTest,
	                         testing::Values(&tiingoStockWebData1, &tiingoStockWebData2,
		                         &tiingoStockWebData3, &tiingoStockWebData4, &tiingoStockWebData10));

	TEST_P(ProcessTiingoStockTest, TestProcessStockProfile) {
		CTiingoStockPtr pTiingoStock = nullptr;
		CWorldStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
		m_tiingoStockSymbolProduct.ParseAndStoreWebData(m_pWebData);
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
			EXPECT_TRUE(gl_dataContainerTiingoStock.IsSymbol(_T("NEW SYMBOL")));
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
}
