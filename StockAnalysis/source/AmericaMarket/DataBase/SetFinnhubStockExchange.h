// CSetFinnhubStockExchange.h : CSetFinnhubStockExchange µÄÉùÃ÷

#pragma once

#include"afxdb.h"

#include"VirtualSetExchange.h"

class CSetFinnhubStockExchange : public CVirtualSetExchange
{
public:
	CSetFinnhubStockExchange(CString strTableName = _T("finnhub_stock_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(pDatabase) {
		sm_TableName = strTableName;
	}
	DECLARE_DYNAMIC(CSetFinnhubStockExchange)
};
