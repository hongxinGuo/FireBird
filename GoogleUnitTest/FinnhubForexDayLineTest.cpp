#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubForexDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubForexDayLineTest : public ::testing::Test
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
		CFinnhubForexDayLine forexDayLine;
	};

	TEST_F(CFinnhubForexDayLineTest, TestInitialize) {
		EXPECT_EQ(forexDayLine.GetIndex(), -1);
		EXPECT_STREQ(forexDayLine.GetInquiringStr(), _T("https://finnhub.io/api/v1/forex/candle?symbol="));
	}

	TEST_F(CFinnhubForexDayLineTest, TestCreatMessage) {
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
		forexDayLine.SetMarket(gl_pWorldMarket.get());
		forexDayLine.SetIndex(1);
		EXPECT_STREQ(forexDayLine.CreatMessage(),
			forexDayLine.GetInquiringStr() + gl_pWorldMarket->GetForexSymbol(1)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CFinnhubForexDayLineTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}