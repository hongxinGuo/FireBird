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
		EXPECT_EQ(m_containerChosenCrypto.Size(), 0) << "��ʼδװ��Crypto����";

		m_containerChosenCrypto.LoadDB();
		EXPECT_EQ(m_containerChosenCrypto.Size(), 11) << "Ĭ��״̬��װ��12������,�����еĴ���DKAETH������Crypto���뼯���ʶ�ֻװ����11��";

		const CFinnhubCryptoSymbolPtr pCrypto = dynamic_pointer_cast<CFinnhubCryptoSymbol>(m_containerChosenCrypto.Get(2));
		EXPECT_STREQ(pCrypto->GetSymbol(), _T("BINANCE:OCEANBTC")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
