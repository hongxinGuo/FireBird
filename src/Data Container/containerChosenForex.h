#pragma once

#include"ContainerVirtualStock.h"
#include "FinnhubForexSymbol.h"

class CContainerChosenForex : public CContainerVirtualStock {
public:
	CContainerChosenForex();
	CContainerChosenForex(const CContainerChosenForex& other) = delete;
	CContainerChosenForex(CContainerChosenForex&& other) noexcept = delete;
	CContainerChosenForex& operator=(const CContainerChosenForex& other) = delete;
	CContainerChosenForex& operator=(CContainerChosenForex&& other) noexcept = delete;
	~CContainerChosenForex() override = default;
	void Reset() override;

	bool LoadDB();
	//bool UpdateDB();

	CForexSymbolPtr GetForexSymbol(const size_t lIndex) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(lIndex)); }
	CForexSymbolPtr GetForexSymbol(const CString& strStockCode) { return dynamic_pointer_cast<CFinnhubForexSymbol>(Get(strStockCode)); }

protected:
	long m_lChosenForexPos;
};
