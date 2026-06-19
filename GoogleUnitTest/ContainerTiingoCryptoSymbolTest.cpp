#include"pch.h"

#include"GeneralCheck.h"

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
		pTiingoCrypto->SetSymbol("SS.SS");
		auto pTiingoCrypto2 = make_shared<CTiingoCrypto>();
		pTiingoCrypto2->SetSymbol("SS.SS.SS");
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
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol("DKAETH"));
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol("KSMUST"));
		EXPECT_FALSE(m_dataTiingoCryptoSymbol.IsSymbol("500008.SS"));
		CTiingoCryptoPtr pTiingoCrypto = m_dataTiingoCryptoSymbol.GetCrypto("DKAETH");
		EXPECT_EQ(pTiingoCrypto->GetSymbol(), "DKAETH");
		EXPECT_TRUE(m_dataTiingoCryptoSymbol.IsSymbol(pTiingoCrypto));

		// 测试UpdateDB
		pTiingoCrypto = make_shared<CTiingoCrypto>();
		pTiingoCrypto->SetSymbol("AA.BB"); // 新代码
		pTiingoCrypto->SetUpdateProfileDB(true);
		m_dataTiingoCryptoSymbol.Add(pTiingoCrypto);
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 1279);
		m_dataTiingoCryptoSymbol.UpdateDB();

		// 恢复原状
		using namespace StockMarket;
		const auto& t = TiingoCryptoSymbol{};
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);
		db(remove_from(t).where(t.Symbol == "AA.BB"));
		tx.commit();
	}

	TEST_F(CContainerTiingoCryptoSymbolTest, TestLoadProfileDB) {
		using namespace StockMarket;
		const auto& t = TiingoCryptoSymbol{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			db(sqlpp::insert_into(t).set(t.Symbol = "DKAETH", t.Name = "Test")); // 重复代码，用于测试，此时代码总数是5702
			tx.commit();
		}
		m_dataTiingoCryptoSymbol.LoadDB();
		EXPECT_EQ(m_dataTiingoCryptoSymbol.Size(), 1278); // 2024-06-01Tiingo股票总共有7183只股票。如果数据库中有重复的股票代码，则会被删除，所以最终装载的股票数应该是5701。
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			auto result = db(sqlpp::select(all_of(t)).from(t).where(t.Symbol == "DKAETH"));
			tx.commit();
			size_t rows = result.size();
			EXPECT_EQ(rows, 1) << "数据库中应该只有一条DKAETH的记录";
			auto& row = result.front();
			EXPECT_EQ(row.Symbol.value(), "DKAETH");
			EXPECT_EQ(row.ID.value(), 1);
		}
		m_dataTiingoCryptoSymbol.Reset();
	}
}
