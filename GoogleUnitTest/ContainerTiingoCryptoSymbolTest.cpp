#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ContainerTiingoCryptoSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerTiingoCryptoSymbolTest : public ::testing::Test {
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
		CContainerTiingoCryptoSymbol m_dataTiingoCryptoSymbol;
	};

	TEST_F(CContainerTiingoCryptoSymbolTest, TestInitialize) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 0);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastSize(), 0);
	}

	TEST_F(CContainerTiingoCryptoSymbolTest, TestIsNeedUpdate) {
		const auto pCrypto = make_shared<CTiingoCryptoSymbol>();

		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsNeedUpdate());
		m_dataTiingoCryptoSymbol.Add(pCrypto);
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsNeedUpdate());
	}

	TEST_F(CContainerTiingoCryptoSymbolTest, TestAddDelete) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 0);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastSize(), 0);

		const auto pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
		pTiingoCrypto->m_strTicker = _T("SS.SS");
		const auto pTiingoCrypto2 = make_shared<CTiingoCryptoSymbol>();
		pTiingoCrypto2->m_strTicker = _T("SS.SS.SS");
		m_dataTiingoCryptoSymbol.Add(pTiingoCrypto);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 1);

		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(pTiingoCrypto));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsSymbol(pTiingoCrypto2));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.Delete(pTiingoCrypto2));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.Delete(pTiingoCrypto));
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 0);
	}

	TEST_F(CContainerTiingoCryptoSymbolTest, TestLoadUpdate) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastSize(), 0) << "初始未装载Crypto代码";

		m_dataTiingoCryptoSymbol.LoadDB();
		EXPECT_EQ(m_dataTiingoCryptoSymbol.GetLastSize(), 1278) << "默认状态下装载1278个Crypto代码";
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(_T("DKAETH")));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(_T("KSMUST")));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsSymbol(_T("500008.SS")));
		CTiingoCryptoSymbolPtr pTiingoCrypto = m_dataTiingoCryptoSymbol.GetSymbol(_T("DKAETH"));
		EXPECT_STREQ(pTiingoCrypto->m_strTicker, _T("DKAETH"));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(pTiingoCrypto));

		// 测试UpdateDB
		pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
		pTiingoCrypto->m_strTicker = _T("AA.BB"); // 新代码
		m_dataTiingoCryptoSymbol.Add(pTiingoCrypto);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 1279);
		m_dataTiingoCryptoSymbol.UpdateDB();

		// 恢复原状
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
