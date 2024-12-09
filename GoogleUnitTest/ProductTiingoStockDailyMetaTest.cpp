#include"pch.h"

 

#include"GeneralCheck.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include"ProductTiingoStockDailyMeta.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoStockDailyMetaTest : public Test {
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
		CProductTiingoStockDailyMeta stockDailyMeta;
	};

	TEST_F(CProductTiingoStockDailyMetaTest, TestInitialize) {
		EXPECT_EQ(stockDailyMeta.GetIndex(), 0);
		EXPECT_STREQ(stockDailyMeta.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/daily/"));
	}

	TEST_F(CProductTiingoStockDailyMetaTest, TestCreatMessage) {
		stockDailyMeta.SetMarket(gl_pWorldMarket);
		stockDailyMeta.SetIndex(1); // stock code is AA
		EXPECT_STREQ(stockDailyMeta.CreateMessage(), _T("https://api.tiingo.com/tiingo/daily/AA?"));
	}

	// 正确的数据
	Test_TiingoWebData tiingoStockDailyMeta1(1, _T(""), _T("{\"ticker\":\"AAPL\",\"name\":\"Apple Inc\",\"exchangeCode\":\"NASDAQ\",\"startDate\":\"1980-12-12\",\"endDate\":\"2019-01-25\",\"description\":\"Apple Inc.services, peripherals\"}"));
	// 日期皆为null
	Test_TiingoWebData tiingoStockDailyMeta2(2, _T(""), _T("{\"ticker\": \"AAPL\",\"name\" : \"Apple Inc\",\"exchangeCode\" : \"NASDAQ\",\"startDate\" : null,\"endDate\" : null,\"description\" : \"Apple Inc. (Apple) designs, manufactures and markets mobile\"}"));
	// 第二个数据缺项
	Test_TiingoWebData tiingoStockDailyMeta3(3, _T(""), _T("{\"ticker\": \"AAPL\",\"name\" : \"Apple Inc\",\"exchangeCode\" : \"NASDAQ\",\"startDate\" : \"1980-12-12\",\"endDate\" : \"2019-01-25\",\"description\" : \"Apple Inc. (Apple) desi\"}"));
	// 正确的数据
	Test_TiingoWebData tiingoStockDailyMeta4(4, _T(""), _T("{\"ticker\": \"AAPL\",\"name\" : \"Apple Inc\",\"exchangeCode\" : \"NASDAQ\",\"startDate\" : \"1980-12-12\",\"endDate\" : \"2019-01-25\",\"description\" : \"Apple Inc. (Apple) des\"}"));
	// 正确的数据
	Test_TiingoWebData tiingoStockDailyMeta10(10, _T(""), _T("{\"ticker\": \"AAPL\",\"name\" : \"Apple Inc\",\"exchangeCode\" : \"NASDAQ\",\"startDate\" : \"1980-12-12\",\"endDate\" : \"2019-01-25\",\"description\" : \"Apple Inc. (Apple) designs, manufactures\"}"));

	class ParseTiingoStockDailyMetaTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			auto pStock = gl_dataContainerTiingoStock.GetStock("AAPL");
			pStock->SetHistoryDayLineStartDate(19000101);
			pStock->SetHistoryDayLineEndDate(19000101);
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			auto pStock = gl_dataContainerTiingoStock.GetStock("AAPL");
			EXPECT_TRUE(pStock->IsUpdateStockDailyMeta());
			auto index = gl_dataContainerTiingoStock.GetOffset(pStock);
			m_tiingoStockDailyMetaProduct.SetIndex(index);
		}

		void TearDown() override {
			// clearUp
			auto pStock = gl_dataContainerTiingoStock.GetStock("AAPL");
			pStock->SetHistoryDayLineStartDate(19000101);
			pStock->SetHistoryDayLineEndDate(19000101);
			pStock->SetUpdateStockDailyMeta(true);
			pStock->SetUpdateProfileDB(false);

			gl_systemConfiguration.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoStocksPtr m_pvStock;
		CProductTiingoStockDailyMeta m_tiingoStockDailyMetaProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoStockDailyMeta,
	                         ParseTiingoStockDailyMetaTest,
	                         testing::Values(&tiingoStockDailyMeta1, &tiingoStockDailyMeta2,
		                         &tiingoStockDailyMeta3, &tiingoStockDailyMeta4, &tiingoStockDailyMeta10));

	TEST_P(ParseTiingoStockDailyMetaTest, TestParseStockDailyMeta) {
		m_tiingoStockDailyMetaProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(gl_dataContainerTiingoStock.GetStock("AAPL")->GetHistoryDayLineStartDate(), 19801212);
			EXPECT_EQ(gl_dataContainerTiingoStock.GetStock("AAPL")->GetHistoryDayLineEndDate(), 20190125);
			EXPECT_FALSE(gl_dataContainerTiingoStock.GetStock("AAPL")->IsUpdateStockDailyMeta());
			EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock("AAPL")->IsUpdateProfileDB());
			break;
		case 2: // 格式不对
			EXPECT_EQ(gl_dataContainerTiingoStock.GetStock("AAPL")->GetHistoryDayLineStartDate(), 19500101) << "当没有日期时，改为19500101";
			EXPECT_EQ(gl_dataContainerTiingoStock.GetStock("AAPL")->GetHistoryDayLineEndDate(), 19500101) << "当没有日期时，改为19500101";
			EXPECT_FALSE(gl_dataContainerTiingoStock.GetStock("AAPL")->IsUpdateStockDailyMeta());
			EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock("AAPL")->IsUpdateProfileDB());
			break;
		case 3: // 缺乏address项
			EXPECT_EQ(gl_dataContainerTiingoStock.GetStock("AAPL")->GetHistoryDayLineStartDate(), 19801212);
			EXPECT_EQ(gl_dataContainerTiingoStock.GetStock("AAPL")->GetHistoryDayLineEndDate(), 20190125);
			EXPECT_FALSE(gl_dataContainerTiingoStock.GetStock("AAPL")->IsUpdateStockDailyMeta());
			EXPECT_TRUE(gl_dataContainerTiingoStock.GetStock("AAPL")->IsUpdateProfileDB());
			break;
		case 4:
			break;
		case 10:
			break;
		default:
			break;
		}
	}

	class ProcessTiingoStockDailyMetaTest2 : public TestWithParam<Test_TiingoWebData*> {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			auto pStock = gl_dataContainerTiingoStock.GetStock("AAPL");
			pStock->SetHistoryDayLineStartDate(19000101);
			pStock->SetHistoryDayLineEndDate(19000101);
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoStockDailyMeta m_tiingoStockProduct;
		long long m_llTiingoBandWidthLeft;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockDailyMeta,
	                         ProcessTiingoStockDailyMetaTest2,
	                         testing::Values(&tiingoStockDailyMeta1, &tiingoStockDailyMeta2,
		                         &tiingoStockDailyMeta3, &tiingoStockDailyMeta4, &tiingoStockDailyMeta10));

	TEST_P(ProcessTiingoStockDailyMetaTest2, TestProcessStockDailyMeta) {
		auto pDailyMeta = m_tiingoStockProduct.ParseTiingoStockDailyMeta(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_STREQ(pDailyMeta->m_strCode, _T("AAPL"));
			EXPECT_STREQ(pDailyMeta->m_strName, _T("Apple Inc"));
			EXPECT_STREQ(pDailyMeta->m_strExchange, _T("NASDAQ"));
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineStartDate, 19801212);
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineEndDate, 20190125);
			break;
		case 2: // 日期皆为空null
			EXPECT_STREQ(pDailyMeta->m_strCode, _T("AAPL"));
			EXPECT_STREQ(pDailyMeta->m_strName, _T("Apple Inc"));
			EXPECT_STREQ(pDailyMeta->m_strExchange, _T("NASDAQ"));
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineStartDate, 19000101);
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineEndDate, 19000101);
			break;
		case 3: // 第二个数据缺乏address项,返回一个成功
			break;
		case 4:
			break;
		case 10:
			EXPECT_STREQ(pDailyMeta->m_strCode, _T("AAPL"));
			EXPECT_STREQ(pDailyMeta->m_strName, _T("Apple Inc"));
			EXPECT_STREQ(pDailyMeta->m_strExchange, _T("NASDAQ"));
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineStartDate, 19801212);
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineEndDate, 20190125);
		// 恢复原状
			break;
		default:
			break;
		}
	}
}
