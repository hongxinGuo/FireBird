module;
#include"sqlpp23/sqlpp23.h"
#include"StockMarketSQLTable.h"

module ContainerChosenCrypto;

import Container.Stock.FinnhubCrypto;
import DatabaseConnector;
import FinnhubCrypto;

import std;
using std::dynamic_pointer_cast;
using std::string;

CContainerChosenCrypto::CContainerChosenCrypto() {
	CContainerChosenCrypto::Reset();
}

void CContainerChosenCrypto::Reset() {
	CContainerVirtualStock::Reset();
}

bool CContainerChosenCrypto::LoadDB() {
	using namespace StockMarket;
	const auto& t = WorldChoiceCrypto{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t));
	//size_t rows = result.size(); //Todo: 
	Reserve(100 + 10);
	for (const auto& row : result) {
		if (gl_dataFinnhubCryptoSymbol.IsSymbol(string{ row.Symbol })) {
			auto pStock = gl_dataFinnhubCryptoSymbol.GetItem(string{ row.Symbol });
			m_mapSymbol[string{ row.Symbol }] = m_mapSymbol.size();
			m_vStock.push_back(pStock);
		}
		else {
			db(sqlpp::delete_from(t).where(t.ID == row.ID));
		}
	}
	tx.commit();

	return true;
}

CFinnhubCryptoPtr CContainerChosenCrypto::GetCryptoSymbol(size_t lIndex) {
	return dynamic_pointer_cast<CFinnhubCrypto>(Get(lIndex));
}

CFinnhubCryptoPtr CContainerChosenCrypto::GetCryptoSymbol(const string& strStockCode) {
	return dynamic_pointer_cast<CFinnhubCrypto>(Get(strStockCode));
}
