#pragma once

#include"FinnhubCryptoSymbol.h"

class CDataFinnhubCryptoSymbol {
public:
	CDataFinnhubCryptoSymbol();
	virtual ~CDataFinnhubCryptoSymbol() = default;
	void Reset(void);

	bool IsFinnhubCryptoSymbol(const CString& strSymbol) const noexcept {
		if (m_mapCryptoSymbol.contains(strSymbol)) return true;
		else return false;
	}

	size_t GetCryptoSymbolSize(void) const noexcept { return m_vCryptoSymbol.size(); }
	CFinnhubCryptoSymbolPtr GetCryptoSymbol(const long lIndex) const { return m_vCryptoSymbol.at(lIndex); }

	CFinnhubCryptoSymbolPtr GetCryptoSymbol(const CString& strSymbol) const { return GetCryptoSymbol(m_mapCryptoSymbol.at(strSymbol)); }

	bool Delete(CFinnhubCryptoSymbolPtr pSymbol);
	void Add(CFinnhubCryptoSymbolPtr pSymbol);

	bool LoadDB(void);
	bool UpdateDB(void);

	bool IsNeedUpdate(void);

protected:
	vector<CFinnhubCryptoSymbolPtr> m_vCryptoSymbol;
	map<CString, size_t> m_mapCryptoSymbol;
	long m_lLastTotalCryptoSymbol;
};
