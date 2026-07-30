module;
#include"sqlpp23/sqlpp23.h"
#include"StockMarketSQLTable.h"

module Container.Stock.TiingoChosenStock;

import Container.Stock.TiingoStock;
import DatabaseConnector;
import Stock.TiingoStock;

import std;
using std::dynamic_pointer_cast;
using std::string;
using std::vector;

CContainerTiingoChosenStock::CContainerTiingoChosenStock() {
	CContainerTiingoChosenStock::Reset();
}

void CContainerTiingoChosenStock::Reset() {
	CContainerVirtualStock::Reset();
}

bool CContainerTiingoChosenStock::LoadDB() {
	using namespace StockMarket;
	const auto& t = WorldChoiceStock{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t));
	size_t rows = result.size(); //Todo:
	Reserve(rows + 10);
	for (const auto& row : result) {
		if (gl_dataContainerTiingoStock.IsSymbol(string{ row.Symbol })) {
			auto pStock = gl_dataContainerTiingoStock.GetStock(string{ row.Symbol });
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

void CContainerTiingoChosenStock::UpdateDB() const {
	vector<string> vSymbol;
	using namespace StockMarket;
	const auto& t = WorldChoiceStock{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t));
	//size_t rows = result.size(); //Todo:
	vSymbol.reserve(100 + 10);
	for (const auto& row : result) {
		if (gl_dataContainerTiingoStock.IsSymbol(string{ row.Symbol })) {
			vSymbol.push_back(string{ row.Symbol });
		}
	}

	for (size_t i = 0; i < m_vStock.size(); i++) {
		string symbol = m_vStock.at(i)->GetSymbol();
		if (std::ranges::find(vSymbol, symbol) == vSymbol.end()) {
			db(sqlpp::insert_into(t).set(
				t.Symbol = symbol
			));
		}
	}
	tx.commit();
}

CTiingoStockPtr CContainerTiingoChosenStock::GetStock(size_t lIndex) {
	return dynamic_pointer_cast<CTiingoStock>(Get(lIndex));
}

CTiingoStockPtr CContainerTiingoChosenStock::GetStock(const string& strStockCode) {
	return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode));
}
