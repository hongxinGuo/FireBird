#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCryptoDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCryptoDayLineTest : public ::testing::Test
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
		CFinnhubCryptoDayLine cryptoDayLine;
	};

	TEST_F(CFinnhubCryptoDayLineTest, TestInitialize) {
		EXPECT_EQ(cryptoDayLine.GetIndex(), -1);
		EXPECT_STREQ(cryptoDayLine.GetInquiringStr(), _T("https://finnhub.io/api/v1/crypto/candle?symbol="));
	}

	TEST_F(CFinnhubCryptoDayLineTest, TestCreatMessage) {
		cryptoDayLine.SetMarket(gl_pWorldMarket.get());
		cryptoDayLine.SetIndex(1);
		EXPECT_STREQ(cryptoDayLine.CreatMessage(),
			cryptoDayLine.GetInquiringStr() + gl_pWorldMarket->GetCryptoSymbol(1)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
	}

	TEST_F(CFinnhubCryptoDayLineTest, TestProcessWebData) {
		// ÓÉMockWorldMarketTest¸ºÔğ²âÊÔ
	}
}