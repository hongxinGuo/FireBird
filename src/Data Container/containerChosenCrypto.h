#pragma once

#include"ContainerVirtualStock.h"

class CContainerChosenCrypto : public CContainerVirtualStock {
public:
	CContainerChosenCrypto();
	~CContainerChosenCrypto() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	//bool UpdateDB(void);

	CFinnhubCryptoSymbolPtr GetCryptoSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(lIndex)); }
	CFinnhubCryptoSymbolPtr GetCryptoSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(strStockCode)); }

protected:
	long m_lChosenCryptoPos;
};
