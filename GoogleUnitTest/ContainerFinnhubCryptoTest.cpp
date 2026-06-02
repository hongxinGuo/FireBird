#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerFinnhubCrypto.h"
#include "dataBaseConnector.h"

using namespace testing;

namespace FireBirdTest {
	class CContainerFinnhuibCryptoTest : public ::testing::Test {
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
		CContainerFinnhubCrypto m_dataFinnhubCrypto;
	};

	TEST_F(CContainerFinnhuibCryptoTest, TestInitialize) {
		EXPECT_EQ(m_dataFinnhubCrypto.Size(), 0);
	}

	TEST_F(CContainerFinnhuibCryptoTest, TestIsUpdateProfileDB) {
		const auto pCrypto = make_shared<CTiingoCrypto>();
		pCrypto->SetUpdateProfileDB(true);

		EXPECT_FALSE(m_dataFinnhubCrypto.IsUpdateProfileDB());
		m_dataFinnhubCrypto.Add(pCrypto);
		EXPECT_TRUE(m_dataFinnhubCrypto.IsUpdateProfileDB());
	}

	TEST_F(CContainerFinnhuibCryptoTest, TestLoadUpdate) {
		EXPECT_EQ(m_dataFinnhubCrypto.Size(), 0) << "初始未装载Crypto代码";

		m_dataFinnhubCrypto.LoadProfileDB();
		EXPECT_EQ(m_dataFinnhubCrypto.Size(), 8367) << "默认状态下装载8367个Crypto代码";
		EXPECT_TRUE(m_dataFinnhubCrypto.IsSymbol("BINANCE:USDTUAH"));
		EXPECT_TRUE(m_dataFinnhubCrypto.IsSymbol("BINANCE:OSTBNB"));
		EXPECT_FALSE(m_dataFinnhubCrypto.IsSymbol("500008.SS"));
		CFinnhubCryptoPtr pFinnhubCrypto = m_dataFinnhubCrypto.GetItem("BINANCE:USDTUAH");
		EXPECT_EQ(pFinnhubCrypto->GetSymbol(), "BINANCE:USDTUAH");
		EXPECT_TRUE(m_dataFinnhubCrypto.IsSymbol(pFinnhubCrypto));

		// 测试UpdateDB
		pFinnhubCrypto = make_shared<CFinnhubCrypto>();
		pFinnhubCrypto->SetSymbol("AA.BB"); // 新代码
		pFinnhubCrypto->SetExchange("Test");

		pFinnhubCrypto->SetUpdateProfileDB(true);
		m_dataFinnhubCrypto.Add(pFinnhubCrypto);
		EXPECT_EQ(m_dataFinnhubCrypto.Size(), 8368);
		m_dataFinnhubCrypto.UpdateProfileDB();

		// 恢复原状
		using namespace StockMarket;
		const auto& t = FinnhubCryptoSymbol{};
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);
		db(remove_from(t).where(t.Symbol == "AA.BB" && t.Exchange == "Test"));
		tx.commit();
	}

	TEST_F(CContainerFinnhuibCryptoTest, TestLoadProfileDB) {
		using namespace StockMarket;
		const auto& t = FinnhubCryptoSymbol{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			db(sqlpp::insert_into(t).set(t.Symbol = "BINANCE:USDTUAH", t.Exchange = "Test")); // 重复代码，用于测试，此时代码总数是5702
			tx.commit();
		}
		m_dataFinnhubCrypto.LoadProfileDB();
		EXPECT_EQ(m_dataFinnhubCrypto.Size(), 8367); // 2024-06-01Finnhub股票总共有8367只股票。如果数据库中有重复的股票代码，则会被删除，所以最终装载的股票数应该是8366。
		{
			auto db = gl_dbStockMarket.get();
			auto tx = start_transaction(db);

			auto result = db(sqlpp::select(all_of(t)).from(t).where(t.Symbol == "BINANCE:USDTUAH"));
			tx.commit();
			size_t rows = result.size();
			EXPECT_EQ(rows, 1) << "数据库中应该只有一条BINANCE:USDTUAH的记录";
			auto& row = result.front();
			EXPECT_EQ(row.Symbol.value(), "BINANCE:USDTUAH");
			EXPECT_EQ(row.ID.value(), 1);
		}
		m_dataFinnhubCrypto.Reset();
	}
}
