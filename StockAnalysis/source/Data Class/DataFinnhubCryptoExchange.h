#pragma once

using namespace std;
#include<vector>

class CDataFinnhubCryptoExchange : CObject {
public:
	CDataFinnhubCryptoExchange();
	~CDataFinnhubCryptoExchange();
	void Reset(void);

	bool IsCryptoExchange(CString strExchange) { if (m_mapCryptoExchange.find(strExchange) == m_mapCryptoExchange.end()) return false; else return true; }
	size_t GetCryptoExchangeSize(void) noexcept { return m_vCryptoExchange.size(); }
	CString GetCryptoExchange(long lIndex) { return m_vCryptoExchange.at(lIndex); }
	bool Delete(CString strforexExchange);
	void Add(CString strCryptoExchange);

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	vector<CString> m_vCryptoExchange;
	map<CString, long> m_mapCryptoExchange;
	long m_lLastTotalCryptoExchange;
};
