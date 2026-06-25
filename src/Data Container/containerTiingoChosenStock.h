#pragma once

#include"ContainerVirtualStock.h"
#include "TiingoStock.h"

class CContainerTiingoChosenStock : public CContainerVirtualStock {
public:
	CContainerTiingoChosenStock();
	CContainerTiingoChosenStock(const CContainerTiingoChosenStock& other) = delete;
	CContainerTiingoChosenStock(CContainerTiingoChosenStock&& other) noexcept = delete;
	CContainerTiingoChosenStock& operator=(const CContainerTiingoChosenStock& other) = delete;
	CContainerTiingoChosenStock& operator=(CContainerTiingoChosenStock&& other) noexcept = delete;
	~CContainerTiingoChosenStock() override = default;
	void Reset() override;

	bool LoadDB();
	//bool UpdateDB();

	CTiingoStockPtr GetStock(const size_t lIndex) { return dynamic_pointer_cast<CTiingoStock>(Get(lIndex)); };
	CTiingoStockPtr GetStock(const string& strStockCode) { return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode)); };

protected:
};
