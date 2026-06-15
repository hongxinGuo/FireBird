#include"pch.h"

#include"GeneralCheck.h"
#include"WorldMarket.h"
#include"ProductTiingoStockDayLine.h"
#include"TestWebData.h"

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
		EXPECT_EQ(stockPriceCandle.GetIndex(), 0);
		EXPECT_EQ(stockPriceCandle.GetInquiryFunction(), "https://api.tiingo.com/tiingo/daily/");
	}

	TEST_F(CProductTiingoStockDayLineTest, TestGetTiingoDayLineInquiryParam) {
		const string strParam = "600601.SS/prices?&startDate=2018-01-01&endDate=2020-01-01";

		EXPECT_TRUE(stockPriceCandle.GetDayLineInquiryParam("600601.SS", toLocalDays(20180101), toLocalDays(20200101)) == strParam);
	}

	TEST_F(CProductTiingoStockDayLineTest, TestGetTiingoDayLineInquiryParam2) {
		const string strParam = "600601.SS/prices?&startDate=1980-01-01&endDate=2020-01-01";

		EXPECT_TRUE(stockPriceCandle.GetDayLineInquiryParam("600601.SS", toLocalDays(19800101), toLocalDays(20200101)) == strParam);
	}

	TEST_F(CProductTiingoStockDayLineTest, TestCreatMessage1) {
		stockPriceCandle.SetIndex(0); // 测试数据库中，此股票代码为000001.SS
		const auto pStock = gl_dataContainerTiingoStock.GetStock(gl_dataContainerTiingoStock.GetStock(0)->GetSymbol());
		pStock->SetDayLineEndDate(chrono::local_days(chrono::days(0)));
		const string strMessage = stockPriceCandle.CreateMessage();
		const chrono::local_days lMarketDate = gl_pWorldMarket->GetMarketDate();
		string sEndDate = std::format("{:%F}", lMarketDate);
		string sMarketDate = gl_pWorldMarket->GetStringOfMarketDate();
		const string sTest = "https://api.tiingo.com/tiingo/daily/A/prices?&startDate=1980-01-01&endDate=" + sEndDate;

		EXPECT_TRUE(strMessage == sTest) << "使用之前的结束日期为申请数据的起始日期";
	}

	TEST_F(CProductTiingoStockDayLineTest, TestCreatMessage2) {
		stockPriceCandle.SetIndex(0); // 测试数据库中，此股票代码为000001.SS
		const auto pStock = gl_dataContainerTiingoStock.GetStock(gl_dataContainerTiingoStock.GetStock(0)->GetSymbol());
		pStock->SetDayLineEndDate(chrono::local_days(chrono::days(0))); // 早于20180101
		const string strMessage = stockPriceCandle.CreateMessage();
		const chrono::local_days lMarketDate = gl_pWorldMarket->GetMarketDate();
		string sEndDate = std::format("{:%F}", lMarketDate);
		string sMarketDate = gl_pWorldMarket->GetStringOfMarketDate();
		string strTest = "https://api.tiingo.com/tiingo/daily/A/prices?&startDate=1980-01-01&endDate=";
		strTest += sEndDate;
		EXPECT_TRUE(strMessage == strTest);
	}

	TEST_F(CProductTiingoStockDayLineTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	namespace {
		// 格式不对(缺开始的‘{’），无法顺利Parser
		Test_TiingoWebData tiingoWebData31(1, "AAPL", R"(["date":"2021-03-11T00:00:00.000Z","close":121.96,"high":123.21,"low":121.26,"open":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 第一个数据没有date项
		Test_TiingoWebData tiingoWebData32(2, "AAPL", R"([{"Missing":"2021-03-11T00:00:00.000Z","close":121.96,"high":123.21,"low":121.26,"open":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 第一个数据没有close项
		Test_TiingoWebData tiingoWebData33(3, "AAPL", R"([{"date":"2021-03-11T00:00:00.000Z","Missing":121.96,"high":123.21,"low":121.26,"open":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 第一个数据没有high项
		Test_TiingoWebData tiingoWebData35(5, "AAPL", R"([{"date":"2021-03-11T00:00:00.000Z","close":121.96,"Missing":123.21,"low":121.26,"open":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 第一个数据没有low项
		Test_TiingoWebData tiingoWebData36(6, "AAPL", R"([{"date":"2021-03-11T00:00:00.000Z","close":121.96,"high":123.21,"Missing":121.26,"open":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 第一个数据没有open项
		Test_TiingoWebData tiingoWebData37(7, "AAPL", R"([{"date":"2021-03-11T00:00:00.000Z","close":121.96,"high":123.21,"low":121.26,"Missing":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 第一个数据没有volume项
		Test_TiingoWebData tiingoWebData38(8, "AAPL", R"([{"date":"2021-03-11T00:00:00.000Z","close":121.96,"high":123.21,"low":121.26,"open":122.54,"Missing":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 第二个数据没有date项
		Test_TiingoWebData tiingoWebData39(9, "AAPL", R"([{"date":"2021-03-11T00:00:00.000Z","close":121.96,"high":123.21,"low":121.26,"open":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"Missing":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":0.0,"splitFactor":1.0}])");
		// 正确的数据
		Test_TiingoWebData tiingoWebData40(10, "AAPL", R"([{"date":"2021-03-11T00:00:00.000Z","close":121.96,"high":123.21,"low":121.26,"open":122.54,"volume":103026514,"adjClose":121.96,"adjHigh":123.21,"adjLow":121.26,"adjOpen":122.54,"adjVolume":103026514,"divCash":0.0,"splitFactor":1.0}, {"date":"2021-03-12T00:00:00.000Z","close":121.03,"high":121.17,"low":119.16,"open":120.4,"volume":88105050,"adjClose":121.03,"adjHigh":121.17,"adjLow":119.16,"adjOpen":120.4,"adjVolume":88105050,"divCash":1.0,"splitFactor":3.0}])");
		// 股票没有日线数据
		Test_TiingoWebData tiingoWebData41(11, "AAPL", R"({"detail":"Error:Ticker 'AAPL' not found"})");
	}

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
		CTiingoCandleLinesPtr pvDayLine;
		CTiingoCandleLine dayLine;

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
			dayLine = pvDayLine->at(0);
			EXPECT_EQ(dayLine.GetDate(), toLocalDays(20210311));
			EXPECT_EQ(dayLine.GetClose(), 121960000);
			EXPECT_EQ(dayLine.GetHigh(), 123210000);
			EXPECT_EQ(dayLine.GetLow(), 121260000);
			EXPECT_EQ(dayLine.GetOpen(), 122540000);
			EXPECT_EQ(dayLine.GetVolume(), 103026514);
			EXPECT_DOUBLE_EQ(dayLine.GetDividend(), 0);
			EXPECT_DOUBLE_EQ(dayLine.GetSplitFactor(), 1.0);

			dayLine = pvDayLine->at(1);
			EXPECT_DOUBLE_EQ(dayLine.GetDividend(), 1.0);
			EXPECT_DOUBLE_EQ(dayLine.GetSplitFactor(), 3.0);
			EXPECT_EQ(dayLine.GetDate(), toLocalDays(20210312));
			EXPECT_EQ(dayLine.GetClose(), 121030000);
			break;
		case 11: // 股票没有日线数据
			EXPECT_EQ(pvDayLine->size(), 0);
			EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1) << "函数报告错误信息";
			EXPECT_EQ(gl_systemMessage.PopErrorMessage(), "Tiingo stock dayLine Error:Ticker 'AAPL' not found");
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

			m_tiingoStockPriceCandle.SetIndex(0); // Tiingo stock index
		}

		void TearDown() override {
			// clearUp
			gl_systemConfiguration.SetUpdateDB(false);
			gl_dataContainerTiingoStock.GetStock("AAPL")->SetUpdateProfileDB(false);
			gl_dataContainerTiingoStock.GetStock(0)->SetUpdateDayLine(true);
			gl_dataContainerTiingoStock.GetStock(0)->SetUpdateDayLineDB(false);
			gl_dataContainerTiingoStock.GetStock(0)->SetUpdateProfileDB(false);
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
		CDayLinePtr pDayLine;
		CTiingoStockPtr pStock = gl_dataContainerTiingoStock.GetStock(0); // 这个是当前处理的股票

		m_tiingoStockPriceCandle.ParseAndStoreWebData(m_pWebData);
		EXPECT_EQ(pStock->GetSymbol(), "A");
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 2: //
			EXPECT_EQ(pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 3: //
			EXPECT_EQ(pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 5:
			EXPECT_EQ(pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 6:
			EXPECT_EQ(pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 7:
			EXPECT_EQ(pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			break;
		case 8:
			EXPECT_EQ(pStock->GetDayLineSize(), 0);
			EXPECT_FALSE(pStock->IsUpdateDayLine());
			EXPECT_FALSE(pStock->IsUpdateDayLineDB());
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
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
		pStock->SetDayLineEndDate(toLocalDays(19800101));
	}
}
