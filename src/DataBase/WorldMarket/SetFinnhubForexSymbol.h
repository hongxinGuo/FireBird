// SetFinnhubForexSymbol.h : CSetFinnhubForexSymbol µÄÉùÃ÷

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetFinnhubForexSymbol : public CVirtualSetStockSymbol {
public:
	CSetFinnhubForexSymbol(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_forex_symbol", CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {}
};
