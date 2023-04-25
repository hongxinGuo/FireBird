// SetCryptoExchange.h : CSetFinnhubCryptoExchange ������

#pragma once
#include"afxdb.h"

#include"VirtualSetExchange.h"

// ���������� 2019��5��26��, 8:14

class CSetFinnhubCryptoExchange : public CVirtualSetExchange {
public:
	CSetFinnhubCryptoExchange(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_crypto_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(strSchema, strTable, pDatabase) { }
};
