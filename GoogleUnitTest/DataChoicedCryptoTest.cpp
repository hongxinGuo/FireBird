#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"DataChoicedCrypto.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataChoicedCryptoTest : public ::testing::Test {
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
		CDataChoicedCrypto m_dataChoicedCrypto;
	};

	TEST_F(CDataChoicedCryptoTest, TestInitialize) {
		EXPECT_EQ(m_dataChoicedCrypto.GetSize(), 0);
	}

	TEST_F(CDataChoicedCryptoTest, TestLoad) {
		EXPECT_EQ(m_dataChoicedCrypto.GetSize(), 0) << "初始未装载Crypto代码";

		m_dataChoicedCrypto.LoadDB();
		EXPECT_EQ(m_dataChoicedCrypto.GetSize(), 11) << "默认状态下装载12个代码,但其中的代码DKAETH不属于Crypto代码集，故而只装载了11个";

		CFinnhubCryptoSymbolPtr pCrypto = m_dataChoicedCrypto.GetCrypto(2);
		EXPECT_STREQ(pCrypto->GetSymbol(), _T("BINANCE:OCEANBTC")) << "装载时没有排序，使用的是原始位置";
	}
}