#pragma once

#include "TiingoCrypto.h"
#include "ContainerVirtualStock.h"

class CContainerTiingoCryptoSymbol : public CContainerVirtualStock {
public:
	CContainerTiingoCryptoSymbol();
	CContainerTiingoCryptoSymbol(const CContainerTiingoCryptoSymbol& other) = delete;
	CContainerTiingoCryptoSymbol(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
	CContainerTiingoCryptoSymbol& operator=(const CContainerTiingoCryptoSymbol& other) = delete;
	CContainerTiingoCryptoSymbol& operator=(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
	~CContainerTiingoCryptoSymbol() override;
	void Reset() override;

	CTiingoCryptoPtr GetCrypto(const size_t lIndex) { return dynamic_pointer_cast<CTiingoCrypto>(Get(lIndex)); }
	CTiingoCryptoPtr GetCrypto(const CString& strCryptoCode) { return dynamic_pointer_cast<CTiingoCrypto>(Get(strCryptoCode)); }

	void UpdateDB();
	bool LoadDB();
};
