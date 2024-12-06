// SetForexExchange.h : CSetFinnhubForexExchange µÄÉùÃ÷

#pragma once
#include"afxdb.h"

import FireBird.Set.Exchange;

class CSetFinnhubForexExchange : public CVirtualSetExchange {
public:
	CSetFinnhubForexExchange(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_forex_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) {}
};
