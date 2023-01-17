#include"pch.h"

#include"GeneralCheck.h"

#include"containerChosenCrypto.h"
#include"FinnhubCryptoSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChosenCryptoTest : public Test {
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
		CContainerChosenCrypto m_dataChosenCrypto;
	};

	TEST_F(CDataChosenCryptoTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenCrypto.Size(), 0);
	}

	TEST_F(CDataChosenCryptoTest, TestLoad) {
		EXPECT_EQ(m_dataChosenCrypto.Size(), 0) << "��ʼδװ��Crypto����";

		m_dataChosenCrypto.LoadDB();
		EXPECT_EQ(m_dataChosenCrypto.Size(), 11) << "Ĭ��״̬��װ��12������,�����еĴ���DKAETH������Crypto���뼯���ʶ�ֻװ����11��";

		CFinnhubCryptoSymbolPtr pCrypto = dynamic_pointer_cast<CFinnhubCryptoSymbol>(m_dataChosenCrypto.Get(2));
		EXPECT_STREQ(pCrypto->GetSymbol(), _T("BINANCE:OCEANBTC")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
