// CSetForexDayLine.h : CSetForexDayLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetForexDayLine : public CVirtualSetHistoryCandle
{
public:
	CSetForexDayLine(CString strSchema = _T("WorldMarket"), CString strTable = _T("forex_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetForexDayLine)
};
