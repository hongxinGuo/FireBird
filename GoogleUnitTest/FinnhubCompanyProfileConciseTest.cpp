#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCompanyProfileConcise.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyProfileConciseTest : public ::testing::Test
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
		CFinnhubCompanyProfileConcise companyProfileConcise;
	};

	TEST_F(CFinnhubCompanyProfileConciseTest, TestInitialize) {
		EXPECT_EQ(companyProfileConcise.GetIndex(), -1);
		EXPECT_STREQ(companyProfileConcise.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/profile2?symbol="));
	}

	TEST_F(CFinnhubCompanyProfileConciseTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetProfileUpdated(false);
		companyProfileConcise.SetMarket(gl_pWorldMarket.get());
		companyProfileConcise.SetIndex(1);
		EXPECT_STREQ(companyProfileConcise.CreatMessage(), companyProfileConcise.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsProfileUpdated());

		gl_pWorldMarket->GetStock(1)->SetProfileUpdated(false);
	}

	TEST_F(CFinnhubCompanyProfileConciseTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}