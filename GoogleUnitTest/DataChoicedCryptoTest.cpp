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
		EXPECT_EQ(m_dataChoicedCrypto.GetSize(), 0) << "��ʼδװ��Crypto����";

		m_dataChoicedCrypto.LoadDB();
		EXPECT_EQ(m_dataChoicedCrypto.GetSize(), 11) << "Ĭ��״̬��װ��12������,�����еĴ���DKAETH������Crypto���뼯���ʶ�ֻװ����11��";

		CFinnhubCryptoSymbolPtr pCrypto = m_dataChoicedCrypto.GetCrypto(2);
		EXPECT_STREQ(pCrypto->GetSymbol(), _T("BINANCE:OCEANBTC")) << "װ��ʱû������ʹ�õ���ԭʼλ��";
	}
}