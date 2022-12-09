#pragma once

#include"FinnhubCryptoSymbol.h"

#include<vector>
#include<map>

class CDataChosenCrypto final : public CObject {
public:
	CDataChosenCrypto();
	~CDataChosenCrypto() override = default;
	void Reset(void);

	CFinnhubCryptoSymbolPtr GetCrypto(long lIndex) { return m_vWorldChosenCrypto.at(lIndex); }
	size_t GetSize(void) const noexcept { return m_vWorldChosenCrypto.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CFinnhubCryptoSymbolPtr> m_vWorldChosenCrypto;
	map<CString, long> m_mapWorldChosenCrypto;
	long m_lChosenCryptoPos;
};
