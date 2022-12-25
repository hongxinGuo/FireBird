#pragma once

//#include"WorldStock.h"
#include <TiingoCryptoSymbol.h>

#include<vector>
#include<map>

class CDataTiingoCryptoSymbol final : public CObject {
public:
	CDataTiingoCryptoSymbol();
	~CDataTiingoCryptoSymbol() override = default;
	void Reset(void);

	void Add(CTiingoCryptoSymbolPtr pTiingoCrypto);
	bool Delete(CTiingoCryptoSymbolPtr pCrypto);
	size_t GetCryptoSymbolSize(void) const noexcept { return m_vTiingoCrypto.size(); }
	long GetLastCryptoSymbolSize(void) const noexcept { return m_lLastTotalTiingoCrypto; }

	bool IsTiingoCryptoSymbol(const CString &strSymbol) const {
		if (m_mapTiingoCrypto.contains(strSymbol)) return true;
		else return false;
	}

	bool IsTiingoCryptoSymbol(const CTiingoCryptoSymbolPtr &pCrypto) const { return IsTiingoCryptoSymbol(pCrypto->m_strTicker); }
	CTiingoCryptoSymbolPtr GetCryptoSymbol(const long lIndex) const { return m_vTiingoCrypto.at(lIndex); }

	CTiingoCryptoSymbolPtr GetCryptoSymbol(const CString &strTicker) const { return m_vTiingoCrypto.at(m_mapTiingoCrypto.at(strTicker)); }

	bool UpdateDB(void);
	bool LoadDB(void);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalTiingoCrypto < m_vTiingoCrypto.size()) return true;
		else return false;
	}

protected:
	vector<CTiingoCryptoSymbolPtr> m_vTiingoCrypto;
	map<CString, long> m_mapTiingoCrypto;
	long m_lLastTotalTiingoCrypto;
};
