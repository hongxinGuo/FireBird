#pragma once

#include <TiingoCryptoSymbol.h>

class CDataTiingoCryptoSymbol final {
public:
	CDataTiingoCryptoSymbol();
	virtual ~CDataTiingoCryptoSymbol() = default;
	void Reset(void);

	void Add(CTiingoCryptoSymbolPtr pTiingoCrypto);
	bool Delete(CTiingoCryptoSymbolPtr pCrypto);
	size_t GetCryptoSymbolSize(void) const noexcept { return m_vTiingoCrypto.size(); }
	long GetLastCryptoSymbolSize(void) const noexcept { return m_lLastTotalTiingoCrypto; }

	bool IsTiingoCryptoSymbol(const CString& strSymbol) const {
		if (m_mapTiingoCrypto.contains(strSymbol)) return true;
		else return false;
	}

	bool IsTiingoCryptoSymbol(const CTiingoCryptoSymbolPtr& pCrypto) const { return IsTiingoCryptoSymbol(pCrypto->m_strTicker); }
	CTiingoCryptoSymbolPtr GetCryptoSymbol(const long lIndex) const { return m_vTiingoCrypto.at(lIndex); }

	CTiingoCryptoSymbolPtr GetCryptoSymbol(const CString& strTicker) const { return m_vTiingoCrypto.at(m_mapTiingoCrypto.at(strTicker)); }

	bool UpdateDB(void);
	bool LoadDB(void);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalTiingoCrypto < m_vTiingoCrypto.size()) return true;
		else return false;
	}

protected:
	vector<CTiingoCryptoSymbolPtr> m_vTiingoCrypto;
	map<CString, size_t> m_mapTiingoCrypto;
	long m_lLastTotalTiingoCrypto;
};
