// CSetFinnhubStockExchange.h : CSetFinnhubStockExchange µÄÉùÃ÷

#pragma once

#include"VirtualSetExchange.h"

class CSetFinnhubStockExchange : public CVirtualSetExchange {
public:
	CSetFinnhubStockExchange(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) { }
};
