#include "pch.h"

#include "ContainerTiingoFundamentalDefinition.h"

#include "CharSetTransfer.h"
#include "InfoReport.h"

CContainerTiingoFundamentalDefinition::CContainerTiingoFundamentalDefinition() {
	m_vTiingoFundamentalDefinition.resize(0);
	m_mapTiingoFundamentalDefinition.clear();
	m_fUpdated = false;
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
	if (m_fUpdated) {
		CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition, setDefinition;
		map<string, size_t> mapDefinition;
		setDefinition.Open();
		setDefinition.m_pDatabase->BeginTrans();
		while (!setDefinition.IsEOF()) {
			mapDefinition[T2Utf8(setDefinition.m_dataCode)] = mapDefinition.size();
			setDefinition.MoveNext();
		}
		setDefinition.m_pDatabase->CommitTrans();
		setDefinition.Close();

		setTiingoFundamentalDefinition.Open();
		setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
		try {
			for (auto& tiingoFundamentalDefinition : m_vTiingoFundamentalDefinition) {
				if (!mapDefinition.contains(tiingoFundamentalDefinition.m_strDataCode)) { // 只添加新增的项目。
					tiingoFundamentalDefinition.Append(setTiingoFundamentalDefinition);
				}
			}
		} catch (CException& e) {
			ReportInformation(e);
		}
		setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
		setTiingoFundamentalDefinition.Close();
		m_fUpdated = false;
	}
	return true;
}

bool CContainerTiingoFundamentalDefinition::LoadDB() {
	CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition;

	m_vTiingoFundamentalDefinition.clear();
	m_mapTiingoFundamentalDefinition.clear();

	setTiingoFundamentalDefinition.Open();
	setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
	while (!setTiingoFundamentalDefinition.IsEOF()) {
		if (!HaveDefinition(T2Utf8(setTiingoFundamentalDefinition.m_dataCode))) {
			CTiingoFundamentalDefinition pTiingoFundamentalDefinition;
			pTiingoFundamentalDefinition.Load(setTiingoFundamentalDefinition);
			Add(pTiingoFundamentalDefinition);
		}
		else { setTiingoFundamentalDefinition.Delete(); }
		setTiingoFundamentalDefinition.MoveNext();
	}
	setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
	setTiingoFundamentalDefinition.Close();
	m_fUpdated = false;

	return true;
}
