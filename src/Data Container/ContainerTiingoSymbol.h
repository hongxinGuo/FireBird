#pragma once

#include"ContainerVirtualStock.h"

class CTiingoStock;

class CContainerTiingoSymbol : public CContainerVirtualStock {
public:
	CContainerTiingoSymbol();
	CContainerTiingoSymbol(const CContainerTiingoSymbol& other) = delete;
	CContainerTiingoSymbol(CContainerTiingoSymbol&& other) noexcept = delete;
	CContainerTiingoSymbol& operator=(const CContainerTiingoSymbol& other) = delete;
	CContainerTiingoSymbol& operator=(CContainerTiingoSymbol&& other) noexcept = delete;
	~CContainerTiingoSymbol() override = default;
	void Reset() override;

	shared_ptr<CTiingoStock> GetStock(size_t lIndex);
	shared_ptr<CTiingoStock> GetStock(const string& strStockCode);
};

using CContainerTiingoSymbolPtr = std::shared_ptr<CContainerTiingoSymbol>;

inline CContainerTiingoSymbolPtr gl_pDataContainerTiingoNewSymbol = nullptr;
inline CContainerTiingoSymbolPtr gl_pDataContainerTiingoDelistedSymbol = nullptr;
