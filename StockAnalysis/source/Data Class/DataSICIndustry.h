#pragma once

#include"SICIndustry.h"

#include<vector>
#include<map>

class CDataSICIndustry : public CObject {
public:
	CDataSICIndustry();
	~CDataSICIndustry() override;
	void Reset(void);

	size_t GetTotalSICIndustry(void) const noexcept { return m_vSICIndustry.size(); }
	long GetLastTotalSICIndustry(void) const noexcept { return m_lLastTotalSICIndustry; }

	bool IsSICIndustry(const long lSICCode) const {
		if (!m_mapSICIndustry.contains(lSICCode)) return true;
		else return false;
	}

	bool IsSICIndustry(const CSICIndustryPtr &pSICIndustry) const { return IsSICIndustry(pSICIndustry->m_lCode); }
	void Add(CSICIndustryPtr pSICIndustry);
	bool Delete(CSICIndustryPtr pSICIndustry);

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CSICIndustryPtr> m_vSICIndustry;
	map<long, long> m_mapSICIndustry;
	long m_lLastTotalSICIndustry;
};
