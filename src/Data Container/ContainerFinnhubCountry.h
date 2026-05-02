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
	bool IsCountry(const CCountry& country) const noexcept { return IsCountry(country.m_strCountry); }
	void Add(const CCountry& pCountry);
	bool Delete(const CCountry& country);
	CCountry GetCountry(const string& strCountry) { return m_vCountry.at(m_mapCountry.at(strCountry)); }

	void UpdateDB();
	bool LoadDB();

protected:
	vector<CCountry> m_vCountry;
	map<string, size_t> m_mapCountry;
	size_t m_llLastTotalCountry;
};
