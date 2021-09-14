// SetFinnhubCryptoSymbol.h : CSetFinnhubCryptoSymbol µÄÉùÃ÷

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetFinnhubCryptoSymbol : public CVirtualSetStockSymbol
{
public:
	CSetFinnhubCryptoSymbol(CString strSchema = _T("WorldMarket"), CString strTable = _T("finnhub_crypto_symbol"), CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetFinnhubCryptoSymbol)
};
