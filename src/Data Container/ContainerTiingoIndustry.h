#pragma once

#include"TiingoIndustry.h"

class CContainerTiingoIndustry {
public:
	CContainerTiingoIndustry();
	CContainerTiingoIndustry(const CContainerTiingoIndustry& other) = delete;
	CContainerTiingoIndustry(CContainerTiingoIndustry&& other) noexcept = delete;
	CContainerTiingoIndustry& operator=(const CContainerTiingoIndustry& other) = delete;
	CContainerTiingoIndustry& operator=(CContainerTiingoIndustry&& other) noexcept = delete;
	virtual ~CContainerTiingoIndustry() = default;
	void Reset();

	size_t GetTotalTiingoIndustry() const noexcept { return m_vTiingoIndustry.size(); }
	auto GetLastTotalTiingoIndustry() const noexcept { return m_lLastTotalTiingoIndustry; }

	bool IsTiingoIndustry(const string& strTiingoIndustry) const { return !m_mapTiingoIndustry.contains(strTiingoIndustry); }
	bool IsTiingoIndustry(const CTiingoIndustryPtr& pTiingoIndustry) const { return IsTiingoIndustry(pTiingoIndustry->m_strIndustry + pTiingoIndustry->m_strSector); }

	void Add(CTiingoIndustryPtr pTiingoIndustry);
	bool Delete(CTiingoIndustryPtr pTiingoIndustry);

	bool UpdateDB();
	bool LoadDB();

protected:
	vector<CTiingoIndustryPtr> m_vTiingoIndustry;
	map<string, size_t> m_mapTiingoIndustry;
	size_t m_lLastTotalTiingoIndustry;
};
