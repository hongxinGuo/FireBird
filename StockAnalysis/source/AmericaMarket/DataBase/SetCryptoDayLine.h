// CSetCryptoDayLine.h : CSetCryptoDayLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetCryptoDayLine : public CVirtualSetHistoryCandle
{
public:
	CSetCryptoDayLine(CString strSchema = _T("WorldMarket"), CString strTable = _T("crypto_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetCryptoDayLine)
};
