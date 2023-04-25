// CSetCryptoDayLine.h : CSetCryptoDayLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetCryptoDayLine : public CVirtualSetHistoryCandleBasic {
public:
	CSetCryptoDayLine(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("crypto_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) { }
};
