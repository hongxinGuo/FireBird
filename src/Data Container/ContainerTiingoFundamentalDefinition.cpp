#include "pch.h"

#include "ContainerTiingoFundamentalDefinition.h"

CContainerTiingoFundamentalDefinition::CContainerTiingoFundamentalDefinition() {
	m_vTiingoFundamentalDefinition.resize(0);
	m_mapTiingoFundamentalDefinition.clear();
	m_fUpdated = false;
}

void CContainerTiingoFundamentalDefinition::Reset() {
	m_vTiingoFundamentalDefinition.clear();
	m_mapTiingoFundamentalDefinition.clear();
}

void CContainerTiingoFundamentalDefinition::Reserve(size_t size) {
	m_vTiingoFundamentalDefinition.reserve(size);
	m_mapTiingoFundamentalDefinition.reserve(size);
}

void CContainerTiingoFundamentalDefinition::Add(CTiingoFundamentalDefinition tiingoFundamentalDefinition) {
	if (!HaveDefinition(tiingoFundamentalDefinition)) {
		m_mapTiingoFundamentalDefinition[tiingoFundamentalDefinition.m_strDataCode] = m_vTiingoFundamentalDefinition.size();
		m_vTiingoFundamentalDefinition.push_back(tiingoFundamentalDefinition);
		m_fUpdated = true;
	}
}

bool CContainerTiingoFundamentalDefinition::Delete(CTiingoFundamentalDefinition tiingoFundamentalDefinition) {
	if (!HaveDefinition(tiingoFundamentalDefinition)) return false;

	auto it = std::ranges::find(m_vTiingoFundamentalDefinition, tiingoFundamentalDefinition.m_strDataCode, &CTiingoFundamentalDefinition::m_strDataCode);
	if (it != m_vTiingoFundamentalDefinition.end()) m_vTiingoFundamentalDefinition.erase(it);
	m_mapTiingoFundamentalDefinition.erase(tiingoFundamentalDefinition.m_strDataCode);
	m_fUpdated = true;

	return true;
}

bool CContainerTiingoFundamentalDefinition::Delete(const string& strDataCode) {
	if (HaveDefinition(strDataCode)) {
		auto p = GetFundamentalDefinition(strDataCode);
		Delete(p);
		return true;
	}

	return false;
}

bool CContainerTiingoFundamentalDefinition::UpdateDB() {
	ASSERT(m_fUpdated);
	map<string, size_t> mapDefinition;
	using namespace StockMarket;
	const auto& t = TiingoFundamentalDefinitions{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	auto multi_insert = insert_into(t).columns(t.dataCode, t.name, t.description, t.statementType, t.units);
	for (const auto& row : result) {
		mapDefinition[row.dataCode] = mapDefinition.size();
	}

	int nValues = 0;
	for (auto& tiingoFundamentalDefinition : m_vTiingoFundamentalDefinition) {
		if (!mapDefinition.contains(tiingoFundamentalDefinition.m_strDataCode)) { // 只添加新增的项目。
			multi_insert.values.add(
				t.dataCode = tiingoFundamentalDefinition.m_strDataCode,
				t.name = tiingoFundamentalDefinition.m_strName.c_str(),
				t.description = tiingoFundamentalDefinition.m_strDescription.c_str(),
				t.statementType = tiingoFundamentalDefinition.m_strStatementType.c_str(),
				t.units = tiingoFundamentalDefinition.m_strUnits.c_str()
			);
			nValues++;
		}
	}
	if (nValues > 0) db(multi_insert);
	tx.commit();
	m_fUpdated = false;

	return true;
}

bool CContainerTiingoFundamentalDefinition::LoadDB() {
	using namespace StockMarket;
	const auto& t = TiingoFundamentalDefinitions{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	Reset();
	auto result = db(select(all_of(t)).from(t).unconditionally());
	size_t rows = result.size();
	Reserve(rows);
	for (const auto& row : result) {
		CTiingoFundamentalDefinition tiingoFundamentalDefinition;
		tiingoFundamentalDefinition.m_strDataCode = row.dataCode;
		tiingoFundamentalDefinition.m_strName = row.name;
		tiingoFundamentalDefinition.m_strDescription = row.description;
		tiingoFundamentalDefinition.m_strStatementType = row.statementType;
		tiingoFundamentalDefinition.m_strUnits = row.units;
		Add(tiingoFundamentalDefinition);
	}
	m_fUpdated = false;

	return true;
}
