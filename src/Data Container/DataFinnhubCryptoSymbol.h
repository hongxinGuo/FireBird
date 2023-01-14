#pragma once

#include"FinnhubCryptoSymbol.h"

class CDataFinnhubCryptoSymbol {
public:
	CDataFinnhubCryptoSymbol();
	virtual ~CDataFinnhubCryptoSymbol() = default;
	void Reset(void);

	[[nodiscard]] bool IsFinnhubCryptoSymbol(const CString& strSymbol) const noexcept {
		if (m_mapCryptoSymbol.contains(strSymbol)) return true;
		else return false;
	}

	[[nodiscard]] size_t GetCryptoSymbolSize(void) const noexcept { return m_vCryptoSymbol.size(); }
	[[nodiscard]] CFinnhubCryptoSymbolPtr GetCryptoSymbol(const size_t lIndex) const { return m_vCryptoSymbol.at(lIndex); }

	[[nodiscard]] CFinnhubCryptoSymbolPtr GetCryptoSymbol(const CString& strSymbol) const { return GetCryptoSymbol(m_mapCryptoSymbol.at(strSymbol)); }

	bool Delete(CFinnhubCryptoSymbolPtr pCryptoSymbol);
	void Add(CFinnhubCryptoSymbolPtr pCryptoSymbol);

	bool LoadDB(void);
	bool UpdateDB(void);

	bool IsNeedUpdate(void);

protected:
	vector<CFinnhubCryptoSymbolPtr> m_vCryptoSymbol;
	map<CString, size_t> m_mapCryptoSymbol;
	long m_lLastTotalCryptoSymbol;
};
