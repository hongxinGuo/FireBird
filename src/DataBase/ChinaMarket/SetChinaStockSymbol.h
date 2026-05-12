// SetChinaStockSymbol.h : CSetChinaStockSymbol 的声明

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetChinaStockSymbol : public CVirtualSetStockSymbol {
public:
	CSetChinaStockSymbol(const CString& strSchema = "stock_market", const CString& strTable = "china_stock_code", CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {}
};
