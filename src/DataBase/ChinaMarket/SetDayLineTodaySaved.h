// CSetDayLineTodaySaved.h : CSetDayLineTodaySaved µÄÉùÃ÷

#pragma once

import FireBird.Set.VirtualHistoryCandle;

class CSetDayLineTodaySaved : public CVirtualSetHistoryCandle {
public:
	CSetDayLineTodaySaved(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("today"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}

	~CSetDayLineTodaySaved() override = default;
};
