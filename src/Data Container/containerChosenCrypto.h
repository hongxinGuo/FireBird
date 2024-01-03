#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubCryptoSymbol.h"

class CContainerChosenCrypto final : public CContainerVirtualStock {
public:
	CContainerChosenCrypto();
	CContainerChosenCrypto(const CContainerChosenCrypto& other) = delete;
	CContainerChosenCrypto(CContainerChosenCrypto&& other) noexcept = delete;
	CContainerChosenCrypto& operator=(const CContainerChosenCrypto& other) = delete;
	CContainerChosenCrypto& operator=(CContainerChosenCrypto&& other) noexcept = delete;
	~CContainerChosenCrypto() override = default;
	void Reset() override;

	bool LoadDB();
	//bool UpdateDB();

	CFinnhubCryptoSymbolPtr GetCryptoSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(lIndex)); }
	CFinnhubCryptoSymbolPtr GetCryptoSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCryptoSymbol>(Get(strStockCode)); }

protected:
	long m_lChosenCryptoPos;
};
