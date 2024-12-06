// SetFinnhubForexSymbol.h : CSetFinnhubForexSymbol µÄÉùÃ÷

#pragma once
import FireBird.Set.VirtualStockSymbol;

class CSetFinnhubForexSymbol : public CVirtualSetStockSymbol {
public:
	CSetFinnhubForexSymbol(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_forex_symbol"), CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {}
};
