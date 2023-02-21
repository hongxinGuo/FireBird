// CSetDayLineTodaySaved.h : CSetDayLineTodaySaved µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetDayLineTodaySaved : public CVirtualSetHistoryCandle {
public:
	CSetDayLineTodaySaved(CString strSchema = _T("ChinaMarket"), CString strTable = _T("today"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) { }

	~CSetDayLineTodaySaved() override = default;
};
