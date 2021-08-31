// SetCryptoExchange.h : CSetFinnhubCryptoExchange ������

#pragma once
#include"afxdb.h"

#include"VirtualSetExchange.h"

// ���������� 2019��5��26��, 8:14

class CSetFinnhubCryptoExchange : public CVirtualSetExchange
{
public:
	CSetFinnhubCryptoExchange(CString strTableName = _T("finnhub_crypto_exchange"), CDatabase* pDatabase = nullptr)
		: CVirtualSetExchange(pDatabase) {
		sm_TableName = strTableName;
	}
	DECLARE_DYNAMIC(CSetFinnhubCryptoExchange)
};
