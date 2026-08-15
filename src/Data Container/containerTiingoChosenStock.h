#pragma once

#include"ContainerVirtualStock.h"

class CTiingoStock;

using std::string;

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
	void UpdateDB(std::stop_token st) const;

	shared_ptr<CTiingoStock> GetStock(size_t lIndex);
	shared_ptr<CTiingoStock> GetStock(const string& strStockCode);

protected:
};

extern CContainerTiingoChosenStock gl_dataContainerTiingoChosenStock;
