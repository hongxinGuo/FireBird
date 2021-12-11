#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubEconomicCountryList.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubEconomicCountryListTest : public ::testing::Test
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
		CFinnhubEconomicCountryList countryList;
	};

	TEST_F(CFinnhubEconomicCountryListTest, TestInitialize) {
		EXPECT_EQ(countryList.GetIndex(), -1);
		EXPECT_STREQ(countryList.GetInquiringStr(), _T("https://finnhub.io/api/v1/country?"));
	}

	TEST_F(CFinnhubEconomicCountryListTest, TestCreatMessage) {
		EXPECT_STREQ(countryList.CreatMessage(), _T("https://finnhub.io/api/v1/country?"));
	}

	TEST_F(CFinnhubEconomicCountryListTest, TestProcessWebData) {
		// not implmented yet
	}
}