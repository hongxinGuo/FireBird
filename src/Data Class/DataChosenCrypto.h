#pragma once

#include"FinnhubCryptoSymbol.h"

class CDataChosenCrypto final {
public:
	CDataChosenCrypto();
	virtual ~CDataChosenCrypto() = default;
	void Reset(void);

	CFinnhubCryptoSymbolPtr GetCrypto(const long lIndex) { return m_vWorldChosenCrypto.at(lIndex); }
	[[nodiscard]] size_t GetSize(void) const noexcept { return m_vWorldChosenCrypto.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CFinnhubCryptoSymbolPtr> m_vWorldChosenCrypto;
	map<CString, size_t> m_mapWorldChosenCrypto;
	long m_lChosenCryptoPos;
};