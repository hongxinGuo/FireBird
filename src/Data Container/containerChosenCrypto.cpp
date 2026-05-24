#include "pch.h"

#include "containerChosenCrypto.h"

#include "dataBaseConnector.h"
#include"FinnhubCrypto.h"

CContainerChosenCrypto::CContainerChosenCrypto() {
	CContainerChosenCrypto::Reset();
}

void CContainerChosenCrypto::Reset() {
	CContainerVirtualStock::Reset();

	m_lChosenCryptoPos = 0;
}

bool CContainerChosenCrypto::LoadDB() {
	bool fDeleteDuplicatedSymbol = false;
	using namespace StockMarket;
	const auto& t = WorldChoiceCrypto{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	Reset();
	size_t rows = result.size();
	Reserve(rows + 10);
	for (const auto& row : result) {
		if (gl_dataFinnhubCryptoSymbol.IsSymbol(row.Symbol)) {
			auto pStock = gl_dataFinnhubCryptoSymbol.GetItem(row.Symbol);
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
