#include "pch.h"

#include "ContainerTiingoChosenStock.h"
#include<sqlpp23/sqlpp23.h>

#include "ContainerTiingoStock.h"
#include "dataBaseConnector.h"
#include"StockMarketSQLTable.h"

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
	size_t rows = result.size();
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
	size_t rows = result.size();
	vSymbol.reserve(rows);
	for (const auto& row : result) {
		if (gl_dataContainerTiingoStock.IsSymbol(string{ row.Symbol })) {
			vSymbol.push_back(string{ row.Symbol });
		}
	}

	for (size_t i = 0; i < m_vStock.size(); i++) {
		string symbol = m_vStock.at(i)->GetSymbol();
		if (ranges::find(vSymbol, symbol) == vSymbol.end()) {
			db(sqlpp::insert_into(t).set(
				t.Symbol = symbol
			));
		}
	}
	tx.commit();
}
