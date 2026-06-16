#include"pch.h"

#include"GeneralCheck.h"
#include"TiingoDataSource.h"

#include"ProductTiingoIEXTopOfBook.h"
#include"TestWebData.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoIEXTopOfBookTest : public Test {
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
		CProductTiingoIEXTopOfBook IEXTopOfBook;
	};

	TEST_F(CProductTiingoIEXTopOfBookTest, TestInitialize) {
		EXPECT_EQ(IEXTopOfBook.GetIndex(), 0);
		EXPECT_EQ(IEXTopOfBook.GetInquiringSymbol(), "");
		EXPECT_EQ(IEXTopOfBook.GetInquiryFunction(), "https://api.tiingo.com/iex?");
	}

	TEST_F(CProductTiingoIEXTopOfBookTest, TestCreatMessage) {
		EXPECT_EQ(IEXTopOfBook.CreateMessage(), IEXTopOfBook.GetInquiryFunction());
		EXPECT_EQ(IEXTopOfBook.CreateMessage(), "https://api.tiingo.com/iex?");
		EXPECT_EQ(IEXTopOfBook.GetInquiringSymbol(), "All") << "此时已被CreateMessage赋值";
	}

	TEST_F(CProductTiingoIEXTopOfBookTest, TestUpdateDataSourceStatus1) {
		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateIEXTopOfBook()) << "付费账户";
		gl_pTiingoDataSource->SetUpdateIEXTopOfBook(true);

		IEXTopOfBook.UpdateSystemStatus();

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateIEXTopOfBook());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), "Tiingo IEX top of book Updated");

		// 恢复原状
		gl_pTiingoDataSource->SetUpdateIEXTopOfBook(false);
	}

	TEST_F(CProductTiingoIEXTopOfBookTest, ParsesAndStoresRTDataForExistingStocks) {
		// Ensure expected symbols exist in the preloaded test data
		ASSERT_TRUE(gl_dataContainerTiingoStock.IsSymbol("IBM")) << "Test requires 'IBM' present in Tiingo symbol container";
		ASSERT_TRUE(gl_dataContainerTiingoStock.IsSymbol("MSFT")) << "Test requires 'MSFT' present in Tiingo symbol container";

		// Move market current trade date earlier so the sample timestamps are considered recent (timestamp > st)
		gl_pWorldMarket->TEST_SetMarketDate(toLocalDays(20240101));

		// Prepare web payload with two entries (IBM and MSFT) — numbers chosen to match existing parse expectations
		const string payload = R"([{"ticker":"IBM", "timestamp" : "2024-09-30T20:00:00+00:00", "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00", "quoteTimestamp" : "2024-09-30T20:00:00+00:00", "open" : 11.62, "high" : 12.3, "low" : 11.56, "mid" : null, "tngoLast" : 12.21, "last" : 12.21, "lastSize" : null, "bidSize" : null, "bidPrice" : null, "askPrice" : null, "askSize" : null, "volume" : 543028360, "prevClose" : 11.42}, { "ticker":"MSFT","timestamp" : "2024-09-30T20:00:00+00:00","lastSaleTimestamp" : "2024-09-30T20:00:00+00:00","quoteTimestamp" : "2024-09-30T20:00:00+00:00","open" : 9.72,"high" : 9.72,"low" : 9.27,"mid" : null,"tngoLast" : 9.72,"last" : 9.72,"lastSize" : null,"bidSize" : null,"bidPrice" : null,"askPrice" : null,"askSize" : null,"volume" : 579726570,"prevClose" : 8.84 }])";

		auto pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(payload);

		// Clear any prior RT values in target stocks
		auto pIBM = gl_dataContainerTiingoStock.GetStock("IBM");
		auto pMSFT = gl_dataContainerTiingoStock.GetStock("MSFT");
		pIBM->SetNew(0);
		pIBM->SetVolume(0);
		pIBM->SetTimePoint(chrono::sys_seconds{});
		pMSFT->SetNew(0);
		pMSFT->SetVolume(0);
		pMSFT->SetTimePoint(chrono::sys_seconds{});

		// Exercise the routine under test
		IEXTopOfBook.ParseAndStoreWebData(pWebData);

		// Verify IBM updated
		EXPECT_EQ(pIBM->GetNew(), 12210000) << "IBM latest price (scaled by ratio)";
		EXPECT_EQ(pIBM->GetVolume(), 543028360LL) << "IBM volume";
		EXPECT_EQ(pIBM->GetTransactionTime(), 1727726400) << "IBM timestamp (epoch seconds)";

		// Verify MSFT updated
		EXPECT_EQ(pMSFT->GetNew(), 9720000) << "MSFT latest price (9.72 * 1e6)";
		EXPECT_EQ(pMSFT->GetVolume(), 579726570LL) << "MSFT volume";
		EXPECT_EQ(pMSFT->GetTransactionTime(), 1727726400) << "MSFT timestamp (epoch seconds)";

		EXPECT_FALSE(gl_pWorldMarket->IsMarketTaskEmpty());
		EXPECT_EQ(gl_pWorldMarket->GetMarketTask()->GetType(), WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__);
		gl_pWorldMarket->DiscardCurrentMarketTask();
		EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty());

		// 恢复原状
		gl_pWorldMarket->TEST_SetMarketDate(s_currentWorldMarketDate);
	}

	namespace {
		// 正确的数据
		Test_TiingoWebData tiingoIEXTopOfBook1(1, "", R"([{"ticker":"000001", "timestamp" : "2024-09-30T20:00:00+00:00", "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00", "quoteTimestamp" : "2024-09-30T20:00:00+00:00", "open" : 11.6213, "high" : 12.3, "low" : 11.56, "mid" : null, "tngoLast" : 12.21, "last" : 12.21, "lastSize" : null, "bidSize" : null, "bidPrice" : null, "askPrice" : null, "askSize" : null, "volume" : 543028360, "prevClose" : 11.42}, { "ticker":"000002","timestamp" : "2024-03-30T20:00:00+00:00","lastSaleTimestamp" : "2024-09-30T20:00:00+00:00","quoteTimestamp" : "2024-09-30T20:00:00+00:00","open" : 9.72,"high" : 9.72,"low" : 9.27,"mid" : null,"tngoLast" : 9.72,"last" : null,"lastSize" : null,"bidSize" : null,"bidPrice" : null,"askPrice" : null,"askSize" : null,"volume" : 579726570,"prevClose" : 8.84 }])");
		// 第一个数据缺项
		Test_TiingoWebData tiingoIEXTopOfBook2(2, "", R"([{"ticker":"000001", "timestamp" : "2024-09-30T20:00:00+00:00", "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00", "quoteTimestamp" : "2024-09-30T20:00:00+00:00", "open" : 11.62, "high" : 12.3, "low" : 11.56, "mid" : null, "tngoLast" : 12.21, "last" : 12.21, "lastSize" : null, "bidSize" : null, "bidPrice" : null, "askPrice" : null, "askSize" : null, "volume" : 543028360, "prevClose" : 11.42}, { "ticker":"000002","timestamp" : "2024-09-30T20:00:00+00:00","lastSaleTimestamp" : "2024-09-30T20:00:00+00:00","quoteTimestamp" : "2024-09-30T20:00:00+00:00","open" : 9.72,"high" : 9.72,"low" : 9.27,"mid" : null,"tngoLast" : 9.72,"last" : 9.72,"lastSize" : null,"bidSize" : null,"bidPrice" : null,"askPrice" : null,"askSize" : null,"volume" : 579726570,"prevClose" : 8.84 }])");
		// 第二个数据缺项
		Test_TiingoWebData tiingoIEXTopOfBook3(3, "", R"([{"ticker":"000001", "timestamp" : "2024-09-30T20:00:00+00:00", "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00", "quoteTimestamp" : "2024-09-30T20:00:00+00:00", "open" : 11.62, "high" : 12.3, "low" : 11.56, "mid" : null, "tngoLast" : 12.21, "last" : 12.21, "lastSize" : null, "bidSize" : null, "bidPrice" : null, "askPrice" : null, "askSize" : null, "volume" : 543028360, "prevClose" : 11.42}, { "ticker":"000002","timestamp" : "2024-09-30T20:00:00+00:00","lastSaleTimestamp" : "2024-09-30T20:00:00+00:00","quoteTimestamp" : "2024-09-30T20:00:00+00:00","open" : 9.72,"high" : 9.72,"low" : 9.27,"mid" : null,"tngoLast" : 9.72,"last" : 9.72,"lastSize" : null,"bidSize" : null,"bidPrice" : null,"askPrice" : null,"askSize" : null,"volume" : 579726570,"prevClose" : 8.84 }])");
		// 正确的数据
		Test_TiingoWebData tiingoIEXTopOfBook4(4, "", R"([{"ticker":"000001", "timestamp" : "2024-09-30T20:00:00+00:00", "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00", "quoteTimestamp" : "2024-09-30T20:00:00+00:00", "open" : 11.62, "high" : 12.3, "low" : 11.56, "mid" : null, "tngoLast" : 12.21, "last" : 12.21, "lastSize" : null, "bidSize" : null, "bidPrice" : null, "askPrice" : null, "askSize" : null, "volume" : 543028360, "prevClose" : 11.42}, { "ticker":"000002","timestamp" : "2024-09-30T20:00:00+00:00","lastSaleTimestamp" : "2024-09-30T20:00:00+00:00","quoteTimestamp" : "2024-09-30T20:00:00+00:00","open" : 9.72,"high" : 9.72,"low" : 9.27,"mid" : null,"tngoLast" : 9.72,"last" : 9.72,"lastSize" : null,"bidSize" : null,"bidPrice" : null,"askPrice" : null,"askSize" : null,"volume" : 579726570,"prevClose" : 8.84 }])");
		// 正确的数据 DOW30
		Test_TiingoWebData tiingoIEXTopOfBook10(10, "", R"([{"ticker":"IBM", "timestamp" : "2024-09-30T20:00:00+00:00", "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00", "quoteTimestamp" : "2024-09-30T20:00:00+00:00", "open" : 11.62, "high" : 12.3, "low" : 11.56, "mid" : null, "tngoLast" : 12.21, "last" : 12.21, "lastSize" : null, "bidSize" : null, "bidPrice" : null, "askPrice" : null, "askSize" : null, "volume" : 543028360, "prevClose" : 11.42}, { "ticker":"MSFT","timestamp" : "2024-09-30T20:00:00+00:00","lastSaleTimestamp" : "2024-09-30T20:00:00+00:00","quoteTimestamp" : "2024-09-30T20:00:00+00:00","open" : 9.72,"high" : 9.72,"low" : 9.27,"mid" : null,"tngoLast" : 9.72,"last" : null,"lastSize" : null,"bidSize" : null,"bidPrice" : null,"askPrice" : null,"askSize" : null,"volume" : 579726570,"prevClose" : 8.84 }])");
	}

	class ParseTiingoIEXTopOfBookTest : public TestWithParam<Test_TiingoWebData*> {
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
		CTiingoIEXTopOfBooksPtr m_pvIEXTopOfBook;
		CProductTiingoIEXTopOfBook m_tiingoIEXTopOfBookProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoIEXTopOfBook,
	                         ParseTiingoIEXTopOfBookTest,
	                         testing::Values(&tiingoIEXTopOfBook1, &tiingoIEXTopOfBook2,
		                         &tiingoIEXTopOfBook3, &tiingoIEXTopOfBook4, &tiingoIEXTopOfBook10));

	TEST_P(ParseTiingoIEXTopOfBookTest, TestParseTiingoIEXTopOfBook) {
		m_pvIEXTopOfBook = m_tiingoIEXTopOfBookProduct.ParseTiingoIEXTopOfBook(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确的数据
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_strTicker, "000001");
			EXPECT_EQ(m_pvIEXTopOfBook->at(1).m_strTicker, "000002");
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_timeStamp.time_since_epoch().count(), 1727726400);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_lLastClose, 11420000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_lOpen, 11621300);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_lHigh, 12300000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_lLow, 11560000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_lNew, 12210000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0).m_llVolume, 543028360);
			EXPECT_EQ(m_pvIEXTopOfBook->at(1).m_timeStamp.time_since_epoch().count(), 1711828800);
			EXPECT_EQ(m_pvIEXTopOfBook->at(1).m_lNew, 9720000) << "Last项为null，使用tngoLast项的值";
			EXPECT_EQ(m_pvIEXTopOfBook->at(1).m_lLow, 9270000);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		case 3: // 缺乏address项
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		case 4:
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		case 10:
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		default:
			break;
		}
	}

	//Todo:
	/*
	TEST_P(ParseTiingoIEXTopOfBookTest, TestParseAndStoreWebData) {
		m_tiingoIEXTopOfBookProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确的数据
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			EXPECT_STREQ(m_pvIEXTopOfBook->at(0)->m_strTicker, "000001");
			EXPECT_STREQ(m_pvIEXTopOfBook->at(1)->m_strTicker, "000002");
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_llTimestamp, 1727726400);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lLastClose, 11420000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lOpen, 11620000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lHigh, 1230000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lLow, 11560000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lNew, 12210000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_llVolume, 543028360);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		case 3: // 缺乏address项
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		case 4:
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		case 10:
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			break;
		default:
			break;
		}
	}
	*/
}
