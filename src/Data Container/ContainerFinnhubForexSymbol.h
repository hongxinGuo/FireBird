#pragma once

#include"ContainerVirtualStock.h"

class CFinnhubForex;

class CContainerFinnhubForexSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubForexSymbol();
	CContainerFinnhubForexSymbol(const CContainerFinnhubForexSymbol& other) = delete;
	CContainerFinnhubForexSymbol(CContainerFinnhubForexSymbol&& other) noexcept = delete;
	CContainerFinnhubForexSymbol& operator=(const CContainerFinnhubForexSymbol& other) = delete;
	CContainerFinnhubForexSymbol& operator=(CContainerFinnhubForexSymbol&& other) noexcept = delete;
	~CContainerFinnhubForexSymbol() override = default;
	void Reset() override;

	bool LoadProfileDB();
	void UpdateProfileDB(std::stop_token st);

	shared_ptr<CFinnhubForex> GetItem(size_t lIndex);
	shared_ptr<CFinnhubForex> GetItem(const string& strStockCode);

protected:
	size_t m_lastTotalSymbol;
};

extern CContainerFinnhubForexSymbol gl_dataFinnhubForexSymbol;
