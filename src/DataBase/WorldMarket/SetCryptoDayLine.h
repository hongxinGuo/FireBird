// CSetCryptoDayLine.h : CSetCryptoDayLine µÄÉùÃ÷

#pragma once

import FireBird.Set.VirtualHistoryCandleBasic;

class CSetCryptoDayLine final : public CVirtualSetHistoryCandleBasic {
public:
	CSetCryptoDayLine(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_crypto_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
