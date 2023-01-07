#include"pch.h"

#include"GeneralCheck.h"

#include"DataChosenCrypto.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChosenCryptoTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CDataChosenCrypto m_dataChosenCrypto;
	};

	TEST_F(CDataChosenCryptoTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenCrypto.GetSize(), 0);
	}

	TEST_F(CDataChosenCryptoTest, TestLoad) {
		EXPECT_EQ(m_dataChosenCrypto.GetSize(), 0) << "初始未装载Crypto代码";

		m_dataChosenCrypto.LoadDB();
		EXPECT_EQ(m_dataChosenCrypto.GetSize(), 11) << "默认状态下装载12个代码,但其中的代码DKAETH不属于Crypto代码集，故而只装载了11个";

		CFinnhubCryptoSymbolPtr pCrypto = m_dataChosenCrypto.GetCrypto(2);
		EXPECT_STREQ(pCrypto->GetSymbol(), _T("BINANCE:OCEANBTC")) << "装载时没有排序，使用的是原始位置";
	}
}
