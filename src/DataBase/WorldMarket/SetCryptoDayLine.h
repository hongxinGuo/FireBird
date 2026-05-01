// CSetCryptoDayLine.h : CSetCryptoDayLine 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetCryptoDayLine final : public CVirtualSetHistoryCandle {
public:
	CSetCryptoDayLine(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_crypto_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
	~CSetCryptoDayLine() override = default;
};
