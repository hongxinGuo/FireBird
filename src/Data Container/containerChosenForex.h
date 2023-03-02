#pragma once

#include"ContainerVirtualStock.h"

class CContainerChosenForex : public CContainerVirtualStock {
public:
	CContainerChosenForex();
	~CContainerChosenForex() override = default;
	void Reset(void) override;

	bool LoadDB(void);
	//bool UpdateDB(void);

	CForexSymbolPtr GetForexSymbol(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(lIndex)); }
	CForexSymbolPtr GetForexSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(strStockCode)); }

protected:
	long m_lChosenForexPos;
};
