// SetForexExchange.h : CSetFinnhubForexExchange ������

#pragma once
#include"afxdb.h"

#include"VirtualSetExchange.h"

class CSetFinnhubForexExchange : public CVirtualSetExchange {
public:
	CSetFinnhubForexExchange(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_forex_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) { }
};
