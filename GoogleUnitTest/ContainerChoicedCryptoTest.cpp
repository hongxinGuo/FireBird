#include"pch.h"

#include"GeneralCheck.h"

#include"containerChosenCrypto.h"
#include"FinnhubCryptoSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerChosenCryptoTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CContainerChosenCrypto m_containerChosenCrypto;
	};

	TEST_F(CContainerChosenCryptoTest, TestInitialize) {
		EXPECT_EQ(m_containerChosenCrypto.Size(), 0);
	}

	TEST_F(CContainerChosenCryptoTest, TestLoad) {
		EXPECT_EQ(m_containerChosenCrypto.Size(), 0) << "初始未装载Crypto代码";

		m_containerChosenCrypto.LoadDB();
		EXPECT_EQ(m_containerChosenCrypto.Size(), 11) << "默认状态下装载12个代码,但其中的代码DKAETH不属于Crypto代码集，故而只装载了11个";

		const CFinnhubCryptoSymbolPtr pCrypto = dynamic_pointer_cast<CFinnhubCryptoSymbol>(m_containerChosenCrypto.Get(2));
		EXPECT_STREQ(pCrypto->GetSymbol(), _T("BINANCE:OCEANBTC")) << "装载时没有排序，使用的是原始位置";
	}
}
