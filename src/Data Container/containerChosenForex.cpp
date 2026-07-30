module;
#include"sqlpp23/sqlpp23.h"
#include"StockMarketSQLTable.h"

module Container.Stock.ChosenForex;


import Container.Stock.FinnhubForexSymbol;
import DatabaseConnector;
import Stock.FinnhubForex;

import std;
import Container.Stock.ChosenForex;
using std::dynamic_pointer_cast;
using std::string;

CContainerChosenForex::CContainerChosenForex() {
	CContainerChosenForex::Reset();
}

void CContainerChosenForex::Reset() {
	CContainerVirtualStock::Reset();
}

bool CContainerChosenForex::LoadDB() {
	using namespace StockMarket;
	const auto& t = WorldChoiceForex{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t));
	size_t rows = result.size(); //Todo:
	Reserve(rows + 10);
	for (const auto& row : result) {
		if (gl_dataFinnhubForexSymbol.IsSymbol(string{ row.Symbol })) {
			auto pStock = gl_dataFinnhubForexSymbol.GetItem(string{ row.Symbol });
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

CForexSymbolPtr CContainerChosenForex::GetForexSymbol(const size_t lIndex) {
	return dynamic_pointer_cast<CFinnhubForex>(Get(lIndex));
}

CForexSymbolPtr CContainerChosenForex::GetForexSymbol(const string& strStockCode) {
	return dynamic_pointer_cast<CFinnhubForex>(Get(strStockCode));
}
