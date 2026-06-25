#include "pch.h"

#include "ContainerTiingoChosenStock.h"

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

	auto result = db(select(all_of(t)).from(t).unconditionally());
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		if (gl_dataContainerTiingoStock.IsSymbol(row.Symbol)) {
			auto pStock = gl_dataContainerTiingoStock.GetStock(row.Symbol);
			m_mapSymbol[row.Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pStock);
		}
		else {
			db(sqlpp::remove_from(t).where(t.ID == row.ID));
		}
	}
	tx.commit();

	return true;
}
