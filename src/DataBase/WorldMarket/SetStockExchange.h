// CSetStockExchange.h : CSetStockExchange 的声明

#pragma once

#include"VirtualSetExchange.h"

class CSetStockExchange : public CVirtualSetExchange {
public:
	CSetStockExchange(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_stock_exchange", CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) {}
	~CSetStockExchange() override = default;
};
