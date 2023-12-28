#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubForexSymbol.h"

class CContainerFinnhubForexSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubForexSymbol();
	CContainerFinnhubForexSymbol(const CContainerFinnhubForexSymbol& other) = delete;
	CContainerFinnhubForexSymbol(CContainerFinnhubForexSymbol&& other) noexcept = delete;
	CContainerFinnhubForexSymbol& operator=(const CContainerFinnhubForexSymbol& other) = delete;
	CContainerFinnhubForexSymbol& operator=(CContainerFinnhubForexSymbol&& other) noexcept = delete;
	~CContainerFinnhubForexSymbol() override = default;
	void Reset() override;

	bool LoadDB();
	bool UpdateDB();

	CForexSymbolPtr GetSymbol(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(lIndex)); }
	CForexSymbolPtr GetSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(strStockCode)); }

protected:
	size_t m_lastTotalSymbol;
};
