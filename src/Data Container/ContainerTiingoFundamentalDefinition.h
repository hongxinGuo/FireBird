#pragma once

using std::map;

#include"TiingoFundamentalDefinition.h"

class CContainerTiingoFundamentalDefinition final {
public:
	CContainerTiingoFundamentalDefinition();
	CContainerTiingoFundamentalDefinition(const CContainerTiingoFundamentalDefinition& other) = delete;
	CContainerTiingoFundamentalDefinition(CContainerTiingoFundamentalDefinition&& other) noexcept = delete;
	CContainerTiingoFundamentalDefinition& operator=(const CContainerTiingoFundamentalDefinition& other) = delete;
	CContainerTiingoFundamentalDefinition& operator=(CContainerTiingoFundamentalDefinition&& other) noexcept = delete;
	~CContainerTiingoFundamentalDefinition() = default;
	void Reset();

	void Add(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition);
	bool Delete(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition);
	size_t GetTotalDefinition() const noexcept { return m_vTiingoFundamentalDefinition.size(); }

	bool IsDefinition(const CString& strDataCode) const noexcept {
		if (m_mapTiingoFundamentalDefinition.contains(strDataCode)) return true;
		return false;
	}

	bool IsDefinition(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition) const { return IsDefinition(pTiingoFundamentalDefinition->m_strDataCode); }
	CTiingoFundamentalDefinitionPtr GetFundamentalDefinition(const size_t lIndex) const { return m_vTiingoFundamentalDefinition.at(lIndex); }
	CTiingoFundamentalDefinitionPtr GetFundamentalDefinition(const CString& strDataCode) const { return m_vTiingoFundamentalDefinition.at(m_mapTiingoFundamentalDefinition.at(strDataCode)); }

	bool UpdateDB();
	bool LoadDB();

	bool IsNeedUpdate() const noexcept { return m_lLastTotalTiingoFundamentalDefinition < m_vTiingoFundamentalDefinition.size(); }

protected:
	vector<CTiingoFundamentalDefinitionPtr> m_vTiingoFundamentalDefinition;
	map<CString, size_t> m_mapTiingoFundamentalDefinition;
	long m_lLastTotalTiingoFundamentalDefinition{ 0 };
};
