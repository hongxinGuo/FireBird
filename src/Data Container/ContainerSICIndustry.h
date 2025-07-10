#pragma once

#include"SicIndustry.h"

class CContainerSicIndustry {
public:
	CContainerSicIndustry();
	CContainerSicIndustry(const CContainerSicIndustry& other) = delete;
	CContainerSicIndustry(CContainerSicIndustry&& other) noexcept = delete;
	CContainerSicIndustry& operator=(const CContainerSicIndustry& other) = delete;
	CContainerSicIndustry& operator=(CContainerSicIndustry&& other) noexcept = delete;
	virtual ~CContainerSicIndustry() = default;
	void Reset();

	size_t GetTotalSicIndustry() const noexcept { return m_vSicIndustry.size(); }
	long GetLastTotalSicIndustry() const noexcept { return m_lLastTotalSicIndustry; }

	bool IsSicIndustry(const long lSicCode) const {
		if (!m_mapSicIndustry.contains(lSicCode)) return true;
		else return false;
	}

	bool IsSicIndustry(const CSICIndustryPtr& pSicIndustry) const { return IsSicIndustry(pSicIndustry->m_lCode); }
	void Add(CSICIndustryPtr pSicIndustry);
	bool Delete(CSICIndustryPtr pSicIndustry);

	bool UpdateDB();
	bool LoadDB();

protected:
	vector<CSICIndustryPtr> m_vSicIndustry;
	map<long, long> m_mapSicIndustry;
	long m_lLastTotalSicIndustry;
};
