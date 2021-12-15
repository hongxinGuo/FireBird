#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubStockEstimatesEPSSurprise.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubStockEstimatesEPSSurpriseTest : public ::testing::Test
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
		CFinnhubStockEstimatesEPSSurprise stockEstimatesEPSSurprise;
	};

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestInitialize) {
		EXPECT_EQ(stockEstimatesEPSSurprise.GetIndex(), -1);
		EXPECT_STREQ(stockEstimatesEPSSurprise.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/earnings?symbol="));
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestCreatMessage) {
		stockEstimatesEPSSurprise.SetMarket(gl_pWorldMarket.get());
		stockEstimatesEPSSurprise.SetIndex(1);
		EXPECT_STREQ(stockEstimatesEPSSurprise.CreatMessage(), stockEstimatesEPSSurprise.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}