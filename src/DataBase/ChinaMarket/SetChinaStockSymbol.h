// SetChinaStockSymbol.h : CSetChinaStockSymbol 的声明

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetChinaStockSymbol : public CVirtualSetStockSymbol {
public:
	CSetChinaStockSymbol(const CString& strSchema = "ChinaMarket", const CString& strTable = "stockcode", CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {}
};
