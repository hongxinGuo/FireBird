#pragma once

#include "ContainerVirtualStock.h"

class CTiingoCrypto;

class CContainerTiingoCryptoSymbol : public CContainerVirtualStock {
public:
	CContainerTiingoCryptoSymbol();
	CContainerTiingoCryptoSymbol(const CContainerTiingoCryptoSymbol& other) = delete;
	CContainerTiingoCryptoSymbol(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
	CContainerTiingoCryptoSymbol& operator=(const CContainerTiingoCryptoSymbol& other) = delete;
	CContainerTiingoCryptoSymbol& operator=(CContainerTiingoCryptoSymbol&& other) noexcept = delete;
	~CContainerTiingoCryptoSymbol() override = default;
	void Reset() override;

	shared_ptr<CTiingoCrypto> GetCrypto(size_t lIndex);
	shared_ptr<CTiingoCrypto> GetCrypto(const string& strCryptoCode);

	void UpdateDB();
	bool LoadDB();
};

extern CContainerTiingoCryptoSymbol gl_dataContainerTiingoCryptoSymbol;
