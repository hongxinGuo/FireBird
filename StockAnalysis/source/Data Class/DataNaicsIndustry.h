#pragma once

#include"NaicsIndustry.h"

class CDataNaicsIndustry final : public CObject {
public:
	CDataNaicsIndustry();
	~CDataNaicsIndustry() override = default;
	void Reset(void);

	size_t GetTotalNaicsIndustry(void) const noexcept { return m_vNaicsIndustry.size(); }
	long GetLastTotalNaicsIndustry(void) const noexcept { return m_lLastTotalNaicsIndustry; }

	bool IsNaicsIndustry(const CString& strNaicsIndustry) const {
		if (!m_mapNaicsIndustry.contains(strNaicsIndustry)) return true;
		else return false;
	}

	bool IsNaicsIndustry(const CNaicsIndustryPtr& pNaicsIndustry) const { return IsNaicsIndustry(pNaicsIndustry->m_strNaics); }
	void Add(CNaicsIndustryPtr pNaicsIndustry);
	bool Delete(CNaicsIndustryPtr pNaicsIndustry);

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CNaicsIndustryPtr> m_vNaicsIndustry;
	map<CString, size_t> m_mapNaicsIndustry;
	long m_lLastTotalNaicsIndustry;
};
