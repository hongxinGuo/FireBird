// SetChinaStockSymbol.h : CSetChinaStockSymbol µÄÉùÃ÷

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetChinaStockSymbol : public CVirtualSetStockSymbol {
public:
	CSetChinaStockSymbol(CString strSchema = _T("ChinaMarket"), CString strTable = _T("stockcode"), CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {
	}
};
