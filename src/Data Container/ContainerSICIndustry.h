#pragma once

#include"SICIndustry.h"

class CContainerSICIndustry {
public:
	CContainerSICIndustry();
	CContainerSICIndustry(const CContainerSICIndustry& other) = delete;
	CContainerSICIndustry(CContainerSICIndustry&& other) noexcept = delete;
	CContainerSICIndustry& operator=(const CContainerSICIndustry& other) = delete;
	CContainerSICIndustry& operator=(CContainerSICIndustry&& other) noexcept = delete;
	virtual ~CContainerSICIndustry() = default;
	void Reset();

	size_t GetTotalSICIndustry() const noexcept { return m_vSICIndustry.size(); }
	long GetLastTotalSICIndustry() const noexcept { return m_lLastTotalSICIndustry; }

	bool IsSICIndustry(const long lSICCode) const {
		if (!m_mapSICIndustry.contains(lSICCode)) return true;
		else return false;
	}

	bool IsSICIndustry(const CSICIndustryPtr& pSICIndustry) const { return IsSICIndustry(pSICIndustry->m_lCode); }
	void Add(CSICIndustryPtr pSICIndustry);
	bool Delete(CSICIndustryPtr pSICIndustry);

	bool UpdateDB();
	bool LoadDB();

protected:
	vector<CSICIndustryPtr> m_vSICIndustry;
	map<long, long> m_mapSICIndustry;
	long m_lLastTotalSICIndustry;
};
