#include"windows.h"
#include"shellapi.h"

#include <iostream>

#undef min
#undef max
#include <sqlpp11/transaction.h>

#include "dataBaseConnector.h"
#include "StockMarketSQLTable.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 测试数据库有时会由于测试没有顺利结束而导致遗留中间数据，本函数删除之
///
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearTestDataBase() {
	using namespace StockMarket;

	{ // 删除gl_dataFinnhubCryptoExchange中的中间数据
		const auto& t = ChinaStockCode{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除china dayline中的中间数据
		const auto& t = ChinaStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除china weekline中的中间数据
		const auto& t = ChinaStockWeekline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除china weekline中的中间数据
		const auto& t = FinnhubCryptoDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除gl_dataFinnhubCryptoExchange中的中间数据
		const auto& t = FinnhubCryptoExchange{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.code == "Test"));
		tx.commit();
	}

	{ // 删除china weekline中的中间数据
		const auto& t = FinnhubCryptoSymbol{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除finnhub forex dayline中的中间数据
		const auto& t = FinnhubForexDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除gl_dataFinnhubForexExchange中的中间数据
		const auto& t = FinnhubForexExchange{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.code == "Test"));
		tx.commit();
	}

	{ // 删除china weekline中的中间数据
		const auto& t = FinnhubForexSymbol{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除gl_dataFinnhubCryptoExchange中的中间数据
		const auto& t = FinnhubStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除china weekline中的中间数据
		const auto& t = FinnhubStockExchange{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.code == "Test"));
		tx.commit();
	}

	{ // 删除china weekline中的中间数据
		const auto& t = FinnhubStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}

	{ // 删除china weekline中的中间数据
		const auto& t = TiingoStockDayline{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		db(sqlpp::remove_from(t).where(t.Exchange == "Test"));
		tx.commit();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UNICODE时，使用wWinMain作为入口函数。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE HInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
	std::cout << "ClearData: Clearing test database...\n";
	//Note: 使用测试环境的数据库连接池，避免对正式环境的数据库造成影响。测试环境的数据库连接池在测试结束时会自动析构。
	//InitSqlppMySQLConnectionPool("FireBird", "firebird", "stock_market", "localhost", 3306, 20, false); //Note:: 连接正式环境的数据库，谨慎使用
	InitSqlppMySQLConnectionPool("Test", "test", "stock_market_test", "localhost", 3306, 20, false); // Note:: 连接测试环境的数据库

	ClearTestDataBase();

	std::cout << "ClearData: Test database cleared.\n";

	return 0;
}
