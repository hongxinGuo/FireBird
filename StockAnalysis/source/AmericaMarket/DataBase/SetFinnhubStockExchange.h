// CSetFinnhubStockExchange.h : CSetFinnhubStockExchange µÄÉùÃ÷

#pragma once

#include"afxdb.h"

#include"VirtualSetExchange.h"

class CSetFinnhubStockExchange : public CVirtualSetExchange
{
public:
	CSetFinnhubStockExchange(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_stock_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetFinnhubStockExchange)
};
