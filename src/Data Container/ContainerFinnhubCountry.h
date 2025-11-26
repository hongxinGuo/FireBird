#pragma once

#include"Country.h"

class CContainerFinnhubCountry final {
public:
	CContainerFinnhubCountry();
	CContainerFinnhubCountry(const CContainerFinnhubCountry& other) = delete;
	CContainerFinnhubCountry(CContainerFinnhubCountry&& other) noexcept = delete;
	CContainerFinnhubCountry& operator=(const CContainerFinnhubCountry& other) = delete;
	CContainerFinnhubCountry& operator=(CContainerFinnhubCountry&& other) noexcept = delete;
	~CContainerFinnhubCountry() = default;
	void Reset();

	auto GetTotalCountry() const noexcept { return m_vCountry.size(); }
	auto GetLastTotalCountry() const noexcept { return m_llLastTotalCountry; }

	bool IsCountry(const string& strCountry) const noexcept { return m_mapCountry.contains(strCountry); }
	bool IsCountry(const CCountryPtr& pCountry) const noexcept { return IsCountry(pCountry->m_strCountry); }
	void Add(const CCountryPtr& pCountry);
	bool Delete(const CCountryPtr& pCountry);
	CCountryPtr GetCountry(const string& strCountry) { return m_vCountry.at(m_mapCountry.at(strCountry)); }

	void UpdateDB();
	bool LoadDB();

protected:
	vector<CCountryPtr> m_vCountry;
	map<string, size_t> m_mapCountry;
	size_t m_llLastTotalCountry;
};
