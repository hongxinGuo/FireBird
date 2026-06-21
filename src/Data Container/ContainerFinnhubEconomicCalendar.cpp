#include "pch.h"
#include "ContainerFinnhubEconomicCalendar.h"

CContainerFinnhubEconomicCalendar::CContainerFinnhubEconomicCalendar() {
	Reset();
}

void CContainerFinnhubEconomicCalendar::Reset() {
	m_lLastTotalEconomicCalendar = 0;
	m_mapEconomicCalendar.clear();
	m_vEconomicCalendar.resize(0);
}

void CContainerFinnhubEconomicCalendar::Reserve(size_t lSize) {
	m_vEconomicCalendar.reserve(lSize);
	m_vEconomicCalendar.clear();
	m_mapEconomicCalendar.reserve(lSize);
	m_mapEconomicCalendar.clear();
}

bool CContainerFinnhubEconomicCalendar::LoadDB() {
	using namespace StockMarket;
	const auto& t = FinnhubEconomicCalendar{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	auto rows = result.size();
	Reserve(rows + 2);
	for (const auto& row : result) {
		CEconomicCalendar economicCalendar;
		economicCalendar.m_strTime = row.Time;
		economicCalendar.m_strCountry = row.Country;
		economicCalendar.m_strEvent = row.Event;
		economicCalendar.m_strImpact = row.Impact;
		economicCalendar.m_dActual = row.Actual;
		economicCalendar.m_dEstimate = row.Estimate;
		economicCalendar.m_dPrev = row.Prev;
		economicCalendar.m_strUnit = row.Unit;
		string strSymbol = economicCalendar.m_strCountry + economicCalendar.m_strEvent + economicCalendar.m_strTime;
		m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
		m_vEconomicCalendar.push_back(economicCalendar);
	}
	tx.commit();

	m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();
	return true;
}

bool CContainerFinnhubEconomicCalendar::UpdateDB() {
	//return true; //Todo: 经济数据中有非法字符，暂时不存储了。
	using namespace StockMarket;
	const auto& t = FinnhubEconomicCalendar{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Time, t.Country, t.Event,
	                                           t.Impact, t.Actual, t.Estimate, t.Prev, t.Unit);

	if (m_lLastTotalEconomicCalendar >= m_vEconomicCalendar.size()) return false;
	int nValues = 0;
	for (auto l = m_lLastTotalEconomicCalendar; l < m_vEconomicCalendar.size(); l++) {
		multi_insert.values.add(
			t.Time = m_vEconomicCalendar.at(l).m_strTime,
			t.Country = m_vEconomicCalendar.at(l).m_strCountry.c_str(),
			t.Event = m_vEconomicCalendar.at(l).m_strEvent.c_str(),
			t.Impact = m_vEconomicCalendar.at(l).m_strImpact.c_str(),
			t.Actual = m_vEconomicCalendar.at(l).m_dActual,
			t.Estimate = m_vEconomicCalendar.at(l).m_dEstimate,
			t.Prev = m_vEconomicCalendar.at(l).m_dPrev,
			t.Unit = m_vEconomicCalendar.at(l).m_strUnit.c_str()
		);
		nValues++;
	}
	if (nValues > 0) db(multi_insert);
	tx.commit();

	m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();
	return true;
}

bool CContainerFinnhubEconomicCalendar::Update(const vector<CEconomicCalendar>& vEconomicCalendar) {
	for (auto& economicCalendar : vEconomicCalendar) {
		string strSymbol = economicCalendar.m_strCountry + economicCalendar.m_strEvent + economicCalendar.m_strTime;
		if (!m_mapEconomicCalendar.contains(strSymbol)) {	// 新事件？
			m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
			m_vEconomicCalendar.push_back(economicCalendar);
		}
	}
	return true;
}
