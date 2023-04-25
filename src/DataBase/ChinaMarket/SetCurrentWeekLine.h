// SetCurrentWeekLine.h : CSetCurrentWeekLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetCurrentWeekLine : public CVirtualSetHistoryCandle {
public:
	CSetCurrentWeekLine(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("CurrentWeekLine"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) { }
};
