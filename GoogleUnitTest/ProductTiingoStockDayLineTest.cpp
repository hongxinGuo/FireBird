#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductTiingoStockDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoStockDayLineTest : public Test {
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
		CProductTiingoStockDayLine stockPriceCandle;
	};

	TEST_F(CProductTiingoStockDayLineTest, TestInitialize) {
		EXPECT_EQ(stockPriceCandle.GetIndex(), -1);
		EXPECT_STREQ(stockPriceCandle.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/daily/"));
	}

	TEST_F(CProductTiingoStockDayLineTest, TestCreatMessage1) {
		stockPriceCandle.SetIndex(0); // 测试数据库中，此股票代码为000001.SS
		stockPriceCandle.SetMarket(gl_pWorldMarket);
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(gl_dataContainerTiingoStock.GetStock(0)->GetSymbol());
		pStock->SetDayLineStartDate(20171231); // 早于20180101
		const CString strMessage = stockPriceCandle.CreateMessage();
		const long lMarketDate = gl_pWorldMarket->GetMarketDate();
		const long year = lMarketDate / 10000;
		const long month = lMarketDate / 100 - year * 100;
		const long day = lMarketDate - year * 10000 - month * 100;
		char buffer[30];
		sprintf_s(buffer, _T("%4d-%d-%d"), year, month, day);
		const CString strEndDate = buffer;
		CString strMarketDate = gl_pWorldMarket->GetStringOfMarketDate();
		const CString strTest = _T("https://api.tiingo.com/tiingo/daily/A/prices?&startDate=1980-1-1&endDate=") + strEndDate;

		EXPECT_STREQ(strMessage, strTest) << "当股票日线的起始日期早于20180101时，使用之前的结束日期为申请数据的起始日期";
	}

	TEST_F(CProductTiingoStockDayLineTest, TestCreatMessage2) {
		stockPriceCandle.SetIndex(0); // 测试数据库中，此股票代码为000001.SS
		stockPriceCandle.SetMarket(gl_pWorldMarket);
		const CWorldStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(gl_dataContainerTiingoStock.GetStock(0)->GetSymbol());
		pStock->SetDayLineStartDate(20171231); // 早于20180101
		const CString strMessage = stockPriceCandle.CreateMessage();
		const long lMarketDate = gl_pWorldMarket->GetMarketDate();
		const long year = lMarketDate / 10000;
		const long month = lMarketDate / 100 - year * 100;
		const long day = lMarketDate - year * 10000 - month * 100;
		char buffer[30];
		sprintf_s(buffer, _T("%4d-%d-%d"), year, month, day);
		const CString strEndDate = buffer;
		CString strMarketDate = gl_pWorldMarket->GetStringOfMarketDate();
		const CString strTest = _T("https://api.tiingo.com/tiingo/daily/A/prices?&startDate=1980-1-1&endDate=") + strEndDate;
		EXPECT_STREQ(strMessage, strTest);
	}

	TEST_F(CProductTiingoStockDayLineTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	Test_TiingoWebData tiingoWebData31(1, _T("AAPL"), _T("[\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有date项
	Test_TiingoWebData tiingoWebData32(2, _T("AAPL"), _T("[{\"Missing\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有close项
	Test_TiingoWebData tiingoWebData33(3, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"Missing\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有high项
	Test_TiingoWebData tiingoWebData35(5, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"Missing\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有low项
	Test_TiingoWebData tiingoWebData36(6, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"Missing\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有open项
	Test_TiingoWebData tiingoWebData37(7, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"Missing\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有volume项
	Test_TiingoWebData tiingoWebData38(8, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"Missing\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第二个数据没有date项
	Test_TiingoWebData tiingoWebData39(9, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"Missing\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 正确的数据
	Test_TiingoWebData tiingoWebData40(10, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 股票没有日线数据
	Test_TiingoWebData tiingoWebData41(11, _T("AAPL"), _T("{\"detail\":\"Error:Ticker 'AAPL' not found\"}"));

	class ParseTiingoStockDayLineTest : public TestWithParam<Test_TiingoWebData*> {
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
		CProductTiingoStockDayLine m_tiingoStockPriceCandle;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoStockDayLine1, ParseTiingoStockDayLineTest,
	                         testing::Values(&tiingoWebData31, &tiingoWebData32, &tiingoWebData33, &tiingoWebData35,
		                         &tiingoWebData36, &tiingoWebData37, &tiingoWebData38, &tiingoWebData39, &tiingoWebData40,
		                         &tiingoWebData41));

	TEST_P(ParseTiingoStockDayLineTest, TestParseTiingoStockDayLine0) {
		CDayLinesPtr pvDayLine;
		CString strMessage;
		CDayLinePtr pDayLine;

		pvDayLine = m_tiingoStockPriceCandle.ParseTiingoStockDayLine(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 2: //
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 3: //
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 5:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 6:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 7:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 8:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 9:
			EXPECT_EQ(pvDayLine->size(), 1);
			break;
		case 10:
			EXPECT_EQ(pvDayLine->size(), 2);
			pDayLine = pvDayLine->at(0);
			EXPECT_EQ(pDayLine->GetMarketDate(), 20210311);
			EXPECT_EQ(pDayLine->GetClose(), 121960);
			EXPECT_EQ(pDayLine->GetHigh(), 123210);
			EXPECT_EQ(pDayLine->GetLow(), 121260);
			EXPECT_EQ(pDayLine->GetOpen(), 122540);
			EXPECT_EQ(pDayLine->GetVolume(), 103026514);
			break;
		case 11: // 股票没有日线数据
			EXPECT_EQ(pvDayLine->size(), 0);
			EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1) << "函数报告错误信息";
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Tiingo stock dayLine Error:Ticker 'AAPL' not found"));
			break;
		default:
			break;
		}
	}

	class ParseTiingoStockDayLineTest2 : public TestWithParam<Test_TiingoWebData*> {
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
		CProductTiingoStockDayLine m_tiingoStockPriceCandle;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoStockDayLine1, ParseTiingoStockDayLineTest2,
	                         testing::Values(&tiingoWebData31, &tiingoWebData32, &tiingoWebData33, &tiingoWebData35,
		                         &tiingoWebData36, &tiingoWebData37, &tiingoWebData38, &tiingoWebData39, &tiingoWebData40,
		                         &tiingoWebData41));

	TEST_P(ParseTiingoStockDayLineTest2, TestParseTiingoStockDayLine0) {
		CDayLinesPtr pvDayLine;
		CString strMessage;
		CDayLinePtr pDayLine;

		pvDayLine = m_tiingoStockPriceCandle.ParseTiingoStockDayLine(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 2: //
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 3: //
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 5:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 6:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 7:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 8:
			EXPECT_EQ(pvDayLine->size(), 0);
			break;
		case 9:
			EXPECT_EQ(pvDayLine->size(), 1);
			break;
		case 10:
			EXPECT_EQ(pvDayLine->size(), 2);
			pDayLine = pvDayLine->at(0);
			EXPECT_EQ(pDayLine->GetMarketDate(), 20210311);
			EXPECT_EQ(pDayLine->GetClose(), 121960);
			EXPECT_EQ(pDayLine->GetHigh(), 123210);
			EXPECT_EQ(pDayLine->GetLow(), 121260);
			EXPECT_EQ(pDayLine->GetOpen(), 122540);
			EXPECT_EQ(pDayLine->GetVolume(), 103026514);
			break;
		case 11: // 股票没有日线数据
			EXPECT_EQ(pvDayLine->size(), 0);
			EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1) << "函数报告错误信息";
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Tiingo stock dayLine Error:Ticker 'AAPL' not found"));
			break;
		default:
			break;
		}
	}

	class ProcessTiingoStockDayLineTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			m_tiingoStockPriceCandle.SetMarket(gl_pWorldMarket);
			m_tiingoStockPriceCandle.SetIndex(0); // Tiingo stock index
		}

		void TearDown() override {
			// clearUp
			gl_systemConfiguration.SetTiingoBandWidthLeft(5368709120);
			gl_systemConfiguration.SetUpdateDB(false);
			gl_dataContainerTiingoStock.GetStock(_T("A"))->SetUpdateProfileDB(false);
			gl_dataContainerTiingoStock.GetStock(_T("AAPL"))->SetUpdateProfileDB(false);
			auto symbol = gl_dataContainerTiingoStock.GetStock(0)->GetSymbol();
			gl_dataContainerFinnhubStock.GetStock(symbol)->SetUpdateDayLine(false);
			gl_dataContainerFinnhubStock.GetStock(symbol)->SetUpdateDayLineDB(false);
			gl_dataContainerFinnhubStock.GetStock(symbol)->SetUpdateProfileDB(false);
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoStockDayLine m_tiingoStockPriceCandle;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockDayLine, ProcessTiingoStockDayLineTest,
	                         testing::Values(&tiingoWebData31, &tiingoWebData32, &tiingoWebData33, &tiingoWebData35,
		                         &tiingoWebData36, &tiingoWebData37, &tiingoWebData38, &tiingoWebData39, &tiingoWebData40));

	TEST_P(ProcessTiingoStockDayLineTest, TestProcessTiingoStockDayLine) {
		CDayLinesPtr pvDayLine;
		CString strMessage;
		CDayLinePtr pDayLine;
		CTiingoStockPtr pStock = gl_dataContainerTiingoStock.GetStock(0); // 这个是当前处理的股票

		m_tiingoStockPriceCandle.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			break;
		case 2: //
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			break;
		case 3: //
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			break;
		case 5:
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			break;
		case 6:
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			break;
		case 7:
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			break;
		case 8:
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			break;
		case 9:
			EXPECT_EQ(pStock->GetDayLineSize(), 1);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_TRUE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_EQ(pStock->GetDayLineSize(), 2);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_TRUE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		default:
			break;
		}
		// 恢复原状
		pStock->UnloadDayLine();
	}
}
