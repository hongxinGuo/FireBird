#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCompanyInsiderTransaction.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyInsiderTransactionTest : public ::testing::Test
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
		CFinnhubCompanyInsiderTransaction companyInsiderTransaction;
	};

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestInitialize) {
		EXPECT_EQ(companyInsiderTransaction.GetIndex(), -1);
		EXPECT_STREQ(companyInsiderTransaction.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol="));
	}

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true);
		companyInsiderTransaction.SetMarket(gl_pWorldMarket.get());
		companyInsiderTransaction.SetIndex(1);
		EXPECT_STREQ(companyInsiderTransaction.CreatMessage(), companyInsiderTransaction.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate());

		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true);
	}

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}