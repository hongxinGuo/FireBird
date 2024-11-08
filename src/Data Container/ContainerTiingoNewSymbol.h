#pragma once

#include"TiingoStock.h"
#include"ContainerVirtualStock.h"

class CContainerTiingoNewSymbol : public CContainerVirtualStock {
public:
	CContainerTiingoNewSymbol();
	CContainerTiingoNewSymbol(const CContainerTiingoNewSymbol& other) = delete;
	CContainerTiingoNewSymbol(CContainerTiingoNewSymbol&& other) noexcept = delete;
	CContainerTiingoNewSymbol& operator=(const CContainerTiingoNewSymbol& other) = delete;
	CContainerTiingoNewSymbol& operator=(CContainerTiingoNewSymbol&& other) noexcept = delete;
	~CContainerTiingoNewSymbol() override;
	void Reset() override;

	CTiingoStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CTiingoStock>(Get(lIndex)); }
	CTiingoStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode)); }
};
