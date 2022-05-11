#pragma once

#include"FinnhubCryptoSymbol.h"

using namespace std;
#include<vector>

class CDataFinnhubCryptoSymbol : public CObject {
public:
	CDataFinnhubCryptoSymbol();
	~CDataFinnhubCryptoSymbol();
	void Reset(void);

	bool IsFinnhubCryptoSymbol(CString strSymbol) { if (m_mapCryptoSymbol.find(strSymbol) == m_mapCryptoSymbol.end()) return false; else return true; }
	size_t GetCryptoSymbolSize(void) noexcept { return m_vCryptoSymbol.size(); }
	CFinnhubCryptoSymbolPtr GetCryptoSymbol(long lIndex) { return m_vCryptoSymbol.at(lIndex); }
	CFinnhubCryptoSymbolPtr GetCryptoSymbol(CString strSymbol) { return GetCryptoSymbol(m_mapCryptoSymbol.at(strSymbol)); }
	bool Delete(CFinnhubCryptoSymbolPtr pSymbol);
	void Add(CFinnhubCryptoSymbolPtr pSymbol);

	bool LoadDB(void);
	bool UpdateDB(void);

	bool IsNeedUpdate(void);

protected:
	vector<CFinnhubCryptoSymbolPtr> m_vCryptoSymbol;
	map<CString, long> m_mapCryptoSymbol;
	long m_lLastTotalCryptoSymbol;
};
