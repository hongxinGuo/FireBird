#include "pch.h"

#include "ContainerTiingoFundamentalDefinition.h"
#include "InfoReport.h"

CContainerTiingoFundamentalDefinition::CContainerTiingoFundamentalDefinition() {
	m_vTiingoFundamentalDefinition.resize(0);
	m_mapTiingoFundamentalDefinition.clear();
	m_fUpdated = false;
}

void CContainerTiingoFundamentalDefinition::Add(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition) {
	if (!HaveDefinition(pTiingoFundamentalDefinition)) {
		m_mapTiingoFundamentalDefinition[pTiingoFundamentalDefinition->m_strDataCode] = m_vTiingoFundamentalDefinition.size();
		m_vTiingoFundamentalDefinition.push_back(pTiingoFundamentalDefinition);
		m_fUpdated = true;
	}
}

bool CContainerTiingoFundamentalDefinition::Delete(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition) {
	if (pTiingoFundamentalDefinition == nullptr) return false;
	if (!HaveDefinition(pTiingoFundamentalDefinition)) return false;

	m_vTiingoFundamentalDefinition.erase(m_vTiingoFundamentalDefinition.begin() + m_mapTiingoFundamentalDefinition.at(pTiingoFundamentalDefinition->m_strDataCode));
	m_mapTiingoFundamentalDefinition.erase(pTiingoFundamentalDefinition->m_strDataCode);
	m_fUpdated = true;

	return true;
}

bool CContainerTiingoFundamentalDefinition::Delete(const CString& strDataCode) {
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
		map<CString, size_t> mapDefinition;
		setDefinition.Open();
		setDefinition.m_pDatabase->BeginTrans();
		while (!setDefinition.IsEOF()) {
			mapDefinition[setDefinition.m_dataCode] = mapDefinition.size();
			setDefinition.MoveNext();
		}
		setDefinition.m_pDatabase->CommitTrans();
		setDefinition.Close();

		setTiingoFundamentalDefinition.Open();
		setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
		try {
			for (long l = 0; l < m_vTiingoFundamentalDefinition.size(); l++) {
				const CTiingoFundamentalDefinitionPtr pTiingoFundamentalDefinition = m_vTiingoFundamentalDefinition.at(l);
				ASSERT(pTiingoFundamentalDefinition != nullptr);
				if (!mapDefinition.contains(pTiingoFundamentalDefinition->m_strDataCode)) { // 只添加新增的项目。
					pTiingoFundamentalDefinition->Append(setTiingoFundamentalDefinition);
				}
			}
		} catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
		setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
		setTiingoFundamentalDefinition.Close();
		m_fUpdated = false;
	}
	return true;
}

bool CContainerTiingoFundamentalDefinition::LoadDB() {
	CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition;
	CString strSymbol = _T("");

	m_vTiingoFundamentalDefinition.clear();
	m_mapTiingoFundamentalDefinition.clear();

	setTiingoFundamentalDefinition.Open();
	setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
	while (!setTiingoFundamentalDefinition.IsEOF()) {
		if (!HaveDefinition(setTiingoFundamentalDefinition.m_dataCode)) {
			const auto pTiingoFundamentalDefinition = make_shared<CTiingoFundamentalDefinition>();
			pTiingoFundamentalDefinition->Load(setTiingoFundamentalDefinition);
			Add(pTiingoFundamentalDefinition);
		}
		else { setTiingoFundamentalDefinition.Delete(); }
		setTiingoFundamentalDefinition.MoveNext();
	}
	setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
	setTiingoFundamentalDefinition.Close();
	m_fUpdated = false; // Note 

	return true;
}
