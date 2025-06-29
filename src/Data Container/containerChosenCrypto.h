#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubCrypto.h"

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

	CFinnhubCryptoPtr GetCryptoSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCrypto>(Get(lIndex)); }
	CFinnhubCryptoPtr GetCryptoSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCrypto>(Get(strStockCode.GetString())); }

protected:
	size_t m_lChosenCryptoPos;
};
