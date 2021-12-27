#pragma once

#include"FinnhubCryptoSymbol.h"

using namespace std;
#include<vector>

class CDataFinnhubCryptoSymbol : public CObject {
public:
	CDataFinnhubCryptoSymbol();
	~CDataFinnhubCryptoSymbol();
	void Reset(void);

	bool IsCryptoSymbol(CString strSymbol) { if (m_mapCryptoSymbol.find(strSymbol) == m_mapCryptoSymbol.end()) return false; else return true; }
	size_t GetCryptoSymbolSize(void) noexcept { return m_vCryptoSymbol.size(); }
	CCryptoSymbolPtr GetCryptoSymbol(long lIndex) { return m_vCryptoSymbol.at(lIndex); }
	CCryptoSymbolPtr GetCryptoSymbol(CString strSymbol) { return GetCryptoSymbol(m_mapCryptoSymbol.at(strSymbol)); }
	bool Delete(CCryptoSymbolPtr pSymbol);
	void Add(CCryptoSymbolPtr pSymbol);

	bool LoadDB(void);
	bool UpdateDB(void);

protected:
	vector<CCryptoSymbolPtr> m_vCryptoSymbol;
	map<CString, long> m_mapCryptoSymbol;
	long m_lLastTotalCryptoSymbol;
};
