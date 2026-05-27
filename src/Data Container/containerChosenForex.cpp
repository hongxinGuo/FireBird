#include "pch.h"

#include"FinnhubForex.h"
#include "ContainerChosenForex.h"
#include "dataBaseConnector.h"

CContainerChosenForex::CContainerChosenForex() {
	CContainerChosenForex::Reset();
}

void CContainerChosenForex::Reset() {
	CContainerVirtualStock::Reset();

	m_lChosenForexPos = 0;
}

bool CContainerChosenForex::LoadDB() {
	using namespace StockMarket;
	const auto& t = WorldChoiceForex{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	Reset();
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		if (gl_dataFinnhubForexSymbol.IsSymbol(row.Symbol)) {
			auto pStock = gl_dataFinnhubForexSymbol.GetItem(row.Symbol);
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
