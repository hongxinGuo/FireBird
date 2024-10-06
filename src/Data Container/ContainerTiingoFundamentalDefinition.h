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

	void Add(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition);
	bool Delete(const CTiingoFundamentalDefinitionPtr& pTiingoFundamentalDefinition);
	bool Delete(const CString& strDataCode);
	size_t GetTotalDefinition() const noexcept { return m_vTiingoFundamentalDefinition.size(); }

	bool HaveDefinition(const CString& strDataCode) const noexcept { return m_mapTiingoFundamentalDefinition.contains(strDataCode); }
	bool HaveDefinition(const CTiingoFundamentalDefinitionPtr& pDefinition) const { return HaveDefinition(pDefinition->m_strDataCode); }

	CTiingoFundamentalDefinitionPtr GetFundamentalDefinition(const size_t lIndex) const { return m_vTiingoFundamentalDefinition.at(lIndex); }
	CTiingoFundamentalDefinitionPtr GetFundamentalDefinition(const CString& strDataCode) const { return m_vTiingoFundamentalDefinition.at(m_mapTiingoFundamentalDefinition.at(strDataCode)); }

	bool UpdateDB();
	bool LoadDB();

	bool IsUpdateDB() const noexcept { return m_fUpdated; }
	void SetUpdateDB(bool fFlag) noexcept { m_fUpdated = fFlag; }

protected:
	vector<CTiingoFundamentalDefinitionPtr> m_vTiingoFundamentalDefinition;
	map<CString, size_t> m_mapTiingoFundamentalDefinition;

	bool m_fUpdated{ false };
};
