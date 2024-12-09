#include"pch.h"

 

#include"GeneralCheck.h"

#include"FinnhubCrypto.h"
#include"containerChosenCrypto.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerChosenCryptoTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
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

		const CFinnhubCryptoPtr pCrypto = m_containerChosenCrypto.GetCryptoSymbol(2);
		EXPECT_STREQ(pCrypto->GetSymbol(), _T("BINANCE:OCEANBTC")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}
