#pragma once

//#include"WorldStock.h"
#include <TiingoCryptoSymbol.h>

using namespace std;
#include<vector>
#include<map>

class CDataTiingoCryptoSymbol : public CObject {
public:
	CDataTiingoCryptoSymbol();
	~CDataTiingoCryptoSymbol();
	void Reset(void);

	void Add(CTiingoCryptoSymbolPtr pTiingoCrypto);
	bool Delete(CTiingoCryptoSymbolPtr pCrypto);
	size_t GetCryptoSymbolSize(void) noexcept { return m_vTiingoCrypto.size(); }
	long GetLastCryptoSymbolSize(void) noexcept { return m_lLastTotalTiingoCrypto; }
	bool IsTiingoCryptoSymbol(CString strSymbol) { if (m_mapTiingoCrypto.find(strSymbol) == m_mapTiingoCrypto.end()) return false; else return true; }
	bool IsTiingoCryptoSymbol(CTiingoCryptoSymbolPtr pCrypto) { return IsTiingoCryptoSymbol(pCrypto->m_strTicker); }
	CTiingoCryptoSymbolPtr GetCryptoSymbol(long lIndex) { return m_vTiingoCrypto.at(lIndex); }
	CTiingoCryptoSymbolPtr GetCryptoSymbol(CString strTicker) { return m_vTiingoCrypto.at(m_mapTiingoCrypto.at(strTicker)); }

	bool UpdateDB(void);
	bool LoadDB(void);

	bool IsNeedUpdate(void) noexcept { if (m_lLastTotalTiingoCrypto < m_vTiingoCrypto.size()) return true; else return false; }

protected:
	vector<CTiingoCryptoSymbolPtr> m_vTiingoCrypto;
	map<CString, long> m_mapTiingoCrypto;
	long m_lLastTotalTiingoCrypto;
};
