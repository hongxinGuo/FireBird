#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldStock.h"
#include"WorldMarket.h"

#include"ProductTiingoStockSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CProductTiingoStockSymbolsTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductTinngoStockSymbol stockSymbols;
	};

	TEST_F(CProductTiingoStockSymbolsTest, TestInitialize) {
		EXPECT_EQ(stockSymbols.GetIndex(), -1);
		EXPECT_STREQ(stockSymbols.GetInquiringStr(), _T("https://api.tiingo.com/tiingo/fundamentals/meta?"));
	}

	TEST_F(CProductTiingoStockSymbolsTest, TestCreatMessage) {
		EXPECT_STREQ(stockSymbols.CreatMessage(), stockSymbols.GetInquiringStr());
	}

	TEST_F(CProductTiingoStockSymbolsTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对，缺乏'{'
	TiingoWebData tiingoWebData1(1, _T(""), _T("[\"permaTicker\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 第一个数据缺项
	TiingoWebData tiingoWebData2(2, _T(""), _T("[{\"Missing\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 第二个数据缺项
	TiingoWebData tiingoWebData3(3, _T(""), _T("[{\"permaTicker\":\"US000000000247\",\"ticker\":\"a\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"Missing\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 正确的数据
	TiingoWebData tiingoWebData4(4, _T(""), _T("[{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"Alcoa Corp\", \"isActive\":true,\"isADR\":false,\"sector\":\"sector have data\",\"industry\":\"industry have new data\",\"sicCode\":\"1234\",\"sicSector\":\"sicSector have data\",\"sicIndustry\":\"sicIndustry have data\",\"reportingCurrency\":\"usd\",\"location\":\"location have data\",\"companyWebsite\":\"companyWebsite have data\",\"secFilingWebsite\":\"secFileingWebsite have data\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));
	// 正确的数据
	TiingoWebData tiingoWebData10(10, _T(""), _T("[{\"permaTicker\":\"US000000000247\",\"ticker\":\"new symbol\",\"name\":\"Agilent Technologies Inc\",\"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-05T23:02:07.999Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.052Z\"},{\"permaTicker\":\"US000000000091\",\"ticker\":\"aa\",\"name\":\"New Name\", \"isActive\":true,\"isADR\":false,\"sector\":\"Field not available for free/evaluation\",\"industry\":\"Field not available for free/evaluation\",\"sicCode\":\"Field not available for free/evaluation\",\"sicSector\":\"Field not available for free/evaluation\",\"sicIndustry\":\"Field not available for free/evaluation\",\"reportingCurrency\":\"usd\",\"location\":\"Field not available for free/evaluation\",\"companyWebsite\":\"Field not available for free/evaluation\",\"secFilingWebsite\":\"Field not available for free/evaluation\",\"statementLastUpdated\":\"2021-03-02T23:02:04.611Z\",\"dailyLastUpdated\":\"2021-03-12T21:54:08.226Z\"}]"));

	class ParseTiingoStockTest : public::testing::TestWithParam<TiingoWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoStockVectorPtr m_pvStock;
		CProductTinngoStockSymbol m_tiingoStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoStock,
													 ParseTiingoStockTest,
													 testing::Values(&tiingoWebData1, &tiingoWebData2,
														 &tiingoWebData3, &tiingoWebData4, &tiingoWebData10));

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
			EXPECT_STREQ(m_pvStock->at(0)->m_strTiingoPermaTicker, _T("US000000000091"));
			EXPECT_STREQ(m_pvStock->at(0)->m_strTicker, _T("AA"));
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
			EXPECT_STREQ(m_pvStock->at(0)->m_strSECFilingWebSite, _T("secFileingWebsite have data"));
			EXPECT_EQ(m_pvStock->at(0)->m_lStatementUpdateDate, 20210302);
			EXPECT_EQ(m_pvStock->at(0)->m_lDailyDataUpdateDate, 20210312);
			EXPECT_EQ(m_pvStock->at(0)->m_iSICCode, 1234);
			break;
		case 10:
			EXPECT_EQ(m_pvStock->size(), 2);
			EXPECT_STREQ(m_pvStock->at(1)->m_strTiingoPermaTicker, _T("US000000000091"));
			EXPECT_STREQ(m_pvStock->at(1)->m_strTicker, _T("AA"));
			EXPECT_STREQ(m_pvStock->at(1)->m_strName, _T("New Name"));
			EXPECT_TRUE(m_pvStock->at(1)->m_fIsActive);
			EXPECT_FALSE(m_pvStock->at(1)->m_fIsADR);
			EXPECT_STREQ(m_pvStock->at(1)->m_strTiingoIndustry, _T(" ")) << "当字符串为Field not available for free/evcaluation时，返回空串(一个空格)";
			EXPECT_STREQ(m_pvStock->at(1)->m_strTiingoSector, _T(" "));
			EXPECT_EQ(m_pvStock->at(1)->m_iSICCode, 0);
			EXPECT_STREQ(m_pvStock->at(1)->m_strSICIndustry, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strSICSector, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strReportingCurrency, _T("usd"));
			EXPECT_STREQ(m_pvStock->at(1)->m_strLocation, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strCompanyWebSite, _T(" "));
			EXPECT_STREQ(m_pvStock->at(1)->m_strSECFilingWebSite, _T(" "));
			EXPECT_EQ(m_pvStock->at(1)->m_lStatementUpdateDate, 20210302);
			EXPECT_EQ(m_pvStock->at(1)->m_lDailyDataUpdateDate, 20210312);
			break;
		}
	}

	class ProcessTiingoStockTest : public::testing::TestWithParam<TiingoWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
			m_tiingoStockSymbolProduct.SetMarket(gl_pWorldMarket.get());
			m_tiingoStockSymbolProduct.SetIndex(0);
			gl_pWorldMarket->SetTiingoStockSymbolUpdated(false);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTinngoStockSymbol m_tiingoStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStock,
													 ProcessTiingoStockTest,
													 testing::Values(&tiingoWebData1, &tiingoWebData2,
														 &tiingoWebData3, &tiingoWebData4, &tiingoWebData10));

	TEST_P(ProcessTiingoStockTest, TestProcessStockProfile) {
		CTiingoStockPtr pTiingoStock = nullptr;
		CWorldStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());
		EXPECT_FALSE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());
		bool fSucceed = m_tiingoStockSymbolProduct.ProcessWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());
			EXPECT_EQ(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());
			break;
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());
			EXPECT_EQ(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());
			break;
		case 3: // 第二个数据缺乏address项
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());
			EXPECT_EQ(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1) << "第一个数据是正确的";
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());
			break;
		case 4:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());
			EXPECT_EQ(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_pWorldMarket->IsStockProfileNeedUpdate());

			//恢复原状
			pStock = gl_pWorldMarket->GetStock(_T("AA"));
			pStock->SetUpdateProfileDB(false);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());
			EXPECT_EQ(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_pWorldMarket->IsTiingoStock(_T("NEW SYMBOL")));
			EXPECT_TRUE((pTiingoStock = gl_pWorldMarket->GetTiingoStock(_T("NEW SYMBOL"))) != nullptr);
			pStock = gl_pWorldMarket->GetStock(_T("AA"));
			EXPECT_STREQ(pStock->GetName(), _T("New Name")) << "更改为此新名字";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());

			// 恢复原状
			gl_pWorldMarket->DeleteTiingoStock(pTiingoStock);
			pStock->SetUpdateProfileDB(false);
			pStock->SetName(_T("Alcoa Corp"));
			EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());
			break;
		}
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());

		gl_pWorldMarket->SetTiingoStockSymbolUpdated(false);
		gl_pWorldMarket->SetFinnhubStockProfileUpdated(false);
	}
}