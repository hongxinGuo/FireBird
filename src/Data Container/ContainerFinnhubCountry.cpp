#include "pch.h"
#include <algorithm>
#include<vector>
#include "ContainerFinnhubCountry.h"

CContainerFinnhubCountry::CContainerFinnhubCountry() { Reset(); }

void CContainerFinnhubCountry::Reset() {
	m_vCountry.resize(0);
	m_mapCountry.clear();
	m_llLastTotalCountry = 0;
}
void CContainerFinnhubCountry::Reserve(size_t size) {
	m_vCountry.reserve(size);
	m_mapCountry.reserve(size);
}

void CContainerFinnhubCountry::Add(const CCountry& pCountry) {
	m_mapCountry[pCountry.m_strCountry] = m_vCountry.size();
	m_vCountry.push_back(pCountry);
}

bool CContainerFinnhubCountry::Delete(const CCountry& country) {
	if (!IsCountry(country.m_strCountry)) return false;

	m_mapCountry.erase(country.m_strCountry);
	auto it = std::ranges::find(m_vCountry, country.m_strCode2, &CCountry::m_strCode2);
	if (it != m_vCountry.end()) m_vCountry.erase(it);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
// 此种更新方法，是默认新的国家代码附加在最后。
//
//////////////////////////////////////////////////////////////////////////
void CContainerFinnhubCountry::UpdateDB() {
	if (m_llLastTotalCountry < m_vCountry.size()) {
		using namespace StockMarket;
		const auto& t = FinnhubCountryList{};
		auto db = GetStockMarketDB();
		auto tx = sqlpp::start_transaction(db);
		auto multi_insert = insert_into(t).columns(t.Code2, t.Code3, t.CodeNo,
		                                           t.Country, t.Currency, t.CurrencyCode);
		int nValues = 0;
		for (auto l = m_llLastTotalCountry; l < m_vCountry.size(); l++) {
			const CCountry& country = m_vCountry.at(l);
			multi_insert.values.add(
				t.Code2 = country.m_strCode2,
				t.Code3 = country.m_strCode3,
				t.CodeNo = country.m_strCodeNo,
				t.Country = country.m_strCountry,
				t.Currency = country.m_strCurrency,
				t.CurrencyCode = country.m_strCurrencyCode
			);
			nValues++;
		}
		if (nValues > 0) db(multi_insert);
		tx.commit();
	}
}

bool CContainerFinnhubCountry::LoadDB() {
	using namespace StockMarket;
	const auto& t = FinnhubCountryList{};

	Reset();
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.Country.asc()));
	Reserve(result.size());

	int counter = 0;
	for (const auto& row : result) {
		CCountry country;
		country.m_strCode2 = row.Code2;
		country.m_strCode3 = row.Code3;
		country.m_strCodeNo = row.CodeNo;
		country.m_strCountry = row.Country;
		country.m_strCurrency = row.Currency;
		country.m_strCurrencyCode = row.CurrencyCode;
		m_mapCountry[country.m_strCountry] = counter;
		counter++;
		m_vCountry.push_back(country);
	}
	tx.commit();
	m_llLastTotalCountry = m_vCountry.size();

	return true;
}
