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
	}

	TEST_F(CContainerTiingoCryptoSymbolTest, TestIsUpdateProfileDB) {
		const auto pCrypto = make_shared<CTiingoCrypto>();
		pCrypto->SetUpdateProfileDB(true);

		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsUpdateProfileDB());
		m_dataTiingoCryptoSymbol.Add(pCrypto);
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsUpdateProfileDB());
	}

	TEST_F(CContainerTiingoCryptoSymbolTest, TestAddDelete) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 0);

		auto pTiingoCrypto = make_shared<CTiingoCrypto>();
		pTiingoCrypto->SetSymbol(_T("SS.SS"));
		auto pTiingoCrypto2 = make_shared<CTiingoCrypto>();
		pTiingoCrypto2->SetSymbol(_T("SS.SS.SS"));
		m_dataTiingoCryptoSymbol.Add(pTiingoCrypto);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 1);

		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(pTiingoCrypto));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsSymbol(pTiingoCrypto2));
		m_dataTiingoCryptoSymbol.Delete(pTiingoCrypto2);
		m_dataTiingoCryptoSymbol.Delete(pTiingoCrypto);

		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 0);
	}

	TEST_F(CContainerTiingoCryptoSymbolTest, TestLoadUpdate) {
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 0) << "初始未装载Crypto代码";

		m_dataTiingoCryptoSymbol.LoadDB();
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 1278) << "默认状态下装载1278个Crypto代码";
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(_T("DKAETH")));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(_T("KSMUST")));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsSymbol(_T("500008.SS")));
		CTiingoCryptoPtr pTiingoCrypto = m_dataTiingoCryptoSymbol.GetCrypto(_T("DKAETH"));
		EXPECT_STREQ(pTiingoCrypto->GetSymbol().c_str(), _T("DKAETH"));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(pTiingoCrypto));

		// 测试UpdateDB
		pTiingoCrypto = make_shared<CTiingoCrypto>();
		pTiingoCrypto->SetSymbol(_T("AA.BB")); // 新代码
		pTiingoCrypto->SetUpdateProfileDB(true);
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
