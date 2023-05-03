#pragma once

#include"NaicsIndustry.h"

class CContainerNaicsIndustry final {
public:
	CContainerNaicsIndustry();
	~CContainerNaicsIndustry() = default;
	void Reset();

	size_t GetTotalNaicsIndustry() const noexcept { return m_vNaicsIndustry.size(); }
	long GetLastTotalNaicsIndustry() const noexcept { return m_lLastTotalNaicsIndustry; }

	bool IsNaicsIndustry(const CString& strNaicsIndustry) const {
		if (!m_mapNaicsIndustry.contains(strNaicsIndustry)) return true;
		return false;
	}

	bool IsNaicsIndustry(const CNaicsIndustryPtr& pNaicsIndustry) const { return IsNaicsIndustry(pNaicsIndustry->m_strNaics); }
	void Add(CNaicsIndustryPtr pNaicsIndustry);
	bool Delete(CNaicsIndustryPtr pNaicsIndustry);

	bool UpdateDB();
	bool LoadDB();

protected:
	vector<CNaicsIndustryPtr> m_vNaicsIndustry;
	map<CString, size_t> m_mapNaicsIndustry;
	long m_lLastTotalNaicsIndustry;
};
