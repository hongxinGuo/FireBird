#pragma once

#include"SICIndustry.h"

#include<vector>
#include<map>

class CDataSICIndustry : public CObject {
public:
	CDataSICIndustry();
	~CDataSICIndustry();
	void Reset(void);

	size_t GetTotalSICIndustry(void) noexcept { return m_vSICIndustry.size(); }
	long GetLastTotalSICIndustry(void) noexcept { return m_lLastTotalSICIndustry; }

	bool IsSICIndustry(long lSICCode) {
		if (m_mapSICIndustry.find(lSICCode) == m_mapSICIndustry.end()) return false;
		else return true;
	}

	bool IsSICIndustry(CSICIndustryPtr pSICIndustry) { return IsSICIndustry(pSICIndustry->m_lCode); }
	void Add(CSICIndustryPtr pSICIndustry);
	bool Delete(CSICIndustryPtr pSICIndustry);

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CSICIndustryPtr> m_vSICIndustry;
	map<long, long> m_mapSICIndustry;
	long m_lLastTotalSICIndustry;
};
