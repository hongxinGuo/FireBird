#pragma once

#include "TiingoCrypto.h"

class CContainerTiingoCryptoSymbol final {
public:
	CContainerTiingoCryptoSymbol();
	CContainerTiingoCryptoSymbol(const CContainerTiingoCryptoSymbol& other) = delete;
	CContainerTiingoCryptoSymbol(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
	CContainerTiingoCryptoSymbol& operator=(const CContainerTiingoCryptoSymbol& other) = delete;
	CContainerTiingoCryptoSymbol& operator=(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
	~CContainerTiingoCryptoSymbol() = default;
	void Reset();

	void Add(const CTiingoCryptoPtr& pTiingoCrypto);
	bool Delete(const CTiingoCryptoPtr& pCrypto);
	size_t Size() const noexcept { return m_vTiingoCrypto.size(); }
	long GetLastSize() const noexcept { return m_lLastTotalTiingoCrypto; }

	bool IsSymbol(const CString& strSymbol) const {
		if (m_mapTiingoCrypto.contains(strSymbol)) return true;
		return false;
	}

	bool IsSymbol(const CTiingoCryptoPtr& pCrypto) const { return IsSymbol(pCrypto->GetSymbol()); }
	CTiingoCryptoPtr GetSymbol(const long lIndex) const { return m_vTiingoCrypto.at(lIndex); }

	CTiingoCryptoPtr GetSymbol(const CString& strTicker) const { return m_vTiingoCrypto.at(m_mapTiingoCrypto.at(strTicker)); }

	bool UpdateDB();
	bool LoadDB();

	bool IsNeedUpdate() const noexcept {
		if (m_lLastTotalTiingoCrypto < m_vTiingoCrypto.size()) return true;
		return false;
	}

protected:
	vector<CTiingoCryptoPtr> m_vTiingoCrypto;
	map<CString, size_t> m_mapTiingoCrypto;
	long m_lLastTotalTiingoCrypto;
};
