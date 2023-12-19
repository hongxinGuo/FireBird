#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubCryptoSymbol.h"

class CContainerFinnhubCryptoSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubCryptoSymbol();
	~CContainerFinnhubCryptoSymbol() override = default;
	void Reset();

	bool LoadDB();
	bool UpdateDB();

	CFinnhubCryptoSymbolPtr GetSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(lIndex)); }
	CFinnhubCryptoSymbolPtr GetSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(strStockCode)); };

protected:
	long m_lLastTotalSymbol;
};
