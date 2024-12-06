// SetCurrentWeekLine.h : CSetCurrentWeekLine µÄÉùÃ÷

#pragma once

import FireBird.Set.VirtualHistoryCandle;

class CSetCurrentWeekLine : public CVirtualSetHistoryCandle {
public:
	CSetCurrentWeekLine(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("CurrentWeekLine"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
};
