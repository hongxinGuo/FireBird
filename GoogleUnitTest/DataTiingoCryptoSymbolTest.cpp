#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"DataTiingoCryptoSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataTiingoCryptoSymbolTest : public ::testing::Test {
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
		CDataTiingoCryptoSymbol m_dataTiingoCryptoSymbol;
	};

	TEST_F(CDataTiingoCryptoSymbolTest, TestInitialize) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetCryptoSymbolSize(), 0);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastCryptoSymbolSize(), 0);
	}

	TEST_F(CDataTiingoCryptoSymbolTest, TestAddDelete) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetCryptoSymbolSize(), 0);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastCryptoSymbolSize(), 0);

		CTiingoCryptoSymbolPtr pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
		pTiingoCrypto->m_strTicker = _T("SS.SS");
		CTiingoCryptoSymbolPtr pTiingoCrypto2 = make_shared<CTiingoCryptoSymbol>();
		pTiingoCrypto2->m_strTicker = _T("SS.SS.SS");
		m_dataTiingoCryptoSymbol.Add(pTiingoCrypto);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetCryptoSymbolSize(), 1);

		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(pTiingoCrypto));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(pTiingoCrypto2));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.Delete(pTiingoCrypto2));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.Delete(pTiingoCrypto));
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetCryptoSymbolSize(), 0);
	}

	TEST_F(CDataTiingoCryptoSymbolTest, TestLoadUpdate) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastCryptoSymbolSize(), 0) << "��ʼδװ��Crypto����";

		m_dataTiingoCryptoSymbol.LoadDB();
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastCryptoSymbolSize(), 1278) << "Ĭ��״̬��װ��1278��Crypto����";
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(_T("DKAETH")));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(_T("KSMUST")));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(_T("500008.SS")));
		CTiingoCryptoSymbolPtr pTiingoCrypto = m_dataTiingoCryptoSymbol.GetCryptoSymbol(_T("DKAETH"));
		EXPECT_STREQ(pTiingoCrypto->m_strTicker, _T("DKAETH"));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(pTiingoCrypto));

		// ����UpdateDB
		pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
		pTiingoCrypto->m_strTicker = _T("AA.BB"); // �´���
		m_dataTiingoCryptoSymbol.Add(pTiingoCrypto);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetCryptoSymbolSize(), 1279);
		m_dataTiingoCryptoSymbol.UpdateDB();

		// �ָ�ԭ״
		CSetTiingoCrypto setCryptoSymbol;
		setCryptoSymbol.m_strFilter = _T("[Ticker] = 'AA.BB'");
		setCryptoSymbol.Open();
		EXPECT_FALSE(setCryptoSymbol.IsEOF());
		setCryptoSymbol.m_pDatabase->BeginTrans();
		setCryptoSymbol.Delete();
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
	}
}