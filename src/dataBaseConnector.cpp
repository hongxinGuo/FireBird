// Implementation for sqlpp11 connection pool initialization
#include "pch.h"
#include "dataBaseConnector.h"

#include <mutex>

// Define the global connection pool with an empty config and zero size.
// It will be replaced when InitSqlppConnectionPool is called.
//sqlpp::mysql::connection_pool gl_dbStockMarket(std::make_shared<sqlpp::mysql::connection_config>(), 0);

/*
void InitSqlppConnectionPool(const std::string& user,
                             const std::string& password,
                             const std::string& database,
                             const std::string& host,
                             std::size_t poolSize,
                             bool debug) {
	static std::once_flag g_sqlppInitFlag;
	// Initialize the underlying MySQL client library once.
	std::call_once(g_sqlppInitFlag, [] {
		try {
			sqlpp::mysql::global_library_init();
		} catch (...) {
			// swallow exceptions - callers can detect uninitialized pool later
		}
	});

	if (poolSize == 0) poolSize = 1;

	auto config = std::make_shared<sqlpp::mysql::connection_config>();
	config->user = user;
	config->password = password;
	config->database = database;
	config->host = host;
	config->debug = debug;

	// Construct and assign the global pool
	gl_dbStockMarket = sqlpp::mysql::connection_pool(config, static_cast<int>(poolSize));
}
*/
