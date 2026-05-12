// SetFinnhubCryptoSymbol.h : CSetFinnhubCryptoSymbol 的声明

#pragma once
#include"VirtualSetStockSymbol.h"

class CSetFinnhubCryptoSymbol : public CVirtualSetStockSymbol {
public:
	CSetFinnhubCryptoSymbol(const CString& strSchema = "stock_market", const CString& strTable = "finnhub_crypto_symbol", CDatabase* pDatabase = nullptr)
		: CVirtualSetStockSymbol(strSchema, strTable, pDatabase) {}
	~CSetFinnhubCryptoSymbol() override = default;
};
