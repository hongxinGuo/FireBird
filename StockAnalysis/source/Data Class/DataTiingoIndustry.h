#pragma once

#include"TiingoIndustry.h"

#include<vector>
#include<map>

class CDataTiingoIndustry : public CObject {
public:
	CDataTiingoIndustry();
	~CDataTiingoIndustry() override;
	void Reset(void);

	size_t GetTotalTiingoIndustry(void) const noexcept { return m_vTiingoIndustry.size(); }
	long GetLastTotalTiingoIndustry(void) const noexcept { return m_lLastTotalTiingoIndustry; }

	bool IsTiingoIndustry(const CString &strTiingoIndustry) const {
		if (!m_mapTiingoIndustry.contains(strTiingoIndustry)) return true;
		else return false;
	}

	bool IsTiingoIndustry(const CTiingoIndustryPt &pTiingoIndustry) const { return IsTiingoIndustry(pTiingoIndustry->m_strIndustry + pTiingoIndustry->m_strSector); }

	void Add(CTiingoIndustryPtr pTiingoIndustry);
	bool Delete(CTiingoIndustryPtr pTiingoIndustry);

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CTiingoIndustryPtr> m_vTiingoIndustry;
	map<CString, long> m_mapTiingoIndustry;
	long m_lLastTotalTiingoIndustry;
};
