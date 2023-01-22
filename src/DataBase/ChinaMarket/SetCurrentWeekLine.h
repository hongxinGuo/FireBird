// SetCurrentWeekLine.h : CSetCurrentWeekLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetCurrentWeekLine : public CVirtualSetHistoryCandle {
public:
	CSetCurrentWeekLine(CString strSchema = _T("ChinaMarket"), CString strTable = _T("CurrentWeekLine"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {
	}
};
