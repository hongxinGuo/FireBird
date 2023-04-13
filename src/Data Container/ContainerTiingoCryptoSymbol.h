#pragma once

#include <TiingoCryptoSymbol.h>

class CContainerTiingoCryptoSymbol final {
public:
	CContainerTiingoCryptoSymbol();
	~CContainerTiingoCryptoSymbol() = default;
	void Reset();

	void Add(const CTiingoCryptoSymbolPtr& pTiingoCrypto);
	bool Delete(const CTiingoCryptoSymbolPtr& pCrypto);
	size_t GetCryptoSymbolSize() const noexcept { return m_vTiingoCrypto.size(); }
	long GetLastCryptoSymbolSize() const noexcept { return m_lLastTotalTiingoCrypto; }

	bool IsTiingoCryptoSymbol(const CString& strSymbol) const {
		if (m_mapTiingoCrypto.contains(strSymbol)) return true;
		return false;
	}

	bool IsTiingoCryptoSymbol(const CTiingoCryptoSymbolPtr& pCrypto) const { return IsTiingoCryptoSymbol(pCrypto->m_strTicker); }
	CTiingoCryptoSymbolPtr GetCryptoSymbol(const long lIndex) const { return m_vTiingoCrypto.at(lIndex); }

	CTiingoCryptoSymbolPtr GetCryptoSymbol(const CString& strTicker) const { return m_vTiingoCrypto.at(m_mapTiingoCrypto.at(strTicker)); }

	bool UpdateDB();
	bool LoadDB();

	bool IsNeedUpdate() const noexcept {
		if (m_lLastTotalTiingoCrypto < m_vTiingoCrypto.size()) return true;
		return false;
	}

protected:
	vector<CTiingoCryptoSymbolPtr> m_vTiingoCrypto;
	map<CString, size_t> m_mapTiingoCrypto;
	long m_lLastTotalTiingoCrypto;
};
