#pragma once

#include"TiingoIndustry.h"

#include<vector>
#include<map>

class CDataTiingoIndustry : public CObject {
public:
	CDataTiingoIndustry();
	~CDataTiingoIndustry();
	void Reset(void);

	size_t GetTotalTiingoIndustry(void) noexcept { return m_vTiingoIndustry.size(); }
	long GetLastTotalTiingoIndustry(void) noexcept { return m_lLastTotalTiingoIndustry; }

	bool IsTiingoIndustry(CString strTiingoIndustry) {
		if (m_mapTiingoIndustry.find(strTiingoIndustry) == m_mapTiingoIndustry.end()) return false;
		else return true;
	}

	bool IsTiingoIndustry(CTiingoIndustryPtr pTiingoIndustry) {
		return IsTiingoIndustry(pTiingoIndustry->m_strIndustry + pTiingoIndustry->m_strSector);
	}

	void Add(CTiingoIndustryPtr pTiingoIndustry);
	bool Delete(CTiingoIndustryPtr pTiingoIndustry);

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CTiingoIndustryPtr> m_vTiingoIndustry;
	map<CString, long> m_mapTiingoIndustry;
	long m_lLastTotalTiingoIndustry;
};
