#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubEconomicCalendar.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubEconomicCalendarTest : public ::testing::Test
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
		CFinnhubEconomicCalendar economicCalendar;
	};

	TEST_F(CFinnhubEconomicCalendarTest, TestInitialize) {
		EXPECT_EQ(economicCalendar.GetIndex(), -1);
		EXPECT_STREQ(economicCalendar.GetInquiringStr(), _T("https://finnhub.io/api/v1/calendar/economic?"));
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestCreatMessage) {
		economicCalendar.SetMarket(gl_pWorldMarket.get());
		economicCalendar.SetIndex(1);
		EXPECT_STREQ(economicCalendar.CreatMessage(), economicCalendar.GetInquiringStr());
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestProcessWebData) {
		// ”…MockWorldMarketTest∏∫‘≤‚ ‘
	}
}