// SetFinnhubCryptoSymbol.h : CSetFinnhubCryptoSymbol ������

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetFinnhubCryptoSymbol : public CVirtualSetStockSymbol {
public:
	CSetFinnhubCryptoSymbol(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_crypto_symbol"), CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) { }
};
