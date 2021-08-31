// SetForexExchange.h : CSetFinnhubForexExchange µÄÉùÃ÷

#pragma once
#include"afxdb.h"

#include"VirtualSetExchange.h"

class CSetFinnhubForexExchange : public CVirtualSetExchange
{
public:
	CSetFinnhubForexExchange(CString strTableName = _T("finnhub_forex_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(pDatabase) {
		sm_TableName = strTableName;
	}
	DECLARE_DYNAMIC(CSetFinnhubForexExchange)
};
