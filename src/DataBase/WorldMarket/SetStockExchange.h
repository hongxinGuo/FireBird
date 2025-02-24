// CSetStockExchange.h : CSetStockExchange µÄÉùÃ÷

#pragma once

#include"VirtualSetExchange.h"

class CSetStockExchange : public CVirtualSetExchange {
public:
	CSetStockExchange(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) {}
};
