#pragma once

#include"ContainerVirtualStock.h"

class CAlpacaStock;

class CContainerAlpacaStockSymbol : public CContainerVirtualStock {
public:
	CContainerAlpacaStockSymbol();
	CContainerAlpacaStockSymbol(const CContainerAlpacaStockSymbol& other) = delete;
	CContainerAlpacaStockSymbol(CContainerAlpacaStockSymbol&& other) noexcept = delete;
	CContainerAlpacaStockSymbol& operator=(const CContainerAlpacaStockSymbol& other) = delete;
	CContainerAlpacaStockSymbol& operator=(CContainerAlpacaStockSymbol&& other) noexcept = delete;
	~CContainerAlpacaStockSymbol() override = default;
	void Reset() override;

	bool LoadProfileDB();
	void UpdateProfileDB(std::stop_token st);

	shared_ptr<CAlpacaStock> GetItem(size_t lIndex);
	shared_ptr<CAlpacaStock> GetItem(const string& strStockCode);

protected:
	size_t m_lastTotalSymbol;
};

extern CContainerAlpacaStockSymbol gl_dataContainerAlpacaStockSymbol;
