// SetFinnhubCryptoSymbol.h : CSetFinnhubCryptoSymbol µÄÉùÃ÷

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetFinnhubCryptoSymbol : public CVirtualSetStockSymbol {
public:
	CSetFinnhubCryptoSymbol(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_crypto_symbol", CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {}
};
