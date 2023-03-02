#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubForexSymbol.h"

class CContainerFinnhubForexSymbol : public CContainerVirtualStock {
public:
	CContainerFinnhubForexSymbol();
	~CContainerFinnhubForexSymbol() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	bool UpdateDB(void);

	CForexSymbolPtr GetSymbol(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(lIndex)); }
	CForexSymbolPtr GetSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(strStockCode)); }

protected:
	size_t m_lastTotalSymbol;
};
