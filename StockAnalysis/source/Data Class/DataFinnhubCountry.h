#pragma once

#include"Country.h"

using namespace std;
#include<vector>
#include<map>

class CDataFinnhubCountry : public CObject {
public:
	CDataFinnhubCountry();
	~CDataFinnhubCountry();
	void Reset(void);

	size_t GetTotalCountry(void) noexcept { return m_vCountry.size(); }
	long GetLastTotalCountry(void) noexcept { return m_lLastTotalCountry; }
	bool IsCountry(CString strCountry) { if (m_mapCountry.find(strCountry) == m_mapCountry.end()) return false;	else return true; }
	bool IsCountry(CCountryPtr pCountry) { return IsCountry(pCountry->m_strCountry); }
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
