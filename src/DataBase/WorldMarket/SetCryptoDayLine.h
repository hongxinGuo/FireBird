// CSetCryptoDayLine.h : CSetCryptoDayLine 的声明

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetCryptoDayLine final : public CVirtualSetHistoryCandleBasic {
public:
	CSetCryptoDayLine(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_crypto_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
