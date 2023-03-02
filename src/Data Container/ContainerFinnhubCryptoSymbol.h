#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubCryptoSymbol.h"
#include "FinnhubForexSymbol.h"

class CContainerFinnhubCryptoSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubCryptoSymbol();
	~CContainerFinnhubCryptoSymbol() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	bool UpdateDB(void);

	CFinnhubCryptoSymbolPtr GetSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(lIndex)); }
	CFinnhubCryptoSymbolPtr GetSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(strStockCode)); };

protected:
	long m_lLastTotalSymbol;
};
