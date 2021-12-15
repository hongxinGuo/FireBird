#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubStockDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubStockDayLineTest : public ::testing::Test
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
		CFinnhubStockDayLine stockDayLine;
	};

	TEST_F(CFinnhubStockDayLineTest, TestInitialize) {
		EXPECT_EQ(stockDayLine.GetIndex(), -1);
		EXPECT_STREQ(stockDayLine.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/candle?symbol="));
	}

	TEST_F(CFinnhubStockDayLineTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetDayLineNeedUpdate(true);
		stockDayLine.SetMarket(gl_pWorldMarket.get());
		stockDayLine.SetIndex(1);
		EXPECT_STREQ(stockDayLine.CreatMessage(),
			stockDayLine.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetStock(1)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CFinnhubStockDayLineTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}