// CSetForexDayLine.h : CSetForexDayLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetForexDayLine : public CVirtualSetHistoryCandleBasic {
public:
	CSetForexDayLine(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("forex_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) { }
};
