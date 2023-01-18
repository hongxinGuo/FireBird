#pragma once

#include"TiingoIndustry.h"

class CDataTiingoIndustry {
public:
	CDataTiingoIndustry();
	virtual ~CDataTiingoIndustry() = default;
	void Reset(void);

	size_t GetTotalTiingoIndustry(void) const noexcept { return m_vTiingoIndustry.size(); }
	long GetLastTotalTiingoIndustry(void) const noexcept { return m_lLastTotalTiingoIndustry; }

	bool IsTiingoIndustry(const CString& strTiingoIndustry) const {
		if (!m_mapTiingoIndustry.contains(strTiingoIndustry)) return true;
		return false;
	}

	bool IsTiingoIndustry(const CTiingoIndustryPtr& pTiingoIndustry) const { return IsTiingoIndustry(pTiingoIndustry->m_strIndustry + pTiingoIndustry->m_strSector); }

	void Add(CTiingoIndustryPtr pTiingoIndustry);
	bool Delete(CTiingoIndustryPtr pTiingoIndustry);

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CTiingoIndustryPtr> m_vTiingoIndustry;
	map<CString, size_t> m_mapTiingoIndustry;
	long m_lLastTotalTiingoIndustry;
};
