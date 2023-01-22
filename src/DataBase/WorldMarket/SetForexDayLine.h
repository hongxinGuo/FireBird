// CSetForexDayLine.h : CSetForexDayLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetForexDayLine : public CVirtualSetHistoryCandleBasic {
public:
	CSetForexDayLine(CString strSchema = _T("WorldMarket"), CString strTable = _T("forex_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
	}
};
