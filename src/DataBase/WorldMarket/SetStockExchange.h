// CSetStockExchange.h : CSetStockExchange µÄÉùÃ÷

#pragma once

#include"VirtualSetExchange.h"

class CSetStockExchange : public CVirtualSetExchange {
public:
	CSetStockExchange(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_stock_exchange", CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) {}
};
