// CSetCryptoDayLine.h : CSetCryptoDayLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetCryptoDayLine : public CVirtualSetHistoryCandleBasic
{
public:
	CSetCryptoDayLine(CString strSchema = _T("WorldMarket"), CString strTable = _T("crypto_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetCryptoDayLine)
};
