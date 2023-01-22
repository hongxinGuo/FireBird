// SetForexExchange.h : CSetFinnhubForexExchange µÄÉùÃ÷

#pragma once
#include"afxdb.h"

#include"VirtualSetExchange.h"

class CSetFinnhubForexExchange : public CVirtualSetExchange {
public:
	CSetFinnhubForexExchange(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_forex_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) {
	}
};
