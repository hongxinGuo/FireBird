// SetFinnhubForexSymbol.h : CSetFinnhubForexSymbol µÄÉùÃ÷

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetFinnhubForexSymbol : public CVirtualSetStockSymbol
{
public:
	CSetFinnhubForexSymbol(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_forex_symbol"), CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetFinnhubForexSymbol)
};
