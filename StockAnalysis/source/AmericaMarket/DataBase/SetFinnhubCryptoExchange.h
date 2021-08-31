// SetCryptoExchange.h : CSetFinnhubCryptoExchange 的声明

#pragma once
#include"afxdb.h"

#include"VirtualSetExchange.h"

// 代码生成在 2019年5月26日, 8:14

class CSetFinnhubCryptoExchange : public CVirtualSetExchange
{
public:
	CSetFinnhubCryptoExchange(CString strTableName = _T("finnhub_crypto_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(pDatabase) {
		sm_TableName = strTableName;
	}
	DECLARE_DYNAMIC(CSetFinnhubCryptoExchange)
};
