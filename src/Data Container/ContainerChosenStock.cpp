#include "pch.h"

#include "ContainerChosenStock.h"

#include "dataBaseConnector.h"

CContainerChosenStock::CContainerChosenStock() {
	CContainerChosenStock::Reset();
}

void CContainerChosenStock::Reset() {
	CContainerVirtualStock::Reset();

	m_lChosenStockPos = 0;
}

bool CContainerChosenStock::LoadDB() {
	bool fDeleteDuplicatedSymbol = false;
	using namespace StockMarket;
	const auto& t = WorldChoiceStock{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	Reset();
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		if (gl_dataContainerFinnhubStock.IsSymbol(row.Symbol)) {
			auto pStock = gl_dataContainerFinnhubStock.GetItem(row.Symbol);
			m_mapSymbol[row.Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pStock);
		}
		else {
			fDeleteDuplicatedSymbol = true;
		}
	}
	tx.commit();

	return true;
}
