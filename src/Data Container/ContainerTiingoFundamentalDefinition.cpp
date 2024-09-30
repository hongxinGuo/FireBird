#include "pch.h"

#include "ContainerTiingoFundamentalDefinition.h"
#include "InfoReport.h"

CContainerTiingoFundamentalDefinition::CContainerTiingoFundamentalDefinition() {
	Reset();
}

void CContainerTiingoFundamentalDefinition::Reset() {
	m_vTiingoFundamentalDefinition.resize(0);
	m_mapTiingoFundamentalDefinition.clear();
	m_lLastTotalTiingoFundamentalDefinition = 0;
}

void CContainerTiingoFundamentalDefinition::Add(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition) {
	m_mapTiingoFundamentalDefinition[pTiingoFundamentalDefinition->m_strDataCode] = m_vTiingoFundamentalDefinition.size();
	m_vTiingoFundamentalDefinition.push_back(pTiingoFundamentalDefinition);
}

bool CContainerTiingoFundamentalDefinition::Delete(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition) {
	if (pTiingoFundamentalDefinition == nullptr) return false;
	if (!IsDefinition(pTiingoFundamentalDefinition)) return false;

	m_vTiingoFundamentalDefinition.erase(m_vTiingoFundamentalDefinition.begin() + m_mapTiingoFundamentalDefinition.at(pTiingoFundamentalDefinition->m_strDataCode));
	m_mapTiingoFundamentalDefinition.erase(pTiingoFundamentalDefinition->m_strDataCode);

	return true;
}

bool CContainerTiingoFundamentalDefinition::UpdateDB() {
	if (m_lLastTotalTiingoFundamentalDefinition < m_vTiingoFundamentalDefinition.size()) {
		CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition;
		try {
			setTiingoFundamentalDefinition.Open();
			setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
			for (long l = m_lLastTotalTiingoFundamentalDefinition; l < m_vTiingoFundamentalDefinition.size(); l++) {
				const CTiingoFundamentalDefinitionPtr pTiingoFundamentalDefinition = m_vTiingoFundamentalDefinition.at(l);
				ASSERT(pTiingoFundamentalDefinition != nullptr);
				pTiingoFundamentalDefinition->Append(setTiingoFundamentalDefinition);
			}
			setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
			setTiingoFundamentalDefinition.Close();
			m_lLastTotalTiingoFundamentalDefinition = m_vTiingoFundamentalDefinition.size();
		} catch (CException* e) {
			ReportInformationAndDeleteException(e);
			setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
			setTiingoFundamentalDefinition.Close();
		}
	}
	return true;
}

bool CContainerTiingoFundamentalDefinition::LoadDB() {
	CSetTiingoFundamentalDefinition setTiingoFundamentalDefinition;
	CString strSymbol = _T("");

	setTiingoFundamentalDefinition.Open();
	setTiingoFundamentalDefinition.m_pDatabase->BeginTrans();
	while (!setTiingoFundamentalDefinition.IsEOF()) {
		if (!IsDefinition(setTiingoFundamentalDefinition.m_dataCode)) {
			const auto pTiingoFundamentalDefinition = make_shared<CTiingoFundamentalDefinition>();
			pTiingoFundamentalDefinition->Load(setTiingoFundamentalDefinition);
			Add(pTiingoFundamentalDefinition);
		}
		else { setTiingoFundamentalDefinition.Delete(); }
		setTiingoFundamentalDefinition.MoveNext();
	}
	setTiingoFundamentalDefinition.m_pDatabase->CommitTrans();
	setTiingoFundamentalDefinition.Close();
	m_lLastTotalTiingoFundamentalDefinition = m_vTiingoFundamentalDefinition.size();

	return true;
}
