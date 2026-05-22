#pragma once

#undef min
#undef max
#include <sqlpp11/mysql/connection_pool.h>

// Inline global connection pool. Using an inline variable so the header can be
// included from multiple translation units without violating the one-definition rule.
inline sqlpp::mysql::connection_pool gl_dbStockMarket(std::make_shared<sqlpp::mysql::connection_config>(), 20);

// Initialize the global sqlpp11 MySQL connection pool. This will call
// sqlpp::mysql::global_library_init() once and construct the pool.
inline void InitSqlppMySQLConnectionPool(const std::string& user,
                                         const std::string& password,
                                         const std::string& database,
                                         const std::string& host,
                                         int port = 3306,
                                         std::size_t poolSize = 20,
                                         bool debug = false) {
	static std::once_flag g_sqlppInitFlag;
	std::call_once(g_sqlppInitFlag, [] {
		try {
			sqlpp::mysql::global_library_init();
		} catch (...) {
			// ignore
		}
	});

	if (poolSize == 0) poolSize = 1;

	auto config = std::make_shared<sqlpp::mysql::connection_config>();
	config->user = user;
	config->password = password;
	config->database = database;
	config->host = host;
	config->port = port;
#ifdef _DEBUG
	config->debug = debug;
#endif
	gl_dbStockMarket = sqlpp::mysql::connection_pool(config, static_cast<int>(poolSize));
}

inline auto GetStockMarketDB() {
	return gl_dbStockMarket.get();
}
