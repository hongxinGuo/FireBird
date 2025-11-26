// SetForexExchange.h : CSetFinnhubForexExchange 的声明

#pragma once
#include"afxdb.h"

#include"VirtualSetExchange.h"

class CSetFinnhubForexExchange : public CVirtualSetExchange {
public:
	CSetFinnhubForexExchange(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_forex_exchange", CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) {}
};
