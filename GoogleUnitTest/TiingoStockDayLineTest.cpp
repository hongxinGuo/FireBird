#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductTiingoStockDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CTiingoStockDayLineTest : public ::testing::Test {
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
		CProductTiingoStockDayLine stockPriceCandle;
	};

	TEST_F(CTiingoStockDayLineTest, TestInitialize) {
		EXPECT_EQ(stockPriceCandle.GetIndex(), -1);
		EXPECT_STREQ(stockPriceCandle.GetInquiringStr(), _T("https://api.tiingo.com/tiingo/daily/"));
	}

	TEST_F(CTiingoStockDayLineTest, TestCreatMessage) {
		CString strMessage, strTest;
		stockPriceCandle.SetIndex(0); // 测试数据库中，此股票代码为000001.SS
		stockPriceCandle.SetMarket(gl_pWorldMarket.get());
		strMessage = stockPriceCandle.CreatMessage();
		long lMarketDate = gl_pWorldMarket->GetMarketDate();
		long year = lMarketDate / 10000;
		long month = lMarketDate / 100 - year * 100;
		long day = lMarketDate - year * 10000 - month * 100;
		char buffer[30];
		sprintf_s(buffer, _T("%4d-%2d-%2d"), year, month, day);
		CString strEndDate = buffer;
		CString strMarketDate = gl_pWorldMarket->GetStringOfMarketDate();
		strTest = _T("https://api.tiingo.com/tiingo/daily/000001.SS/prices?&startDate=1980-1-1&endDate=") + strEndDate;
		EXPECT_STREQ(strMessage, strTest);
	}

	TEST_F(CTiingoStockDayLineTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	TiingoWebData tiingoWebData31(1, _T("AAPL"), _T("[\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有date项
	TiingoWebData tiingoWebData32(2, _T("AAPL"), _T("[{\"Missing\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有close项
	TiingoWebData tiingoWebData33(3, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"Missing\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有high项
	TiingoWebData tiingoWebData35(5, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"Missing\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有low项
	TiingoWebData tiingoWebData36(6, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"Missing\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有open项
	TiingoWebData tiingoWebData37(7, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"Missing\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第一个数据没有volume项
	TiingoWebData tiingoWebData38(8, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"Missing\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 第二个数据没有date项
	TiingoWebData tiingoWebData39(9, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"Missing\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));
	// 正确的数据
	TiingoWebData tiingoWebData40(10, _T("AAPL"), _T("[{\"date\":\"2021-03-11T00:00:00.000Z\",\"close\":121.96,\"high\":123.21,\"low\":121.26,\"open\":122.54,\"volume\":103026514,\"adjClose\":121.96,\"adjHigh\":123.21,\"adjLow\":121.26,\"adjOpen\":122.54,\"adjVolume\":103026514,\"divCash\":0.0,\"splitFactor\":1.0}, {\"date\":\"2021-03-12T00:00:00.000Z\",\"close\":121.03,\"high\":121.17,\"low\":119.16,\"open\":120.4,\"volume\":88105050,\"adjClose\":121.03,\"adjHigh\":121.17,\"adjLow\":119.16,\"adjOpen\":120.4,\"adjVolume\":88105050,\"divCash\":0.0,\"splitFactor\":1.0}]"));

	class ParseTiingoStockDayLineTest : public::testing::TestWithParam<TiingoWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
		}

		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoStockDayLine m_tiingoStockPriceCandle;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoStockDayLine1,
													 ParseTiingoStockDayLineTest,
													 testing::Values(&tiingoWebData31, &tiingoWebData32, &tiingoWebData33, &tiingoWebData35,
														 &tiingoWebData36, &tiingoWebData37, &tiingoWebData38, &tiingoWebData39, &tiingoWebData40));

	TEST_P(ParseTiingoStockDayLineTest, TestParseTiingoStockDayLine0) {
		CDayLineVectorPtr pvDayLine;
		CString strMessage;
		CDayLinePtr pDayLine;

		pvDayLine = m_tiingoStockPriceCandle.ParseTiingoStockDayLine(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			strMessage = _T("日线为无效JSon数据\n");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
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
		default:
			break;
		}
	}
}