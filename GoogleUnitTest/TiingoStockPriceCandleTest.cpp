#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"TiingoStockPriceCandle.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CTiingoStockPriceCandleTest : public ::testing::Test
	{
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
		CTiingoStockPriceCandle stockPriceCandle;
	};

	TEST_F(CTiingoStockPriceCandleTest, TestInitialize) {
		EXPECT_EQ(stockPriceCandle.GetIndex(), -1);
		EXPECT_STREQ(stockPriceCandle.GetInquiringStr(), _T("https://api.tiingo.com/tiingo/daily/"));
	}

	TEST_F(CTiingoStockPriceCandleTest, TestCreatMessage) {
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

	TEST_F(CTiingoStockPriceCandleTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}
}