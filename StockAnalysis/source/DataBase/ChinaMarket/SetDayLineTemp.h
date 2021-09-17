// CSetDayLineTemp.h : CSetDayLineTemp µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetDayLineTemp : public CVirtualSetHistoryCandle
{
public:
	CSetDayLineTemp(CString strSchema = _T("ChinaMarket"), CString strTable = _T("today"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetDayLineTemp)
};
