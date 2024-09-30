#pragma once

#include"WorldStock.h"
#include"TiingoStock.h"

#include"ContainerVirtualStock.h"

class CContainerTiingoStock : public CContainerVirtualStock {
public:
	CContainerTiingoStock();
	CContainerTiingoStock(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock(CContainerTiingoStock&& other) noexcept = delete;
	CContainerTiingoStock& operator=(const CContainerTiingoStock& other) = delete;
	CContainerTiingoStock& operator=(CContainerTiingoStock&& other) noexcept = delete;
	~CContainerTiingoStock() override = default;
	void Reset();

	CTiingoStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CTiingoStock>(Get(lIndex)); }
	CTiingoStockPtr GetStock(const CString& strStockCode) { return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode)); }

	void UpdateDB();
	bool LoadDB();

	bool IsUpdateFinancialStateDB() noexcept;
	void UpdateFinancialStateDB() const;
};
