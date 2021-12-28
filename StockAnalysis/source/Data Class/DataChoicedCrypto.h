#pragma once

#include"FinnhubCryptoSymbol.h"

using namespace std;
#include<vector>
#include<map>

class CDataChoicedCrypto : public CObject {
public:
	CDataChoicedCrypto();
	~CDataChoicedCrypto();
	void Reset(void);

	CFinnhubCryptoSymbolPtr GetCrypto(long lIndex) { return m_vWorldChoicedCrypto.at(lIndex); }
	size_t GetSize(void) { return m_vWorldChoicedCrypto.size(); }

	bool LoadDB(void);
	//bool UpdateDB(void);

protected:
	vector<CFinnhubCryptoSymbolPtr> m_vWorldChoicedCrypto;
	map<CString, long> m_mapWorldChoicedCrypto;
	long m_lChoicedCryptoPos;
};
