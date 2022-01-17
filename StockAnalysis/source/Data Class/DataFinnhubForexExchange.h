#pragma once

using namespace std;
#include<vector>
#include<map>

class CDataFinnhubForexExchange : public CObject {
public:
	CDataFinnhubForexExchange();
	~CDataFinnhubForexExchange();
	void Reset(void);

	bool IsForexExchange(CString strExchange) { if (m_mapForexExchange.find(strExchange) == m_mapForexExchange.end()) return false; else return true; }
	size_t GetForexExchangeSize(void) noexcept { return m_vForexExchange.size(); }
	CString GetForexExchange(long lIndex) { return m_vForexExchange.at(lIndex); }
	bool Delete(CString strforexExchange);
	void Add(CString strForexExchange);

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	vector<CString> m_vForexExchange;
	map<CString, long> m_mapForexExchange;
	long m_lLastTotalForexExchange;
};
