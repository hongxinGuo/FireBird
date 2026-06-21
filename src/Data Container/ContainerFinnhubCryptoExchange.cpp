#include "pch.h"

#include "ContainerFinnhubCryptoExchange.h"

CContainerFinnhubCryptoExchange::CContainerFinnhubCryptoExchange() {
	Reset();
}

void CContainerFinnhubCryptoExchange::Reset() {
	m_vCryptoExchange.resize(0);
	m_mapCryptoExchange.clear();
	m_lLastTotalCryptoExchange = 0;
}

void CContainerFinnhubCryptoExchange::Reserve(size_t size) {
	m_vCryptoExchange.reserve(size);
	m_vCryptoExchange.clear();
	m_mapCryptoExchange.reserve(size);
	m_mapCryptoExchange.clear();
}

bool CContainerFinnhubCryptoExchange::Delete(const string& sCryptoExchange) {
	if (!IsExchange(sCryptoExchange)) return false;

	const auto it = std::ranges::find(m_vCryptoExchange.begin(), m_vCryptoExchange.end(), sCryptoExchange);
	m_vCryptoExchange.erase(it);
	m_mapCryptoExchange.erase(sCryptoExchange);

	return true;
}

void CContainerFinnhubCryptoExchange::Add(const string& sCryptoExchange) {
	m_mapCryptoExchange[sCryptoExchange] = m_vCryptoExchange.size();
	m_vCryptoExchange.push_back(sCryptoExchange);
}

bool CContainerFinnhubCryptoExchange::LoadDB() {
	using namespace StockMarket;
	const auto& t = FinnhubCryptoExchange{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	auto rows = result.size();
	Reserve(rows);
	int i = 0;
	for (const auto& row : result) {
		string str = row.code;
		m_vCryptoExchange.push_back(str);
		m_mapCryptoExchange[str] = i++;
	}
	tx.commit();
	m_lLastTotalCryptoExchange = static_cast<long>(m_vCryptoExchange.size());

	return true;
}

bool CContainerFinnhubCryptoExchange::UpdateDB() {
	if (m_lLastTotalCryptoExchange < m_vCryptoExchange.size()) {
		using namespace StockMarket;
		const auto& t = FinnhubCryptoExchange{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);

		for (auto l = m_lLastTotalCryptoExchange; l < m_vCryptoExchange.size(); l++) {
			db(sqlpp::insert_into(t).set(
				t.code = m_vCryptoExchange.at(l)
			));
		}
		tx.commit();
		m_lLastTotalCryptoExchange = static_cast<long>(m_vCryptoExchange.size());
		return true;
	}
	return false;
}
