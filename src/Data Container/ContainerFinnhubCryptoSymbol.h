#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubCryptoSymbol.h"

class CContainerFinnhubCryptoSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubCryptoSymbol();
	CContainerFinnhubCryptoSymbol(const CContainerFinnhubCryptoSymbol& other) = delete;
	CContainerFinnhubCryptoSymbol(CContainerFinnhubCryptoSymbol&& other) noexcept = delete;
	CContainerFinnhubCryptoSymbol& operator=(const CContainerFinnhubCryptoSymbol& other) = delete;
	CContainerFinnhubCryptoSymbol& operator=(CContainerFinnhubCryptoSymbol&& other) noexcept = delete;
	~CContainerFinnhubCryptoSymbol() override = default;
	void Reset() override;

	bool LoadDB();
	bool UpdateDB();

	CFinnhubCryptoSymbolPtr GetSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(lIndex)); }
	CFinnhubCryptoSymbolPtr GetSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(strStockCode)); };

protected:
	long m_lLastTotalSymbol;
};
