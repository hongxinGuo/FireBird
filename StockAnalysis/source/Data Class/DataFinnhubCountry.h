#pragma once

#include"Country.h"

#include<vector>
#include<map>

class CDataFinnhubCountry final : public CObject {
public:
	CDataFinnhubCountry();
	~CDataFinnhubCountry() override = default;
	void Reset(void);

	size_t GetTotalCountry(void) const noexcept { return m_vCountry.size(); }
	long GetLastTotalCountry(void) const noexcept { return m_lLastTotalCountry; }

	bool IsCountry(CString strCountry) const noexcept {
		if (m_mapCountry.contains(strCountry)) return true;
		else return false;
	}

	bool IsCountry(CCountryPtr pCountry) const noexcept { return IsCountry(pCountry->m_strCountry); }
	void Add(CCountryPtr pCountry);
	bool Delete(CCountryPtr pCountry);
	CCountryPtr GetCountry(CString strCountry) { return m_vCountry.at(m_mapCountry.at(strCountry)); }

	bool UpdateDB(void);
	bool LoadDB(void);

protected:
	vector<CCountryPtr> m_vCountry;
	map<CString, long> m_mapCountry;
	long m_lLastTotalCountry;
};
