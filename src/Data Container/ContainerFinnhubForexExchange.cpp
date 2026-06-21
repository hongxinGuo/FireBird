#include "pch.h"

#include "ContainerFinnhubForexExchange.h"
#include "InfoReport.h"

CContainerFinnhubForexExchange::CContainerFinnhubForexExchange() {
	Reset();
}

void CContainerFinnhubForexExchange::Reset() {
	m_vForexExchange.resize(0);
	m_mapForexExchange.clear();
	m_llLastTotalForexExchange = 0;
}

void CContainerFinnhubForexExchange::Reserve(size_t size) {
	m_vForexExchange.reserve(size);
	m_vForexExchange.clear();
	m_mapForexExchange.reserve(size);
	m_mapForexExchange.clear();
}

bool CContainerFinnhubForexExchange::Delete(const string& strForexExchange) {
	if (!IsExchange(strForexExchange)) return false;

	const auto it = std::ranges::find(m_vForexExchange.begin(), m_vForexExchange.end(), strForexExchange);
	m_vForexExchange.erase(it);
	m_mapForexExchange.erase(strForexExchange);

	return true;
}

void CContainerFinnhubForexExchange::Add(const string& strForexExchange) {
	m_mapForexExchange[strForexExchange] = m_vForexExchange.size();
	m_vForexExchange.push_back(strForexExchange);
}

bool CContainerFinnhubForexExchange::LoadDB() {
	using namespace StockMarket;
	const auto& t = FinnhubForexExchange{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	auto rows = result.size();
	Reserve(rows);
	int i = 0;
	for (const auto& row : result) {
		string str = row.code;
		m_vForexExchange.push_back(str);
		m_mapForexExchange[str] = i++;
	}
	tx.commit();
	m_llLastTotalForexExchange = m_vForexExchange.size();

	return true;
}

bool CContainerFinnhubForexExchange::UpdateDB() {
	if (m_llLastTotalForexExchange < m_vForexExchange.size()) {
		try {
			using namespace StockMarket;
			const auto& t = FinnhubForexExchange{};
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			auto multi_insert = insert_into(t).columns(t.code);
			int nValues = 0;
			for (auto l = m_llLastTotalForexExchange; l < m_vForexExchange.size(); l++) {
				multi_insert.values.add(
					t.code = m_vForexExchange.at(l)
				);
				nValues++;
			}
			if (nValues > 0) db(multi_insert);
			tx.commit();
			m_llLastTotalForexExchange = m_vForexExchange.size();
		} catch (CException& e) {
			ReportInformation(e);
		}
		return true;
	}
	return false;
}
