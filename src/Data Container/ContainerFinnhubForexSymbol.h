#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubForex.h"

class CContainerFinnhubForexSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubForexSymbol();
	CContainerFinnhubForexSymbol(const CContainerFinnhubForexSymbol& other) = delete;
	CContainerFinnhubForexSymbol(CContainerFinnhubForexSymbol&& other) noexcept = delete;
	CContainerFinnhubForexSymbol& operator=(const CContainerFinnhubForexSymbol& other) = delete;
	CContainerFinnhubForexSymbol& operator=(CContainerFinnhubForexSymbol&& other) noexcept = delete;
	~CContainerFinnhubForexSymbol() override;
	void Reset() override;

	bool LoadDB();
	bool UpdateDB();

	CForexSymbolPtr GetItem(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubForex>(Get(lIndex)); }
	CForexSymbolPtr GetItem(const string& strStockCode) { return dynamic_pointer_cast<CFinnhubForex>(Get(strStockCode)); }

protected:
	size_t m_lastTotalSymbol;
};
