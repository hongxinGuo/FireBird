#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubStock.h"
#include"TiingoDataSource.h"

#include"ProductTiingoIEXTopOfBook.h"
#include"TestWebData.h"

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
		EXPECT_STREQ(IEXTopOfBook.GetInquiringSymbol(), _T(""));
		EXPECT_STREQ(IEXTopOfBook.GetInquiryFunction(), _T("https://api.tiingo.com/iex?"));
	}

	TEST_F(CProductTiingoIEXTopOfBookTest, TestCreatMessage) {
		EXPECT_STREQ(IEXTopOfBook.CreateMessage(), IEXTopOfBook.GetInquiryFunction());
		EXPECT_STREQ(IEXTopOfBook.CreateMessage(), _T("https://api.tiingo.com/iex?"));
		EXPECT_STREQ(IEXTopOfBook.GetInquiringSymbol(), _T("All")) << "此时已被CreateMessage赋值";
	}

	TEST_F(CProductTiingoIEXTopOfBookTest, TestUpdateDataSourceStatus1) {
		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateIEXTopOfBook()) << "付费账户";
		gl_pTiingoDataSource->SetUpdateIEXTopOfBook(true);

		IEXTopOfBook.UpdateDataSourceStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateIEXTopOfBook());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage().c_str(), _T("Tiingo IEX top of book Updated"));

		// 恢复原状
		gl_pTiingoDataSource->SetUpdateIEXTopOfBook(false);
	}

	// 正确的数据
	Test_TiingoWebData tiingoIEXTopOfBook1(1, _T(""), _T("[{\"ticker\":\"000001\", \"timestamp\" : \"2024-09-30T20:00:00+00:00\", \"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"open\" : 11.6213, \"high\" : 12.3, \"low\" : 11.56, \"mid\" : null, \"tngoLast\" : 12.21, \"last\" : 12.21, \"lastSize\" : null, \"bidSize\" : null, \"bidPrice\" : null, \"askPrice\" : null, \"askSize\" : null, \"volume\" : 543028360, \"prevClose\" : 11.42}, { \"ticker\":\"000002\",\"timestamp\" : \"2024-03-30T20:00:00+00:00\",\"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"open\" : 9.72,\"high\" : 9.72,\"low\" : 9.27,\"mid\" : null,\"tngoLast\" : 9.72,\"last\" : 9.72,\"lastSize\" : null,\"bidSize\" : null,\"bidPrice\" : null,\"askPrice\" : null,\"askSize\" : null,\"volume\" : 579726570,\"prevClose\" : 8.84 }]"));
	// 第一个数据缺项
	Test_TiingoWebData tiingoIEXTopOfBook2(2,_T(""), _T("[{\"ticker\":\"000001\", \"timestamp\" : \"2024-09-30T20:00:00+00:00\", \"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"open\" : 11.62, \"high\" : 12.3, \"low\" : 11.56, \"mid\" : null, \"tngoLast\" : 12.21, \"last\" : 12.21, \"lastSize\" : null, \"bidSize\" : null, \"bidPrice\" : null, \"askPrice\" : null, \"askSize\" : null, \"volume\" : 543028360, \"prevClose\" : 11.42}, { \"ticker\":\"000002\",\"timestamp\" : \"2024-09-30T20:00:00+00:00\",\"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"open\" : 9.72,\"high\" : 9.72,\"low\" : 9.27,\"mid\" : null,\"tngoLast\" : 9.72,\"last\" : 9.72,\"lastSize\" : null,\"bidSize\" : null,\"bidPrice\" : null,\"askPrice\" : null,\"askSize\" : null,\"volume\" : 579726570,\"prevClose\" : 8.84 }]"));
	// 第二个数据缺项
	Test_TiingoWebData tiingoIEXTopOfBook3(3, _T(""), _T("[{\"ticker\":\"000001\", \"timestamp\" : \"2024-09-30T20:00:00+00:00\", \"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"open\" : 11.62, \"high\" : 12.3, \"low\" : 11.56, \"mid\" : null, \"tngoLast\" : 12.21, \"last\" : 12.21, \"lastSize\" : null, \"bidSize\" : null, \"bidPrice\" : null, \"askPrice\" : null, \"askSize\" : null, \"volume\" : 543028360, \"prevClose\" : 11.42}, { \"ticker\":\"000002\",\"timestamp\" : \"2024-09-30T20:00:00+00:00\",\"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"open\" : 9.72,\"high\" : 9.72,\"low\" : 9.27,\"mid\" : null,\"tngoLast\" : 9.72,\"last\" : 9.72,\"lastSize\" : null,\"bidSize\" : null,\"bidPrice\" : null,\"askPrice\" : null,\"askSize\" : null,\"volume\" : 579726570,\"prevClose\" : 8.84 }]"));
	// 正确的数据
	Test_TiingoWebData tiingoIEXTopOfBook4(4, _T(""), _T("[{\"ticker\":\"000001\", \"timestamp\" : \"2024-09-30T20:00:00+00:00\", \"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"open\" : 11.62, \"high\" : 12.3, \"low\" : 11.56, \"mid\" : null, \"tngoLast\" : 12.21, \"last\" : 12.21, \"lastSize\" : null, \"bidSize\" : null, \"bidPrice\" : null, \"askPrice\" : null, \"askSize\" : null, \"volume\" : 543028360, \"prevClose\" : 11.42}, { \"ticker\":\"000002\",\"timestamp\" : \"2024-09-30T20:00:00+00:00\",\"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"open\" : 9.72,\"high\" : 9.72,\"low\" : 9.27,\"mid\" : null,\"tngoLast\" : 9.72,\"last\" : 9.72,\"lastSize\" : null,\"bidSize\" : null,\"bidPrice\" : null,\"askPrice\" : null,\"askSize\" : null,\"volume\" : 579726570,\"prevClose\" : 8.84 }]"));
	// 正确的数据 DOW30
	Test_TiingoWebData tiingoIEXTopOfBook10(10, _T(""), _T("[{\"ticker\":\"IBM\", \"timestamp\" : \"2024-09-30T20:00:00+00:00\", \"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\", \"open\" : 11.62, \"high\" : 12.3, \"low\" : 11.56, \"mid\" : null, \"tngoLast\" : 12.21, \"last\" : 12.21, \"lastSize\" : null, \"bidSize\" : null, \"bidPrice\" : null, \"askPrice\" : null, \"askSize\" : null, \"volume\" : 543028360, \"prevClose\" : 11.42}, { \"ticker\":\"MSFT\",\"timestamp\" : \"2024-09-30T20:00:00+00:00\",\"lastSaleTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"quoteTimestamp\" : \"2024-09-30T20:00:00+00:00\",\"open\" : 9.72,\"high\" : 9.72,\"low\" : 9.27,\"mid\" : null,\"tngoLast\" : 9.72,\"last\" : 9.72,\"lastSize\" : null,\"bidSize\" : null,\"bidPrice\" : null,\"askPrice\" : null,\"askSize\" : null,\"volume\" : 579726570,\"prevClose\" : 8.84 }]"));

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
			EXPECT_STREQ(m_pvIEXTopOfBook->at(0)->m_strTicker, _T("000001"));
			EXPECT_STREQ(m_pvIEXTopOfBook->at(1)->m_strTicker, _T("000002"));
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_timeStamp.time_since_epoch().count(), 1727726400);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lLastClose, 11420000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lOpen, 11621300);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lHigh, 12300000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lLow, 11560000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_lNew, 12210000);
			EXPECT_EQ(m_pvIEXTopOfBook->at(0)->m_llVolume, 543028360);
			EXPECT_EQ(m_pvIEXTopOfBook->at(1)->m_timeStamp.time_since_epoch().count(), 1711828800);
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

	//todo
	/*
	TEST_P(ParseTiingoIEXTopOfBookTest, TestParseAndStoreWebData) {
		m_tiingoIEXTopOfBookProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确的数据
			EXPECT_EQ(m_pvIEXTopOfBook->size(), 2);
			EXPECT_STREQ(m_pvIEXTopOfBook->at(0)->m_strTicker, _T("000001"));
			EXPECT_STREQ(m_pvIEXTopOfBook->at(1)->m_strTicker, _T("000002"));
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
