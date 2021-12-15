#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCompanyProfile.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyProfileTest : public ::testing::Test
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
		CFinnhubCompanyProfile companyProfile;
	};

	TEST_F(CFinnhubCompanyProfileTest, TestInitialize) {
		EXPECT_EQ(companyProfile.GetIndex(), -1);
		EXPECT_STREQ(companyProfile.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/profile?symbol="));
	}

	TEST_F(CFinnhubCompanyProfileTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetProfileUpdated(false);
		companyProfile.SetMarket(gl_pWorldMarket.get());
		companyProfile.SetIndex(1);
		EXPECT_STREQ(companyProfile.CreatMessage(), companyProfile.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated());

		gl_pWorldMarket->GetStock(1)->SetProfileUpdated(false);
	}

	TEST_F(CFinnhubCompanyProfileTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}