#pragma once

#include"NaicsIndustry.h"

#include<vector>
#include<map>

class CDataNaicsIndustry final : public CObject {
public:
	CDataNaicsIndustry();
	~CDataNaicsIndustry() override = default;
	void Reset(void);

	size_t GetTotalNaicsIndustry(void) noexcept { return m_vNaicsIndustry.size(); }
	long GetLastTotalNaicsIndustry(void) noexcept { return m_lLastTotalNaicsIndustry; }

	bool IsNaicsIndustry(CString strNaicsIndustry) {
		if (m_mapNaicsIndustry.find(strNaicsIndustry) == m_mapNaicsIndustry.end()) return false;
		else return true;
	}

	bool IsNaicsIndustry(CNaicsIndustryPtr pNaicsIndustry) { return IsNaicsIndustry(pNaicsIndustry->m_strNaics); }
	void Add(CNaicsIndustryPtr pNaicsIndustry);
	bool Delete(CNaicsIndustryPtr pNaicsIndustry);

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CNaicsIndustryPtr> m_vNaicsIndustry;
	map<CString, long> m_mapNaicsIndustry;
	long m_lLastTotalNaicsIndustry;
};
