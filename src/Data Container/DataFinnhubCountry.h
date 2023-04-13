#pragma once

#include"Country.h"

class CDataFinnhubCountry final {
public:
	CDataFinnhubCountry();
	~CDataFinnhubCountry() = default;
	void Reset();

	size_t GetTotalCountry() const noexcept { return m_vCountry.size(); }
	long GetLastTotalCountry() const noexcept { return m_lLastTotalCountry; }

	bool IsCountry(const CString& strCountry) const noexcept {
		if (m_mapCountry.contains(strCountry)) return true;
		return false;
	}

	bool IsCountry(const CCountryPtr& pCountry) const noexcept { return IsCountry(pCountry->m_strCountry); }
	void Add(const CCountryPtr& pCountry);
	bool Delete(const CCountryPtr& pCountry);
	CCountryPtr GetCountry(const CString& strCountry) { return m_vCountry.at(m_mapCountry.at(strCountry)); }

	void UpdateDB();
	bool LoadDB();

protected:
	vector<CCountryPtr> m_vCountry;
	map<CString, size_t> m_mapCountry;
	long m_lLastTotalCountry;
};
