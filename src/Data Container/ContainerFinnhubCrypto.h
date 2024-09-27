#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubCrypto.h"

class CContainerFinnhubCrypto : public CContainerVirtualStock {
public:
	CContainerFinnhubCrypto();
	CContainerFinnhubCrypto(const CContainerFinnhubCrypto& other) = delete;
	CContainerFinnhubCrypto(CContainerFinnhubCrypto&& other) noexcept = delete;
	CContainerFinnhubCrypto& operator=(const CContainerFinnhubCrypto& other) = delete;
	CContainerFinnhubCrypto& operator=(CContainerFinnhubCrypto&& other) noexcept = delete;
	~CContainerFinnhubCrypto() override = default;
	void Reset() override;

	bool LoadDB();
	bool UpdateDB();

	CFinnhubCryptoPtr GetSymbol(size_t lIndex) { return dynamic_pointer_cast<CFinnhubCrypto>(Get(lIndex)); }
	CFinnhubCryptoPtr GetSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubCrypto>(Get(strStockCode)); };

protected:
	long m_lLastTotalSymbol;
};
